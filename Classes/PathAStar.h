#pragma once
#include "cocos2d.h"
#include "stlastar.h" // See header for copyright and usage information

#include <iostream>
#include <math.h>
#include <deque>
#include "MapPoint.h"
#define DEBUG_LISTS 0
#define DEBUG_LIST_LENGTHS_ONLY 0

#define DISORDER 1

using namespace cocos2d;
using namespace std;

// Definitions

class MapSearchNode
{
public:
	unsigned int x;	 // the (x,y) positions of the node
	unsigned int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py ) { x=px; y=py; }

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );

	void PrintNodeInfo();
};

class AStarPoint
{
public:
    
    unsigned int x;
    
    unsigned int y;
    
    AStarPoint(unsigned int x, unsigned int y):x(x),y(y){;}
};

class PathAStar: public Ref 
{
private:
    
    unsigned char** m_nMap;
    
    unsigned int m_nRow;
    
    unsigned int m_nCol;
    
    int m_nMaxNodes;
    
public:
    
    void setData(const unsigned int row, const unsigned int col, unsigned char**map, int maxNodes);
    
	static int GetMap(M_INT x, M_INT y);
    
	static std::deque<MapPoint> findPathByAStar(unsigned char**map, const unsigned int row, const unsigned int col, int maxNodes, const MapPoint& begin, const MapPoint& end);

private:

	std::deque<AStarPoint> findPathByAStarInternal(const MapPoint& begin, const MapPoint& end);
    
    MapPoint getFilterEndPoint(const MapPoint& begin, const MapPoint& end);

};

