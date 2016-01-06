
#include "GameMapImage.h"
#include "GameData.h"
#include <QFontMetrics>

//QImage* m_pMapImage = NULL;
//bool bUpdateImage = true;
int      GameMapImage::s_iGameMapWidth = 1;
int      GameMapImage::s_iGameMapHeight = 1;
RGBPixel GameMapImage::s_GameMap[GAME_MAP_IMAGE_WIDTH][GAME_MAP_IMAGE_HEIGHT];
bool     GameMapImage::s_bUpdateImage;
QImage*  GameMapImage::s_pMapImage;

GameMapImage::GameMapImage(QQuickItem *parent)
    : QQuickPaintedItem(parent) //, rightAligned(false)
{
    m_bDisplayCities = true;
    m_bDisplayResources = true;
}

void GameMapImage::updateImage()
{
    if( s_pMapImage == NULL || s_bUpdateImage )
    {
        s_bUpdateImage = false;

        if( s_pMapImage == NULL)
        {
            s_pMapImage = new QImage(GAME_MAP_IMAGE_WIDTH,GAME_MAP_IMAGE_HEIGHT,QImage::Format_RGB888);
            s_pMapImage->fill(QColor(225,225,225));
        }

        // Update the image coloring
        QRgb color;
        for( int x=0; x<GAME_MAP_IMAGE_WIDTH; x++ )
        {
            for( int y=0; y<GAME_MAP_IMAGE_HEIGHT; y++ )
            {
                color = qRgb(s_GameMap[x][y].r, s_GameMap[x][y].g, s_GameMap[x][y].b);
                s_pMapImage->setPixel( x,y, color );
            }
        }
    }
}

void GameMapImage::paint(QPainter *painter)
{
    int iWindowHeight = painter->viewport().height();
    int iWindowWidth = painter->viewport().width();
    if( iWindowHeight <= 0 ) return;
    if( iWindowWidth <= 0 ) return;

    QBrush offWhiteBrush(QColor("#EEE"));
    painter->setBrush(offWhiteBrush);
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing);

    painter->drawRect( QRect(0,0,iWindowWidth,iWindowHeight) );
/*
    painter->drawRoundedRect(0, 0, boundingRect().width(), boundingRect().height() - 10, 10, 10);

    if (rightAligned)
    {
        const QPointF points[3] = {
            QPointF(boundingRect().width() - 10.0, boundingRect().height() - 10.0),
            QPointF(boundingRect().width() - 20.0, boundingRect().height()),
            QPointF(boundingRect().width() - 30.0, boundingRect().height() - 10.0),
        };
        painter->drawConvexPolygon(points, 3);
    }
    else
    {
        const QPointF points[3] = {
            QPointF(10.0, boundingRect().height() - 10.0),
            QPointF(20.0, boundingRect().height()),
            QPointF(30.0, boundingRect().height() - 10.0),
        };
        painter->drawConvexPolygon(points, 3);
    }
*/
    updateImage();
/*
    //QPixmap pixmap(100,100);
    //pixmap.fill(QColor(250,0,0));
    //
    //QImage imagemap(100,100,QImage::Format_RGB32);
    //QImage imagemap(100,100,QImage::Format_RGB888);
    QImage imagemap(100,100,QImage::Format_RGB888);
    imagemap.fill(QColor(225,225,225));

    QRgb value;
    //value = qRgb(189, 149, 39); // 0xffbd9527
    value = qRgb(100, 100, 30);
    imagemap.setPixel( 1, 1, value);
    imagemap.setPixel(25,25, value);
    imagemap.setPixel(25,26, value);
    imagemap.setPixel(26,26, value);
*/
    //float fRatio = (float)s_iGameMapWidth / (float)s_iGameMapHeight;
    float fGameMapRatio = (float)s_iGameMapWidth / (float)s_iGameMapHeight;
    if( fGameMapRatio <= 0.0f ) return;

    int iGameMapDisplayWidth = fGameMapRatio * iWindowHeight;
    int iGameMapDisplayHeight = iWindowHeight;
    if( iGameMapDisplayWidth > iWindowWidth )
    {
        iGameMapDisplayWidth = iWindowWidth;
        iGameMapDisplayHeight = iWindowWidth / fGameMapRatio;
    }

    int iPositionOffsetX = (iWindowWidth - iGameMapDisplayWidth)/2;
    int iPositionOffsetY = (iWindowHeight - iGameMapDisplayHeight)/2;


    //painter->drawImage(0,0,imagemap);
    //painter->drawImage(0,0,imagemap, 0,0, 50,50);
    //painter->drawImage( QRect(10,0,200,200), imagemap );
    //painter->drawImage( QRect(iPositionX,iPositionY,iGameMapWidth,iGameMapHeight), imagemap );
    painter->drawImage( QRect(iPositionOffsetX,iPositionOffsetY,iGameMapDisplayWidth,iGameMapDisplayHeight), *s_pMapImage );

    QPen whitePen(QColor("#FFF"));
    whitePen.setWidthF(4.0);
    QPen redPen(QColor("#F44"));
    redPen.setWidthF(4.0);
    QPen bluePen(QColor("#44C"));
    bluePen.setWidthF(4.0);
    QPen blackPen(QColor("#00"));
    blackPen.setWidthF(4.0);
    QBrush blackBrush(QColor("#000"));

    // Display Poi
    if( g_pGameData )
    {
        painter->setBrush(Qt::NoBrush);

        for( int i = 0; i < g_pGameData->m_iNumberOfPoi; i++ )
        {
            //SharedMemory_Poi poi = *iter;
            SharedMemory_Poi poi = g_pGameData->m_PoiList[i];

            if( poi.m_iOwner == 0 ) painter->setPen(whitePen);
            if( poi.m_iOwner == 1 ) painter->setPen(redPen);
            if( poi.m_iOwner == 2 ) painter->setPen(bluePen);

            int iCenterX = iPositionOffsetX + (poi.m_fPosX/s_iGameMapWidth)*iGameMapDisplayWidth;
            int iCenterY = iPositionOffsetY + (poi.m_fPosY/s_iGameMapHeight)*iGameMapDisplayHeight;

            // Draw the circles, squares, Xs
            if( strcmp( poi.m_szPoiType.m_szText, "City" ) == 0 )
            {
                if( m_bDisplayCities )
                {
                    float fSize = 0.04f * iGameMapDisplayWidth;
                    painter->drawEllipse( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );

                    /*
                    float fTextRectHeight = 10;
                    QString strText = "10 Prod";
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(blackBrush);
                    painter->drawRect( QRectF(iCenterX,iCenterY-fTextRectHeight,5*strText.length()+4,fTextRectHeight+1) );
                    painter->setPen(whitePen);
                    painter->setBrush(Qt::NoBrush);
                    painter->drawText( iCenterX, iCenterY, strText );
                    */
                }
            }
            else if( strcmp( poi.m_szPoiType.m_szText, "Resource" ) == 0 )
            {
                if( m_bDisplayResources )
                {
                    float fSize = 0.02f * iGameMapDisplayWidth;
                    //painter->drawRect( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );
                    painter->drawEllipse( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );

                    /*
                    float fTextRectHeight = 10; //0.04f * iGameMapDisplayWidth;
                    QString strText = "10 Oil";
                    painter->setPen(Qt::NoPen);
                    painter->setBrush(blackBrush);
                    painter->drawRect( QRectF(iCenterX,iCenterY-fTextRectHeight,5*strText.length()+4,fTextRectHeight+1) );
                    painter->setPen(whitePen);
                    painter->setBrush(Qt::NoBrush);
                    painter->drawText( iCenterX, iCenterY, strText );
                    */
                }
            }
            else if( strcmp( poi.m_szPoiType.m_szText, "Unit" ) == 0 )
            {
                float fSize = 0.02f * iGameMapDisplayWidth;
                painter->drawLine( iCenterX-fSize/2.0f,iCenterY-fSize/2.0f, iCenterX+fSize/2.0f,iCenterY+fSize/2.0f );
                painter->drawLine( iCenterX+fSize/2.0f,iCenterY-fSize/2.0f, iCenterX-fSize/2.0f,iCenterY+fSize/2.0f );
                //painter->drawRect( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );
            }
            else
            {
                float fSize = 0.02f * iGameMapDisplayWidth;
                painter->drawRect( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );
            }

            //float fCircleSize = 0.04f * iGameMapDisplayWidth;
            //painter->drawRoundedRect( QRect(iCenterX-5,iCenterY-5,20,20),0.0,0.0 );
            //painter->drawEllipse( QRectF(iCenterX-fCircleSize/2.0f,iCenterY-fCircleSize/2.0f,fCircleSize,fCircleSize) );
        }
        for( int i = 0; i < g_pGameData->m_iNumberOfPoi; i++ )
        {
            //SharedMemory_Poi poi = *iter;
            SharedMemory_Poi poi = g_pGameData->m_PoiList[i];
            if( strcmp( poi.m_szPoiType.m_szText, "City" ) == 0 && m_bDisplayCities == false ) continue;
            if( strcmp( poi.m_szPoiType.m_szText, "Resource" ) == 0 && m_bDisplayResources == false ) continue;

            if( poi.m_iOwner == 0 ) painter->setPen(whitePen);
            if( poi.m_iOwner == 1 ) painter->setPen(redPen);
            if( poi.m_iOwner == 2 ) painter->setPen(bluePen);

            int iCenterX = iPositionOffsetX + (poi.m_fPosX/s_iGameMapWidth)*iGameMapDisplayWidth;
            int iCenterY = iPositionOffsetY + (poi.m_fPosY/s_iGameMapHeight)*iGameMapDisplayHeight;

            QString strText = poi.m_szMapDesc.m_szText; //"10 Prod";

            if( strText != "" )
            {
                //QFont font("times", 24);
                //QFont font("times", 24);
                //QFontMetrics fm(font);
                QFontMetrics fm(painter->font());
                int pixelsWide = fm.width(strText);//"What's the width of this text?");
                int pixelsHigh = fm.height();

                //float fTextRectHeight = 10;
                painter->setPen(Qt::NoPen);
                painter->setBrush(blackBrush);
                //painter->drawRect( QRectF(iCenterX,iCenterY-fTextRectHeight,5*strText.length()+4,fTextRectHeight+1) );
                //painter->drawRect( QRectF(iCenterX-1,iCenterY-fTextRectHeight,pixelsWide+1,fTextRectHeight+1) );
                painter->drawRect( QRectF(iCenterX-1,iCenterY-pixelsHigh+2,pixelsWide+1,pixelsHigh) );
                painter->setPen(whitePen);
                painter->setBrush(Qt::NoBrush);
                painter->drawText( iCenterX, iCenterY, strText );
            }

            /*
            // Draw the circles, squares, Xs
            if( strcmp( poi.m_szPoiType, "City" ) == 0 )
            {
                //float fSize = 0.04f * iGameMapDisplayWidth;
                //painter->drawEllipse( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );

                float fTextRectHeight = 10;
                QString strText = poi.m_szDesc; //"10 Prod";
                painter->setPen(Qt::NoPen);
                painter->setBrush(blackBrush);
                painter->drawRect( QRectF(iCenterX,iCenterY-fTextRectHeight,5*strText.length()+4,fTextRectHeight+1) );
                painter->setPen(whitePen);
                painter->setBrush(Qt::NoBrush);
                painter->drawText( iCenterX, iCenterY, strText );
            }
            else if( strcmp( poi.m_szPoiType, "Resource" ) == 0 )
            {
                //float fSize = 0.02f * iGameMapDisplayWidth;
                //painter->drawRect( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );

                float fTextRectHeight = 10; //0.04f * iGameMapDisplayWidth;
                QString strText = poi.m_szDesc; //"10 Oil";
                painter->setPen(Qt::NoPen);
                painter->setBrush(blackBrush);
                painter->drawRect( QRectF(iCenterX,iCenterY-fTextRectHeight,5*strText.length()+4,fTextRectHeight+1) );
                painter->setPen(whitePen);
                painter->setBrush(Qt::NoBrush);
                painter->drawText( iCenterX, iCenterY, strText );
            }
            else if( strcmp( poi.m_szPoiType, "Unit" ) == 0 )
            {
                //float fSize = 0.02f * iGameMapDisplayWidth;
                //painter->drawLine( iCenterX-fSize/2.0f,iCenterY-fSize/2.0f, iCenterX+fSize/2.0f,iCenterY+fSize/2.0f );
                //painter->drawLine( iCenterX+fSize/2.0f,iCenterY-fSize/2.0f, iCenterX-fSize/2.0f,iCenterY+fSize/2.0f );
            }
            else
            {
                //float fSize = 0.02f * iGameMapDisplayWidth;
                //painter->drawRect( QRectF(iCenterX-fSize/2.0f,iCenterY-fSize/2.0f,fSize,fSize) );
            }
            */

            //float fCircleSize = 0.04f * iGameMapDisplayWidth;
            //painter->drawRoundedRect( QRect(iCenterX-5,iCenterY-5,20,20),0.0,0.0 );
            //painter->drawEllipse( QRectF(iCenterX-fCircleSize/2.0f,iCenterY-fCircleSize/2.0f,fCircleSize,fCircleSize) );
        }
    }
}
/*
bool GameMapImage::isRightAligned()
{
    return this->rightAligned;
}

void GameMapImage::setRightAligned(bool rightAligned)
{
    this->rightAligned = rightAligned;
}
*/
