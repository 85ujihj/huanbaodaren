

#ifndef __project__RoninAStar__
#define __project__RoninAStar__

#include <iostream>
#include "cocos2d.h"
#include "MapPoint.h"
#include <deque>

USING_NS_CC;
class MapPointAStar : public Ref
{
public:
    MapPoint position;
    int gScore;
    int hScore;
    
    MapPointAStar * parent;
    
    MapPointAStar();
    virtual ~MapPointAStar();
    void init(const MapPoint & pos, const MapPoint & target, int _G_score);
    static MapPointAStar * create(int _G_score, const MapPoint & pos, const MapPoint & target);
    
    int getScore() const;
    bool isEqualPoint(const MapPoint & point);
    virtual bool isEqual(const Ref* pObject);
    
    void print();
};


class AStarListener
{
public:
    virtual bool isPointValid(const MapPoint& point) = 0;
};


class RoninAStar: public Ref
{
    
    static int compareMapPointAStar(const Ref * p1, const Ref * p2);
    static void sortMapPointAStarArray(CCArray * array);
    
    static int compareAStarPtByHScore(const Ref * p1, const Ref * p2);
    static void sortMapPointAStarByHScore(CCArray * array);
    
    static MapPointAStar* findPointInArray(const CCArray * array, const MapPointAStar * point);
    
    MapPointAStar* findPathByAStarInternal(const MapPoint& startPoint, const MapPoint& target, AStarListener * listener);
    
public:
    
    static std::deque<MapPoint> findPathByAStar(const MapPoint& startPoint, const MapPoint& target, AStarListener * listener);
};

#endif /* defined(__project__RoninAStar__) */
