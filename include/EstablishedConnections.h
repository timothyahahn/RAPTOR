// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      EstablishedConnection.h
//  Author:         Timothy Hahn, PhD
//  Project:        raptor
//
//  Description:    The file contains data types that are important for
//  determining
//					the performance of the various
//algorithms.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  06/02/2009	v1.02	Initial Version.
//  04/14/2019  v2.0    Reworked version based upon cmake and octave
//
// ____________________________________________________________________________

#ifndef ESTABLISHED_H
#define ESTABLISHED_H

#include "Edge.h"

struct EstablishedConnection {
  Edge **connectionPath;
  size_t connectionLength;
  long long int wavelength;
  double connectionStartTime;
  double connectionEndTime;
  double initQFactor;
  double belowQFactor;
  double averageQFactor;
  std::vector<double> *QFactors;
  std::vector<double> *QTimes;
};

#endif
