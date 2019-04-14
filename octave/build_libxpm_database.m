function build_libxpm_database(fs,channel_power,D,alphaDB,gam,res_Disp,HalfWindow)
  fs_num = length(fs);
  xpm_matrix = zeros(fs_num,fs_num);
  
  for i=1:fs_num
      %comput xpm cross-phase effect of all other channels on fs(i)
       for j=1:fs_num
           %is j within i's half window?
           if (abs(i - j) <= HalfWindow)
               frow = fs(i);
               fcol = fs(j);
               if (frow~=fcol)
                   xpm_matrix(i,j) = XPM_noise_term(channel_power,frow,fcol,D,alphaDB,gam,1,'cosine',res_Disp);
               else
                   xpm_matrix(i,j) = 0;
               end
           end
       end
  end
  
  save xpm_inputs.mat fs channel_power D alphaDB gam res_Disp HalfWindow
  save xpm_database.mat xpm_matrix;
  

function  noise = XPM_noise_term(channel_power,frow,fcol,p_D,alphaDB,gamma,Spans,waveform_str,res_Disp)
global c  BR  R  N D;
global pre_Disp under_Disp  last_Disp;
global alpha r BWD_opt waveform;

c             =  2.99792457778e+8;
BR            =  10e+9;
R             =  0.5;                          % pulse roll-off factor
lambda_row    =  c/frow;                         % wavelength of the center channel
N             =  Spans;                       
D             =  p_D;                          % dispersion parameter of NZDSF 
pre_Disp      =  0;                            % pre-compensation
under_Disp    =  0;                            % under-compensation per span
last_Disp     =  res_Disp - pre_Disp - N*under_Disp;
waveform      =  waveform_str;
alpha         =  alphaDB*0.1/log10(exp(1));    % alphauation of NZDSF
r             =  gamma;                        % nonlinear coefficient of NZDSF
BWD_opt       =  12.5e+9;                      % bandwidth of optical filter
lambda_col    =  c/fcol;

f_begin       = -8*BR;          f_end      = 8*BR;   
w_begin       = 2*pi*f_begin;   w_end      = 2*pi*f_end;   % define w scope for numerical integration

Pmax          = channel_power;
Pdc           = channel_power;                % power of the fc channel. DC
noise         = xpmNOISE_part1(Pdc,Pmax,w_begin,w_end,lambda_row,lambda_col);

   
%%%%%%%%%%%%%%%% tranfer funciton of XPM generated noise%%%%%%%%%%%%%%%%%%
% w         : modulation frequency
% lambdai   : wavelength of the prob channel
% lambdak   : wavelength of the pump channel

function  ret = HW(w,lambdai,lambdak)
global c  N D last_Disp alpha r; 
        % compute the noise generate by the phase-intensity conversion.
        % we have assumed that noise generated by DCFs are neglegible.

      aik         = alpha - j*w*D*(lambdai-lambdak);
      bi          = w^2*D*lambdai^2/(4*pi*c);
      Ci          = w^2*lambdai^2*last_Disp/(4*pi*c);
                
      term1        = aik*sin(Ci)-2*bi*cos(Ci);
      term2        = term1/(aik^2+4*bi^2);
      term3        = term2 + sin(Ci)/aik;
      
      ret          = 2*r*N*term3;

% the optical filter is assued to be a first-order gaussian filter
function ret = opt_gauss_filter(w)
global BWD_opt; 

BWD_W  = 2*pi*BWD_opt;

ret   = exp(-2*log(2)*(w/BWD_W)^2);

function ret = elc_gauss_filter(w)
global BWD_elc; 

BWD_W  = 2*pi*BWD_elc;

ret   = exp(-2*log(2)*(w/BWD_W)^2);

function ret = non_filter(w)
ret =1;

%%%%%%%%%%%%%%%%%% one part spectrum density of NRZ noise%%%%%%%%%%%%%%%%%%
function psd  = Square_cosine_psd(w) 
 global BR;

 T       = 1/BR;
 term1   = f1(T,w);
 term2   = sinc(T*w/(2*pi));
 psd     = (term1*term2)^2*T;

function ret = f1(T,w)
global R;

if (R*T*w/pi)^2 ==1 
    ret = pi^2/8;  
else
    ret = cos(R*T*w/2)/(1-(R*T*w/pi)^2);
end

function psd = On_off_psd(w)
 global BR;

 T       = 1/BR;
 term    = sinc(T*w/2/pi);
 psd     = (term)^2*T;


%%%%%%%%%%%%%%part1 of XPM noise, using numerical integration%%%%%%%%%%%
function ret = part1_integral(ws,lambdai, lambdak)
global waveform;

   ret     = zeros(1,length(ws));
 
   for i=1:length(ws)  
     hw     = HW(ws(i),lambdai,lambdak);
     %hflt  = opt_gauss_filter(ws(i));
     %hflt  = elc_gauss_filter(ws(i));
     hflt   = non_filter(ws(i));
     
     if (strcmp(waveform,'cosine'))
       psd    = Square_cosine_psd(ws(i));
     end
     if (strcmp(waveform,'rectangle'))
       psd    = On_off_psd(ws(i));
     end
    
     ret(i) = abs(hw^2)*abs(hflt^2)*psd;
   end


function ret = ing_engine(Pmax,w_begin,w_end,lambdai,lambdak)
     
    ret  = 1/(2*pi)*quad(@part1_integral,w_begin,w_end,[],[],lambdai,lambdak)*Pmax^2/4;     
  

function ret = xpmNOISE_part1(Pdc,Pmax,w_begin,w_end,lambdai,lambdak)
 
  ret  = Pdc^2*ing_engine(Pmax,w_begin,w_end,lambdai,lambdak);
