#include "PathAStar.h"
#include "GameScene.h"
static PathAStar* _roninAStar = NULL;

bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{

	// same state in a maze search is simply when (x,y) are the same
	if( (x == rhs.x) &&
		(y == rhs.y) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

void MapSearchNode::PrintNodeInfo()
{
	//cout << "Node position : (" << x << ", " << y << ")" << endl;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	float xd = fabs(float(((float)x - (float)nodeGoal.x)));
	float yd = fabs(float(((float)y - (float)nodeGoal.y)));

	return xd + yd;
}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

	if( (x == nodeGoal.x) &&
		(y == nodeGoal.y) )
	{
		return true;
	}

	return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
{

	int parent_x = -1; 
	int parent_y = -1; 

	if( parent_node )
	{
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}
	

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards

	if( (PathAStar::GetMap( x-1, y ) != DISORDER)
		&& !((parent_x == x-1) && (parent_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x-1, y );
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (PathAStar::GetMap( x-1, y-1 ) != DISORDER)
		&& !((parent_x == x-1) && (parent_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x-1, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}

	if( (PathAStar::GetMap( x, y-1 ) != DISORDER)
		&& !((parent_x == x) && (parent_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (PathAStar::GetMap( x+1, y-1 ) != DISORDER)
		&& !((parent_x == x+1) && (parent_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y-1 );
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (PathAStar::GetMap( x+1, y ) != DISORDER)
		&& !((parent_x == x+1) && (parent_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y );
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (PathAStar::GetMap( x+1, y+1 ) != DISORDER)
		&& !((parent_x == x+1) && (parent_y == y+1))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y+1 );
		astarsearch->AddSuccessor( NewNode );
	}
		
	if( (PathAStar::GetMap( x, y+1 ) != DISORDER)
		&& !((parent_x == x) && (parent_y == y+1))
		)
	{
		NewNode = MapSearchNode( x, y+1 );
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (PathAStar::GetMap( x-1, y+1 ) != DISORDER)
		&& !((parent_x == x-1) && (parent_y == y+1))
		)
	{
		NewNode = MapSearchNode( x-1, y+1 );
		astarsearch->AddSuccessor( NewNode );
	}
	return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	return (float) PathAStar::GetMap( x, y );
}

int PathAStar::GetMap(M_INT x, M_INT y)
{

	if(0 > x || x >= _roninAStar->m_nRow || 0 > y || y >= _roninAStar->m_nCol)
	{
		return 1;
	}
    
    if (_roninAStar->m_nMap[y][x] == 1)
    {
        return 1;
    }
    
    if (GAME_SCENE->getMapPoint(MapPoint(x * 65536 + y)))
    {
        return 1;
    }
    
	return 0;
}

void PathAStar::setData(const unsigned int row, const unsigned int col, unsigned char**map, int maxNodes)
{
    m_nMap = map;
    m_nRow = row;
    m_nCol = col;
    m_nMaxNodes = maxNodes;
}

std::deque<MapPoint> PathAStar::findPathByAStar(unsigned char**map, const unsigned int row, const unsigned int col, int maxNodes, const MapPoint& begin, const MapPoint& end)
{
    if (_roninAStar == NULL)
    {
        _roninAStar = new PathAStar();
    }
    _roninAStar->setData(row, col, map, maxNodes);
    
    std::deque<MapPoint> array;
	std::deque<AStarPoint> vec = _roninAStar->findPathByAStarInternal(begin, end);

    if (vec.size() <= 1)
        return array;

    std::deque<AStarPoint>::iterator itr = vec.begin();

    while (itr != vec.end())
    {
        MapPoint p = MapPoint(itr->x, itr->y);
        array.push_back(p);
        itr++;
    }

    return array;
}

std::deque<AStarPoint> PathAStar::findPathByAStarInternal(const MapPoint& begin, const MapPoint& end)
{
	std::deque<AStarPoint> vec;

    MapPoint filterEnd = this->getFilterEndPoint(begin, end);

	AStarSearch<MapSearchNode> astarsearch = AStarSearch<MapSearchNode>(m_nMaxNodes);

	unsigned int SearchCount = 0;

	const unsigned int NumSearches = 1;

	while(SearchCount < NumSearches)
	{
		
		// Create a start state
		MapSearchNode nodeStart;
		nodeStart.x = begin.x;
		nodeStart.y = begin.z;

		// Define the goal state
		MapSearchNode nodeEnd;
		nodeEnd.x = filterEnd.x;
		nodeEnd.y = filterEnd.z;
		
		// Set Start and goal states
		
		astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

		unsigned int SearchState;
		unsigned int SearchSteps = 0;

		do
		{
			SearchState = astarsearch.SearchStep();

			SearchSteps++;

		}
		while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );

		if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
		{

				MapSearchNode *node = astarsearch.GetSolutionStart();
				
				int steps = 0;

				node->PrintNodeInfo();

                while (1)
                {
					vec.push_back(AStarPoint(node->x, node->y));
					node = astarsearch.GetSolutionNext();
                    
                    CC_BREAK_IF(node == NULL);

					node->PrintNodeInfo();
					steps ++;
				
				};

				// Once you're done with the solution you can free the nodes up
				astarsearch.FreeSolutionNodes();

	
		}
		else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) 
		{
		
		}

		// Display the number of loops the search went through

		SearchCount ++;

		astarsearch.EnsureMemoryFreed();
	}
    
	return vec;
}

MapPoint PathAStar::getFilterEndPoint(const MapPoint& begin, const MapPoint &end)
{
    MapPoint relust = end;
    do
    {
        CC_BREAK_IF(GetMap(end.x, end.z) != DISORDER);
        
        M_INT count = begin.getDistance(end) + 10;
        for (M_INT i=1; i<=count; i++)
        {
            std::vector<MapPoint> mapVec = end.getMapPointVectorForDistance(i);

            int lenght = 0xffff;
            for (std::vector<MapPoint>::iterator itr=mapVec.begin(); itr!=mapVec.end(); itr++)
            {
                MapPoint p = *itr;
                if (GetMap(p.x, p.z) == DISORDER)
                    continue;
                
                if (p.getDistance(begin) >= lenght)
                    continue;
                
                if (p.equals(begin))
                    continue;

                relust = p;
                lenght = p.getDistance(begin);
            }
            CC_BREAK_IF(!relust.equals(end));
        }

    }
    while (0);

    return relust;
}