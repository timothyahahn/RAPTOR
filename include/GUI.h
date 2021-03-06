// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      GUI.cpp
//  Author:         Andrew Albers, Montana State University
//  Project:        raptor
//
//  Description:    The file contains many of the functions necessary for the
//					GUI.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  05/20/2009	v1.0	Initial Version.
//  04/14/2019  v2.0    Reworked version based upon cmake and octave
//
// ____________________________________________________________________________

#ifndef GUI_H
#define GUI_H

#include <cstdlib>

#include "Edge.h"
#include "Thread.h"

#include "allegro5/allegro.h"

extern Thread* threadZero;
extern Thread** threads;

extern void runSimulation(int argc, const char* argv[]);

void paint_link_usage(int p);
void paint_router_nums();
void paint_num_dests();
void paint_num_prob_fails();
void paint_num_qual_fails();
void explore_history();

void show_spans();
void show_maximums();
void show_averages();
void show_indexes();
void show_workstations();
void show_failures_from();
void show_failures_to();
void show_key();
void show_detailScreen();

void topo_zoomin();
void topo_zoomout();
void topo_move();

void graphleft();
void graphright();
void graphpoint();
void graphzoomin();
void graphzoomout();
void graphzoomwindow();
void graphmove();

void buildTopoSetFunction(int tf);
void buildtopoplacerouter();
void buildtopoplaceedge();
void buildtopomoverouter();
void buildtopoeditrouter();
const char* edit_inputter(bool alignleft, int xpx, int ypx, int wid, int hei);
int edit_inputint(bool alignleft, int edit, int xpx, int ypx, int wid, int hei);
void buildtopoeditedge();
void scaleAllEdgesTo(int spns, int px);
void paint_topo_edges(int p);
void topoRefresh();

void switcher(int argc, const char* argv[]);

bool userDisplayEdge();
void initialize_topbuttons();
void initialize_graphbuttons();
bool buildTopoMenu();

void userBuildTopo();
void userLoadPrevSim(int argc, const char* argv[]);
void userEditConfigs();
void userRunNewSim(int argc, const char* argv[]);

void menu();

int functionactive;  // codes: -1 = nonechosen, 0 = exit, 1 = runNewSim, 2 =
                     // loadPrevSim, 3 = buildTopo, 4 = editConfig
int mx, my, mb, mr, color, color2, white, explore_time, graphX1, graphY1,
    graphX2, graphY2, paintNum, usageWindow, startTime, nums;
int topoX1, topoX2, topoY1, topoY2;
int realTopoWidthPx, realTopoHeightPx, bkgrndX, bkgrndY, numTopoWorkstations,
    numTopoEdges;
int showrouternum;  // codes : showindexes (1), showwks (2), showfailurefrom
                    // (3), showfailureto (4) are all represented by values for
                    // showrouternum
int graphfunction;  // codes: 1 = pointer, 2 = zoomin 3 = zoomout 4 =
                    // zoomwindow, 5 = move //graphfunction
int topofunction;  // codes: 1 = addrouter, 2 = addedge, 3 = moverouter, 4 =
                   // editrouter, 5 = editedge
bool is_exploring, showcolorkey, showmaximums,
    showaverages;  // showindexes,showwks, showfailurefrom,showfailureto

std::vector<Router*> topoRouters;  // for building topology

double clickdist, rx, ry, dmx, dmy, shrinkConstant;
Edge* edge;

ALLEGRO_BITMAP* flashscreen;
ALLEGRO_BITMAP* edgeOriginals[14];
ALLEGRO_BITMAP* arrowOriginals[14];
ALLEGRO_BITMAP* tailOriginals[14];
ALLEGRO_BITMAP* topbutton[10];
ALLEGRO_BITMAP* topobutton[7];
ALLEGRO_BITMAP* topobuttonover[5];
ALLEGRO_BITMAP* topbuttonpress[9];
ALLEGRO_BITMAP* graphbutton[8];
ALLEGRO_BITMAP* graphbuttonpress[7];
ALLEGRO_BITMAP* menubutton[5];
ALLEGRO_BITMAP* menubuttonover[5];
ALLEGRO_BITMAP* topomenuover[2];

extern ALLEGRO_BITMAP* buffer;
extern ALLEGRO_BITMAP* pointer;
extern ALLEGRO_BITMAP* edgespans;
extern ALLEGRO_BITMAP* graph;
extern ALLEGRO_BITMAP* graphbuttons;
extern ALLEGRO_BITMAP* topbuttons;
extern ALLEGRO_BITMAP* routersbmp;
extern ALLEGRO_BITMAP* editrouterinfo;
extern ALLEGRO_BITMAP* editedgeinfo;
extern ALLEGRO_BITMAP* edgesbmp;
extern ALLEGRO_BITMAP* mainbuf;
extern ALLEGRO_BITMAP* popup;
extern ALLEGRO_BITMAP* graphbackground;
extern ALLEGRO_BITMAP* topobackground;
extern ALLEGRO_BITMAP* colorkey;
extern ALLEGRO_BITMAP* topomenu;

#define SCRNHEI 700
#define SCRNWID 1300

extern char foldName[25];
extern char folder[50];

#endif
