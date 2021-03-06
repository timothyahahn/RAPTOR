// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      Router.h
//  Author:         Timothy Hahn, PhD
//  Project:        raptor
//
//  Description:    The file contains the declaration of the Router class.
//					The purpose of the Router is to simulate the
//optical 					routers in the network, passing messages on their way 					to the
//destination and handling them when they get to 					the final destination.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  05/20/2009	v1.0	Initial Version.
//  06/02/2009	v1.02	Minor optimizations and bug fixes.
//  04/14/2019  v2.0    Reworked version based upon cmake and octave
//
// ____________________________________________________________________________

#ifndef ROUTER_H
#define ROUTER_H

#include "Edge.h"

#ifndef NO_ALLEGRO

#include "allegro5/allegro.h"

extern int realTopoWidthPx;
extern int realTopoHeightPx;
#endif

#include <vector>

struct DP_node {
  Edge** paths;
  bool* waveAvailability;
  size_t* pathLength;
  size_t* pathSpans;
  double* optimalWave;
  double* pathQuality;
  double* pathWeight;
};

class Router {
 public:
  Router();
  ~Router();

  inline void setIndex(size_t i) { routerIndex = i; }
  inline size_t getIndex() const { return routerIndex; }

  void generateProbabilities();

  size_t generateDestination(double p);

#ifndef NO_ALLEGRO
  inline void incNumWorkstations() { ++numWorkstations; };
  inline int getNumWorkstations() { return numWorkstations; };
  void setRadius(double pct);
  inline void addTopoEdge(Edge* e) { edgeList.push_back(e); };
  void scaleEdgesTo(int spns, int px);
  inline Edge* getTopoEdgeByDest(size_t e) {
    for (size_t x = 0; x < edgeList.size(); x++) {
      if (edgeList[x]->getDestinationIndex() == e) {
        return edgeList[x];
      }
    }
    return 0;
  };
#endif

  void addEdge(Edge* e);

  inline long long int isAdjacentTo(size_t r) const { return adjacencyList[r]; }

  inline Edge* getEdgeByIndex(size_t e) const { return edgeList[e]; }

  Edge* getEdgeByDestination(size_t r) const;

  void updateUsage();
  void resetUsage();

  void resetQMDegredation();
  void resetFailures();

  inline size_t getQualityFailures() const { return qualityFailures; }
  inline size_t getWaveFailures() const { return waveFailures; }

  inline void incrementQualityFailures() { ++qualityFailures; }
  inline void incrementWaveFailures() { ++waveFailures; }

  inline size_t getNumberOfEdges() const { return edgeList.size(); }

  void generateACOProbabilities(size_t dest);

  Edge* chooseEdge(double p) const;

#ifndef NO_ALLEGRO
  void refreshedgebmps(bool useThread);
  inline int getXPercent() { return xpercent; };
  inline int getYPercent() { return ypercent; };
  inline int getXPixel() { return xPixel; }
  inline int getYPixel() { return yPixel; }
  inline int getRadius() { return radius; };
  inline void setSelected() {
    if (isSelected)
      isSelected = false;
    else
      isSelected = true;
  }
  void setNumWorkstations(int w);
  void paintNumWKs();
  inline void setXPercent(int x) { xpercent = x; };
  inline void setYPercent(int y) { ypercent = y; };
  inline void setXYPixels() {
    xPixel = (((double)xpercent / 100.0) * realTopoWidthPx);
    yPixel = (((double)ypercent / 100.0) * realTopoHeightPx);
  }
  inline void moveXYPixels(int x, int y) {
    xPixel += x;
    yPixel += y;
  }
  void paintNumDests(int n);
  void paintEdgeSpans();
  void paintUsage(int p);
  void updateGUI();
  void paintProgress(int x, int y, int h, double PaintTime, double TimePerPx,
                     int r, int g, int b);
  void saveData(char* file);

  inline void incConnAttemptsFrom() { connAttemptsFromThis++; };
  inline void setConnAttemptsFrom(size_t a) { connAttemptsFromThis = a; };
  inline size_t getConnAttemptsFrom() { return connAttemptsFromThis; };
  inline void incConnAttemptsTo() { connAttemptsToThis++; };
  inline void setConnAttemptsTo(size_t a) { connAttemptsToThis = a; };
  inline size_t getConnAttemptsTo() { return connAttemptsToThis; };
  inline void incConnSuccessesFrom() { connSuccessesFromThis++; };
  inline void setConnSuccessesFrom(size_t a) {
    connSuccessesFromThis = a;
  };
  inline size_t getConnSuccessesFrom() { return connSuccessesFromThis; };
  inline void incConnSuccessesTo() { connSuccessesToThis++; };
  inline void setConnSuccessesTo(size_t a) { connSuccessesToThis = a; };
  inline size_t getConnSuccessesTo() { return connSuccessesToThis; };
  inline void addToAvgQTo(double q) { avgQTo += q; };
  inline void addToAvgQFrom(double q) { avgQFrom += q; };

  inline const char* getName() { return name; };
  inline void setName(const char* nm) { sprintf(name, nm); };

  void selectScreen();

#endif
  DP_node* dp_node;

 private:
  size_t routerIndex;

  size_t qualityFailures;
  size_t waveFailures;

  long long int* adjacencyList;

#ifndef NO_ALLEGRO
  size_t connAttemptsFromThis;
  size_t connAttemptsToThis;
  size_t connSuccessesFromThis;
  size_t connSuccessesToThis;

  double avgQTo;
  double avgQFrom;

  ALLEGRO_BITMAP* routerpic;
  char name[20];
  int radius;
  int rcolor;
  int numWorkstations;
  int xpercent;
  int ypercent;
  int xPixel;
  int yPixel;
  bool isSelected;
#endif

  std::vector<Edge*> edgeList;

  double* destinationProbs;
  double* acoProbs;
};

#endif
