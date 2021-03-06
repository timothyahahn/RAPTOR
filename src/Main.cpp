// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      Main.cpp
//  Author:         Timothy Hahn, PhD
//  Project:        raptor
//
//  Description:    The file contains the implementation of main(), whose only
//					purpose is to pass the command line arguments to
//the controller.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  05/20/2009	v1.0	Initial Version.
//  04/14/2019  v2.0    Reworked version based upon cmake and octave
//
// ____________________________________________________________________________

#include "ErrorCodes.h"
#include "Thread.h"

#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

#include <iostream>
#include <sstream>
#include <vector>

#ifndef NO_ALLEGRO

#include "allegro5/allegro.h"

extern void close_button_handler();
extern void flash();
extern void explore_history();

extern void switcher(int argc, const char *argv[]);

extern int color, color2, white, explore_time;

ALLEGRO_BITMAP *buffer;
ALLEGRO_BITMAP *pointer;
ALLEGRO_BITMAP *graph;
ALLEGRO_BITMAP *graphbuttons;
ALLEGRO_BITMAP *topbuttons;
ALLEGRO_BITMAP *edgespans;
ALLEGRO_BITMAP *routersbmp;
ALLEGRO_BITMAP *routerinfo;
ALLEGRO_BITMAP *editrouterinfo;
ALLEGRO_BITMAP *editedgeinfo;
ALLEGRO_BITMAP *edgesbmp;
ALLEGRO_BITMAP *mainbuf;
ALLEGRO_BITMAP *progbarbmp;
ALLEGRO_BITMAP *routerpic;
ALLEGRO_BITMAP *graphbackground;
ALLEGRO_BITMAP *topobackground;
ALLEGRO_BITMAP *topomenu;
ALLEGRO_BITMAP *popup;
ALLEGRO_BITMAP *detailinfo;

ALLEGRO_BITMAP *colorkey;

char foldName[25];
char folder[50];

#define SCRNWID 1300
#define SCRNHEI 700
#endif

// Hate to make a global instance of the threads, but the other classes
// need access to the threads information. They can't just include a
// pointer to the class due to a circular reference.
Thread *threadZero = nullptr;
Thread **threads = nullptr;

size_t threadCount = 0;

std::vector<AlgorithmToRun *> algParams;

void *runThread(void *n);
void runSimulation(int argc, const char *argv[]);

pthread_mutex_t ScheduleMutex;

int main(int argc, const char *argv[]) {
  if (argc != 7) {
    std::cerr << "Usage: " << argv[0]
              << " <Topology> <Wavelengths> <Random Seed> <Thread Count> "
                 "<Iteration Count> <Probe Count>"
              << std::endl;
    std::cerr << std::endl;

    std::cerr << "Topology: NSF, Mesh, Mesh6x6, Mesh8x8, Mesh10x10"
              << std::endl;
    std::cerr << "Wavelengths: 21, 41, 81, 161, 321, 641, 1281" << std::endl;
    std::cerr << "Random Seed: <any valid unsigned int>" << std::endl;
    std::cerr << "Thread Count: <maximum number of threads to run, 1 to n>"
              << std::endl;
    std::cerr << "Iteration Count: <number of iterations, 1 to n>" << std::endl;
    std::cerr << "Probe Count: <number of probes, 1 to n>" << std::endl;

    return ERROR_INVALID_PARAMETERS;
  }

#ifndef NO_ALLEGRO
  al_install_system(ALLEGRO_VERSION_INT, nullptr);

  set_close_button_callback(close_button_handler);
  set_color_depth(16);

  if (!al_install_keyboard()) return ERROR_GUI;

  if (!al_install_mouse()) return ERROR_GUI;

  install_timer();

  set_window_title(
      "RAPTOR (Route Assignment Program for Transparent Optical Routes)");

  graph = al_create_bitmap(SCRNWID, SCRNHEI);
  graphbuttons = al_create_bitmap(SCRNWID, 16);
  topbuttons = al_create_bitmap(SCRNWID, 31);
  buffer = al_create_bitmap(SCRNWID, SCRNHEI);
  mainbuf = al_create_bitmap(SCRNWID, SCRNHEI);
  routersbmp = al_create_bitmap(SCRNWID, SCRNHEI);
  edgesbmp = al_create_bitmap(SCRNWID, SCRNHEI);
  edgespans = al_create_bitmap(SCRNWID, SCRNHEI);
  popup = al_create_bitmap(SCRNWID, SCRNHEI);

  graphbackground = al_load_bitmap("bitmaps/graphbackground.bmp");
  topobackground = al_load_bitmap("bitmaps/topobackground2.bmp");
  progbarbmp = al_load_bitmap("bitmaps/progressbar.bmp");
  routerinfo = al_load_bitmap("bitmaps/routerinfo.bmp");
  editrouterinfo = al_load_bitmap("bitmaps/editrouterinfo.bmp");
  editedgeinfo = al_load_bitmap("bitmaps/editedgeinfo.bmp");
  detailinfo = al_load_bitmap("bitmaps/configurationinfo.bmp");
  colorkey = al_load_bitmap("bitmaps/colorkey.bmp");
  pointer = al_load_bitmap("bitmaps/pointer2.bmp");
  topomenu = al_load_bitmap("bitmaps/topomenu.bmp");

  flash();

  set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCRNWID, SCRNHEI, 0, 0);

  color = makecol(0, 0, 0);
  color2 = makecol(0, 255, 0);
  white = makecol(255, 255, 255);

  set_mouse_sprite(pointer);
  set_mouse_sprite_focus(15, 15);

  show_mouse(screen);

  set_display_switch_mode(SWITCH_BACKGROUND);

  switcher(argc, argv);

  set_mouse_sprite(nullptr);

  al_destroy_bitmap(buffer);
  al_destroy_bitmap(graph);
  al_destroy_bitmap(pointer);
  al_destroy_bitmap(topobackground);
  al_destroy_bitmap(popup);
  al_destroy_bitmap(routerinfo);

  al_uninstall_system();
#else
  runSimulation(argc, argv);
#endif

  return 0;
}

void runSimulation(int argc, const char *argv[]) {
  int *threadZeroReturn = 0;

  std::vector<pthread_t *> pThreads;

  threadCount = atoi(argv[4]);

  threads = new Thread *[threadCount];

  unsigned int iterationCount = atoi(argv[5]);

#ifndef NO_ALLEGRO
  rectfill(screen, 0, 0, SCREEN_W, 40, color);
  textprintf_ex(screen, font, 20, 15, color2, color,
                "Building XPM Database, please wait...");
#endif

  Thread *thread = new Thread(0, argc, argv, false);

  threadZero->initResourceManager();

#ifndef NO_ALLEGRO
  rectfill(screen, 0, 0, SCREEN_W, 40, color);
#endif

  pthread_mutex_init(&ScheduleMutex, nullptr);

  if (threadCount > algParams.size()) threadCount = algParams.size();

  for (size_t t = 1; t < threadCount; ++t) {
    Thread *thread = new Thread(t, argc, argv, false);
    pThreads.push_back(new pthread_t);

    thread->initResourceManager();
  }

  std::ostringstream buffer;
  buffer << "Created " << threadCount << " threads " << std::endl;
  threadZero->recordEvent(buffer.str(), true, 0);
  threadZero->flushLog(true);

  for (size_t t = 1; t < threadCount; ++t) {
    int ret_code =
        pthread_create(pThreads[t - 1], nullptr, runThread, new size_t(t));

    if (ret_code != 0) {
      std::cerr << "ERROR: Thread creation failed with code: " << ret_code
                << std::endl;
      return;
    }
  }

  threadZeroReturn = static_cast<int *>(runThread(new size_t(0)));

  for (size_t t = 1; t < threadCount; ++t) {
    pthread_join(*pThreads[t - 1], nullptr);

#ifndef NO_ALLEGRO
    textprintf_ex(screen, font, 20, 500, makecol(0, 255, 0), makecol(0, 0, 0),
                  "Thread %5d", t - 1);
#endif
  }

  for (size_t t = 0; t < threadCount; ++t) {
    delete threads[t];

    if (t != 0) delete pThreads[t - 1];
  }

  delete[] threads;

  pThreads.clear();

  pthread_mutex_destroy(&ScheduleMutex);

  delete threadZeroReturn;

#ifndef NO_ALLEGRO

  explore_time = 0;
  while (!key[KEY_ESC]) {
  }
#endif
}

void *runThread(void *n) {
  size_t *t_id = static_cast<size_t *>(n);

  int *retVal = new int;

  while (algParams.size() > 0) {
    pthread_mutex_lock(&ScheduleMutex);

    AlgorithmToRun *alg = algParams.back();
    algParams.pop_back();

    pthread_mutex_unlock(&ScheduleMutex);

    *retVal = threads[*t_id]->runThread(alg);

#ifndef NO_ALLEGRO
    textprintf_ex(screen, font, 20, SCREEN_H - 30, color2, color, "%s", folder);
    threads[*t_id]->saveThread(folder);
#endif
  }

  delete t_id;

  return retVal;
}
