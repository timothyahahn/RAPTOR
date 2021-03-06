// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      ConfigCenter.h
//  Author:         Yan Qi
//  Project:        K-ShortestPath
//
//  Description:    The file defines the class ConfigCenter, which defines
//	basic functions and handles parameters inputted by the user.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  11/21/2006   Yan   Initial Version
//  04/19/2019   Hahn  Modern OS/Compiler Changes
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Copyright Notice:
//
//  Copyright (c) 2006.
//
//  Warning: This computer program is protected by copyright law and
//  international treaties.  Unauthorized reproduction or distribution
//  of this program, or any portion of it, may result in severe civil and
//  criminal penalties, and will be prosecuted to the maximum extent
//  possible under the law.
//
// ____________________________________________________________________________

#ifndef _QYCONFIGCENTER_H_
#define _QYCONFIGCENTER_H_

#include <cstdio>
#include <map>

class ConfigCenter {
 public:
  typedef std::pair<size_t, size_t> SizeT_Pair;
  typedef SizeT_Pair Edge_Type;
  typedef std::map<SizeT_Pair, double> SizeT_Pair_Double_Map;
  typedef std::map<SizeT_Pair, double>::iterator SizeT_Pair_Double_Map_Iterator;
};

#endif  //_QYCONFIGCENTER_H_
