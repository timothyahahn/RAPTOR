// ____________________________________________________________________________
//
//  General Information:
//
//  File Name:      CQYShortestPath.h
//  Author:         Yan Qi
//  Project:        KShortestPath
//
//  Description:    Declaration of class CQYShortestPath, which implements 
//  Dijkstra algorithm for the shortest path in the directed graph.
//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Revision History:
//
//  11/23/2006   Yan   Initial Version
//  04/19/2019   Hahn  Modern OS/Compiler Changes

//
//  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  Copyright Notice:
//
//  Copyright (c) 2006 Your Company Inc.
//
//  Warning: This computer program is protected by copyright law and 
//  international treaties.  Unauthorized reproduction or distribution
//  of this program, or any portion of it, may result in severe civil and
//  criminal penalties, and will be prosecuted to the maximum extent 
//  possible under the law.
//
// ____________________________________________________________________________

#ifndef _QYSHORTETSPATH_H_
#define _QYSHORTETSPATH_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include "QYConfigCenter.h"
#include "QYDirectedGraph.h"
#include "QYDirectedPath.h"

class CQYShortestPath  
{
	typedef CQYConfigCenter::Int_Pair Edge_Type;
	typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS, boost::no_property, boost::property < boost::edge_weight_t, double > > Boost_Graph_Type;
	typedef boost::graph_traits < Boost_Graph_Type >::edge_descriptor Edge_Descriptor;
	typedef boost::graph_traits < Boost_Graph_Type >::vertex_descriptor Vertex_Descriptor;
		
public:
	CQYShortestPath(const CQYDirectedGraph& rGraph);
	virtual ~CQYShortestPath();

	CQYDirectedPath* GetShortestPath(int nSourceNodeId, int nTargetNodeId);
	void ConstructPathTree(int nSourceNodeId);

	double GetDistance(int i) { return m_distanceMap.count(i)? m_distanceMap[i] : CQYDirectedGraph::DISCONNECT; }
	void SetDistance(int i, double new_value){ m_distanceMap[i] = new_value; }

	int GetNextNodeId(int i) { return m_distanceMap.count(i)? m_nextNodeMap[i] : CQYDirectedGraph::DEADEND;}
	void SetNextNodeId(int i, int val) { m_nextNodeMap[i] = val; }

private: // methods
	void _Init();
	void _DijkstraShortestPathsAlg();
	CQYDirectedPath* _GetShortestPath(int nTargetNodeId);

private: // members
	std::vector<CQYConfigCenter::Int_Pair> m_vEdges;
	std::vector<double> m_vWeights;

	std::map<int, double> m_distanceMap;
	std::map<int, int> m_nextNodeMap;
		
	int m_nSourceNodeId;
	const CQYDirectedGraph& m_rGraph;
};

#endif //_QYSHORTETSPATH_H_
