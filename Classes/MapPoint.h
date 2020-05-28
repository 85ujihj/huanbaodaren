
//  MapPoint.h
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#ifndef __guyue__MapPoint__
#define __guyue__MapPoint__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

typedef short M_INT;

using namespace std;
class MapPoint {
    
public:
    
    M_INT x;
    
    M_INT z;
    
public:
    
    MapPoint();
    
    MapPoint(M_INT x, M_INT z);
    
    virtual ~MapPoint();
    
    MapPoint(const cocos2d::Vec2& point);
    
    cocos2d::Vec2 getCCPointValue() const;
    
    cocos2d::Size getCCSizeValue() const;
    
    MapPoint(int value);
    
    int getValue() const;
    
    MapPoint& operator= (const MapPoint& other);
    
    MapPoint operator+(const MapPoint& right) const;
    
    MapPoint operator-(const MapPoint& right) const;
    
    MapPoint operator-() const;
    
    MapPoint operator*(M_INT a) const;
    
    MapPoint operator/(M_INT a) const;
    
    bool operator< (const MapPoint& other) const;
    
    bool operator== (const MapPoint& other) const;
    
    bool equals(const MapPoint& target) const;
    
    inline M_INT getLength() const
    {
        return (M_INT)MAX(fabsf(x), fabsf(z));
    }
    
    inline M_INT getDistance(const MapPoint& other) const
    {
        return (M_INT)MAX(fabsf(other.x - this->x), fabsf(other.z - this->z));
    }
    
    std::vector<MapPoint> getMapPointVectorForDistance(M_INT lenght) const;
    
public:
    
    static void setGridSize(cocos2d::Size size);
    
};

const MapPoint MapPointZero = MapPoint();

#define mapAdd(_A_, _B_) MapPoint(((_A_).x + (_B_).x), ((_A_).z + (_B_).z))
#define mapSub(_A_, _B_) MapPoint(((_A_).x - (_B_).x), ((_A_).z - (_B_).z))


#endif /* defined(__guyue__MapPoint__) */
