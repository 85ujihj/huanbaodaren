

#include "RoninAStar.h"
#include <algorithm>

#define MAAX(a, b) ( ((a) >= (b)) ? (a) : (b) )
#define MIIN(a, b) ( ((a) <= (b)) ? (a) : (b) )
#define CLAAMP(value, min, max) MAAX(min, MIIN(value, max))

#define CAST_INSTANCE(ptr, clazz)  ( ((ptr) != NULL) ? dynamic_cast<clazz *>((ptr)) : NULL )
#define INSTANCEOF(ptr, clazz) ( CAST_INSTANCE(ptr, clazz) != NULL )

#define RONIN_CCARRAY_FOR_EACH(obj, array, clazz) \
if ( clazz * obj = (clazz *)1 ) \
if ( CCObject * __object__ = (CCObject *)1 ) \
if ( (array) && (array)->data->num > 0 ) \
for( CCObject** arr = (array)->data->arr, **end = (array)->data->arr + (array)->data->num-1; \
arr <= end && ((__object__ = *arr) != NULL) && ((obj = CAST_INSTANCE(__object__, clazz)) || true); \
arr++)

#define WIDTH(pSprite)  (pSprite->getContentSize().width)
#define HEIGHT(pSprite) (pSprite->getContentSize().height)

#define GET_RECT(pNode) \
( CCRectMake( pNode->getPositionX() - pNode->getAnchorPointInPoints().x, \
pNode->getPositionY() - pNode->getAnchorPointInPoints().y, \
WIDTH(pNode), HEIGHT(pNode) ) )

#define GET_EXTENT_RECT(rect, extendWidth, extendHeight) \
CCRectMake( (rect).origin.x - (extendWidth), (rect).origin.y - (extendHeight),\
(rect).size.width + 2 * (extendWidth), (rect).size.height + 2 * (extendHeight) )

#define CENTRAL_POINT_WORLD(pSprite) pSprite->convertToWorldSpace(ccp(WIDTH(pSprite)/2, HEIGHT(pSprite)/2));


#define GIRD_SIZE CCSize(30, 18)

MapPointAStar::MapPointAStar()
:position(MapPointZero)
,gScore(0)
,hScore(0)
,parent(NULL)
{
    
}

MapPointAStar::~MapPointAStar()
{
    parent = NULL;
}

void MapPointAStar::init(const MapPoint & pos, const MapPoint & target, int _G_score)
{
    position = pos;
    hScore = 10 * fabs(target.x - position.x) + 10 * fabs(target.z - position.z);
    gScore = _G_score;
}

MapPointAStar* MapPointAStar::create(int _G_score, const MapPoint & pos, const MapPoint & target)
{
    MapPointAStar * ret = new MapPointAStar();
    ret->init(pos, target, _G_score);
    ret->autorelease();
    return ret;
}

int MapPointAStar::getScore() const
{
    return gScore + hScore;
}

bool MapPointAStar::isEqualPoint(const MapPoint & point)
{
    return position.equals(point);
}

bool MapPointAStar::isEqual(const Ref* pObject)
{
//    if (CCObject::isEqual(pObject) )
//        return true;
    if (this == pObject) {
        return true;
    }
    
    const MapPointAStar * other = CAST_INSTANCE(pObject, const MapPointAStar);
    return ( other != NULL && position.equals(other->position));
}

void MapPointAStar::print()
{
   // CCLog("start Point %d, %d, score %d", position.x, position.z, getScore());
}

int RoninAStar::compareMapPointAStar(const Ref * p1, const Ref * p2)
{
    const MapPointAStar * ap1 = (const MapPointAStar *)p1;
    const MapPointAStar * ap2 = (const MapPointAStar *)p2;
    return (ap1->getScore() == ap2->getScore()) ? ap1->hScore > ap2->hScore : ap1->getScore() > ap2->getScore();
}

void RoninAStar::sortMapPointAStarArray(CCArray * array)
{
    if ( array == NULL || array->count() <= 1 )
        return;
    
    std::sort(array->data->arr, array->data->arr + array->data->num, compareMapPointAStar);
}

int RoninAStar::compareAStarPtByHScore(const Ref * p1, const Ref * p2) {
    const MapPointAStar * ap1 = (const MapPointAStar *)p1;
    const MapPointAStar * ap2 = (const MapPointAStar *)p2;
    return (ap1->hScore == ap2->hScore) ? ap1->getScore() > ap2->getScore() : ap1->hScore > ap2->hScore;
}

void RoninAStar::sortMapPointAStarByHScore(CCArray * array)
{
    if ( array == NULL || array->count() <= 1 )
        return;
    
    std::sort(array->data->arr, array->data->arr + array->data->num, compareAStarPtByHScore);
}

MapPointAStar* RoninAStar::findPointInArray(const CCArray * array, const MapPointAStar * point)
{
    Ref * obj;
    CCARRAY_FOREACH(array, obj)
    {
       // if ( obj->isEqual(point) )
            if (obj==point)
            return CAST_INSTANCE(obj, MapPointAStar);
    }
    return NULL;
}

MapPointAStar* RoninAStar::findPathByAStarInternal(const MapPoint& startPoint, const MapPoint& target, AStarListener* listener)
{
    CCArray* openList = CCArray::create(MapPointAStar::create(0, startPoint, target), NULL);
    CCArray* closedList = CCArray::create();
    
    while ( openList->count() > 0 )
    {
        sortMapPointAStarArray(openList);
        MapPointAStar * s = (MapPointAStar *)openList->lastObject();
        openList->removeLastObject();
        
        if ( s->isEqualPoint(target) )
        {
            return s;
        }
        
        closedList->addObject(s);
        
        std::vector<MapPointAStar*> vec;
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(-1, 1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(0, 1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(1, 1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(1, 0)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(1, -1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(0, -1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(-1, -1)), target));
        vec.push_back(MapPointAStar::create(s->gScore + 10, mapAdd(s->position, MapPoint(-1, 0)), target));
        
        
        MapPointAStar* obj;
        for (std::vector<MapPointAStar*>::iterator itr=vec.begin(); itr!=vec.end(); itr++)
        {
            obj = *itr;
            if (obj->position.getDistance(startPoint) > 10)
                continue;
            
            if ( !listener->isPointValid(obj->position) )
                continue;
            
            MapPointAStar * pointInClosed = findPointInArray(closedList, obj);
            if ( pointInClosed != NULL )
                continue;
            
            MapPointAStar * pointInOpen = findPointInArray(openList, obj);
            if ( pointInOpen == NULL )
            {
                obj->parent = s;
                openList->addObject(obj);
            }
            else if ( pointInOpen->getScore() > obj->getScore() )
            {
                pointInOpen->parent = s;
                pointInOpen->gScore = obj->gScore;
            }
        }
    }
    
    sortMapPointAStarByHScore(closedList);
    
    return CAST_INSTANCE(closedList->lastObject(), MapPointAStar);
}

std::deque<MapPoint> RoninAStar::findPathByAStar(const MapPoint& startPoint, const MapPoint& target, AStarListener* listener)
{
    RoninAStar* _roninAStar = new RoninAStar();
    
    MapPointAStar * result = _roninAStar->findPathByAStarInternal(startPoint, target, listener);
    
    std::deque<MapPoint> deuqeMap;
    
    MapPointAStar * point = result;
    while (point != NULL)
    {
        deuqeMap.push_front(point->position);
        point = point->parent;
    }
    
    _roninAStar->autorelease();
    
    return deuqeMap;
}

