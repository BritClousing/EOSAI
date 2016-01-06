#ifndef GameMapImage_h
#define GameMapImage_h

#include "../EOSAI/SharedMemory.h"
#include <QtQuick>
#include <QImage>

class GameMapImage : public QQuickPaintedItem
{
    Q_OBJECT
    //Q_PROPERTY(bool rightAligned READ isRightAligned WRITE setRightAligned NOTIFY rightAlignedChanged)
    Q_PROPERTY(bool displayCities READ displayCities WRITE setDisplayCities NOTIFY displayCitiesChanged)
    Q_PROPERTY(bool displayResources READ displayResources WRITE setDisplayResources NOTIFY displayResourcesChanged)

    public:
        GameMapImage(QQuickItem *parent = 0);
        void paint(QPainter *painter);

        //bool isRightAligned();
        //void setRightAligned(bool rightAligned);
        bool displayCities(){ return m_bDisplayCities; }
        void setDisplayCities(bool display){ m_bDisplayCities = display; }
        bool displayResources(){ return m_bDisplayResources; }
        void setDisplayResources(bool display){ m_bDisplayResources = display; }

        void updateImage();

    private:
        bool rightAligned = false;

    signals:
        //void rightAlignedChanged();
        void displayCitiesChanged();\
        void displayResourcesChanged();\

    public:
        bool m_bDisplayCities;
        bool m_bDisplayResources;

        static int      s_iGameMapWidth;
        static int      s_iGameMapHeight;
        static RGBPixel s_GameMap[GAME_MAP_IMAGE_WIDTH][GAME_MAP_IMAGE_HEIGHT];
        static bool     s_bUpdateImage;
        static QImage*  s_pMapImage;
};

#endif
