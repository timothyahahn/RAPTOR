// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      RaptorTime.h
//  Author:         Timothy Hahn, PhD
//  Project:        raptor
//
//  Description:    The file contains the declaration of some data types
//					required by the EventQueue.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  05/20/2009	v1.0	Initial Version.
//  04/14/2019  v2.0    Reworked version based upon cmake and octave
//
// ____________________________________________________________________________

#ifndef RAPTOR_TIME_H
#define RAPTOR_TIME_H

const double TEN_HOURS = /*10.0 **/ 60.0 * 60.0;
const double HUNDRED_HOURS = 10.0 * TEN_HOURS;

const double SPEED_OF_LIGHT = double(299792458);

struct FormattedTime {
	unsigned int hours;
	unsigned int minutes;
	float seconds;
};

#endif
