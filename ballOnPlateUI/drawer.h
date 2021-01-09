#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsScene>
#include <QList>
#include <QtGui>
#include <QtCore>

class Drawer:public QGraphicsScene
{
public:
    Drawer(QObject * parent = 0);
    virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    virtual void mouseMoveEvent( QGraphicsSceneMouseEvent * mouseEvent );
    void draw();
    bool getFinishPath();
    bool getBallOnPlate();
    bool isPathReady();//returns true only once
    int getPanelX();
    int getPanelY();
    int getMouseX();
    int getMouseY();
    void setMouseX(int x);
    void setMouseY(int y);
    void setBallOnPlate(bool onPlate);
    void setPanel(int x, int y);
    QList<QPoint> getPathList();

    const qreal LEFT_BORDER = 0;
    const qreal RIGHT_BORDER = 440;
    const qreal TOP_BORDER = 0;
    const qreal BOTTOM_BORDER = 320;
    const qreal POINT_RADIUS = 10;
    const qreal MAX_RADIUS = 20; //MAX_RADIUS >= 2*POINT_RADIUS
private:
    qreal collisionX(qreal x);
    qreal collisionY(qreal y);
    QPointF collision(QPointF &point);
    QPointF collision(qreal x, qreal y);

    qreal coordinateToPositionX(int x);
    qreal coordinateToPositionY(int y);

    int positionToCoordinateX(qreal x);
    int positionToCoordinateY(qreal y);
    QPoint positionToCoordinate(const QPointF &point);

    int panelX;
    int panelY;

    int mouseX = 0;
    int mouseY = 0;

    bool pathReady = false;
    bool ballOnPlate = false;
    bool closeToStartPoint = false;
    bool finishPath = false;
    QList<QPointF> path;

};

#endif // DRAWER_H
