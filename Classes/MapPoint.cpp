//
//  MapPoint.cpp
//  guyue
//
//  Created by dsh on 15/1/8.
//
//

#include "MapPoint.h"
#define Size cocos2d::Size
#define Point cocos2d::Vec2
#define Rect cocos2d::Rect
static Size GRID_SIZE = Size(60, 32);

MapPoint::MapPoint()
:x(0)
,z(0)
{
    
}

MapPoint::MapPoint(M_INT x, M_INT z)
:x(x)
,z(z)
{
    
}

MapPoint::MapPoint(const Vec2& point)
:x((M_INT)point.x/GRID_SIZE.width)
,z((M_INT)point.y/GRID_SIZE.height)
{
    
}

MapPoint::MapPoint(int value)
:x(value / 65536)
,z(value % 65536)
{
    
}

MapPoint::~MapPoint()
{
    
}

int MapPoint::getValue() const
{
    int value = (int)(this->x * 65536 + this->z);
    return value;
}

Vec2 MapPoint::getCCPointValue() const
{
    Vec2 point = Vec2(x * GRID_SIZE.width, z * GRID_SIZE.height);
    point = ccpAdd(point, GRID_SIZE/2);
    
    return point;
}

Size MapPoint::getCCSizeValue() const
{
    Size size = Size(x * GRID_SIZE.width, z * GRID_SIZE.height);
    return size;
}

MapPoint& MapPoint::operator= (const MapPoint& other)
{
    this->x = other.x;
    this->z = other.z;
    return *this;
}

MapPoint MapPoint::operator+(const MapPoint& right) const
{
    return MapPoint(this->x + right.x, this->z + right.z);
}

MapPoint MapPoint::operator-(const MapPoint& right) const
{
    return MapPoint(this->x - right.x, this->z - right.z);
}

MapPoint MapPoint::operator-() const
{
    return MapPoint(-x, -z);
}

MapPoint MapPoint::operator*(M_INT a) const
{
    return MapPoint((M_INT)this->x * a, (M_INT)this->z * a);
}

MapPoint MapPoint::operator/(M_INT a) const
{
    CCAssert(a, "CCPoint division by 0.");
    return MapPoint((M_INT)this->x / a, (M_INT)this->z / a);
}

bool MapPoint::operator<(const MapPoint &other) const
{
    int a = this->x * 65536 + this->z;
    int b = other.x * 65536 + other.z;
    return (a < b);
}

bool MapPoint::operator==(const MapPoint &other) const
{
    int a = this->x * 65536 + this->z;
    int b = other.x * 65536 + other.z;
    return (a == b);
}

bool MapPoint::equals(const MapPoint &target) const
{
    return (this->x == target.x && this->z == target.z);
}

std::vector<MapPoint> MapPoint::getMapPointVectorForDistance(M_INT lenght) const
{
    std::vector<MapPoint> mapVec;
    M_INT x = -lenght, z = -lenght;
    while (1)
    {
        CC_BREAK_IF(mapVec.size() == 8*lenght);
        
        mapVec.push_back(mapAdd(*this, MapPoint(x, z)));
        
        if (mapVec.size() <= 2*lenght)
        {
            x++;
        }
        else if (2*lenght < mapVec.size() && mapVec.size() <= 4*lenght)
        {
            z++;
        }
        else if (4*lenght < mapVec.size() && mapVec.size() <= 6*lenght)
        {
            x--;
        }
        else if (6*lenght < mapVec.size() && mapVec.size() < 8*lenght)
        {
            z--;
        }
    }
    
    return mapVec;
}

void MapPoint::setGridSize(Size size)
{
    GRID_SIZE = size;
}
