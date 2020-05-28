

#ifndef __mir_mobile__BgMap__
#define __mir_mobile__BgMap__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "BgMapFloorTile.h"
#include "NpcFigure.h"
#include "MapPoint.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace rapidjson;
#define WINSIZE Director::getInstance()->getWinSize()

struct PortalInformation
{
    int key;
    
    int mapID;
    
    int born;
    
    cocos2d::Vec2 point;
    
    bool equals(const PortalInformation& other)
    {
        return  (key == other.key && mapID == other.mapID && born == other.born && point.equals(other.point));
    }
};

const PortalInformation PortalInformationZero = {0, 0, 0, cocos2d::Vec2::ZERO};

struct NpcInformation
{
    int key;
    
    NPCDirectionType directionType;
    
    cocos2d::Vec2 point;
    
    bool equals(const NpcInformation& other)
    {
        return  (key == other.key && (int)directionType == (int)other.directionType && point.equals(other.point));
    }
};

class BgMapDelegate
{
    
public:
    
    virtual void updateImageDisplay(cocos2d::Rect& showRect, cocos2d::Rect& hideRect) = 0;
};

class BgMap: public Node {
    
    CC_SYNTHESIZE_READONLY(int, m_mapID, MapID);
    
    CC_SYNTHESIZE_READONLY(unsigned char**, m_grid, MapGrid);//地图
    
    CC_SYNTHESIZE_READONLY(M_INT, m_gridRow, GridRow);//每行第几个

    CC_SYNTHESIZE_READONLY(M_INT, m_gridCol, GridCol);//行
    
    CC_SYNTHESIZE_READONLY(cocos2d::Size, m_gridSize, GridSize);
    
    CC_SYNTHESIZE_READONLY(cocos2d::Size, m_bgSize, BgSize);
    
    CC_SYNTHESIZE(BgMapDelegate*, m_delegate, Delegate);
    
    std::vector<BgMapFloorTile*> m_floorTileVec;

    std::vector<NpcFigure*> m_npcFigureVec;
    
    unsigned int m_imageRow;
    
    unsigned int m_imageCol;
    
    cocos2d::Size m_imageSize;
    
    std::vector<struct PortalInformation> m_mapPassageway;
    
    std::map<int, cocos2d::Vec2> m_mapBornPoint;
    
    std::vector<struct NpcInformation> m_npcInfoVec;
    
    std::multimap<unsigned int, MapPoint> m_enemyMap;
    
    std::string m_backGroundMusic;
    
    Node* m_playerLead;
    
    CC_SYNTHESIZE_READONLY(Sprite*, m_sMap, SmallMap);
    
private:
    
    void initGridData(rapidjson::Value& arr);
    
    void initBgMapFloorTile();

    void initBgMapPassagewayImage();
    
    void initNpcFigure();
    
    void updateImageDisplay();
    
    void readGirdData();
    
    void readGoodsData();
    
public:
    
    void updateMapPosition(float delay = 0);

    unsigned char getCurrentGridValue(const MapPoint& point);
    
    PortalInformation isMapPassageway(Node* node);
    
    std::multimap<unsigned int, MapPoint>& getEnemeyMap();
    
    //inline CCRect getShowRect();
    //inline CCRect getHideRect();
     cocos2d::Rect getShowRect();
    
     cocos2d::Rect getHideRect();
    
    int getMapID();
    
    
public:
    
    BgMap(int mapID);
    
    virtual ~BgMap();
    
    static BgMap* createWithPath(int mapID);
    
    bool init();
    
    void addChildPlayerLead(Node* node, int bornPoint);
    
    void addChildPlayerLead(Node* node, cocos2d::Vec2 point);
    
    static const float getZOrder(cocos2d::Vec2 point)
    {
        return (-point.y / 10.0f);
    }
    
    static const float getZOrderZero(BgMap* bgMap)
    {
        return (-bgMap->getBgSize().height / 10.0f);
    }
};


#endif /* defined(__mir_mobile__BgMap__) */
