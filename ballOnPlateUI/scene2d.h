#ifndef SCENE2D_H
#define SCENE2D_H

#include <QGraphicsScene>
#include <QtGui>
#include <QtCore>

class Scene2d:public QGraphicsScene
{
public:
    Scene2d(QObject * parent = 0);
    void draw();
    bool getBallOnPlate();
    int getPanelX();
    int getPanelY();
    void setBallOnPlate(bool onPlate);
    void setPanel(int x, int y);
    const qreal LEFT_BORDER = 0;
    const qreal RIGHT_BORDER = 480;
    const qreal TOP_BORDER = 0;
    const qreal BOTTOM_BORDER = 460;
    const qreal BALL_RADIUS = 10;

private:
    qreal collisionX(qreal x);
    qreal collisionY(qreal y);

    qreal coordinateToPositionX(int x);
    qreal coordinateToPositionY(int y);

    int panelX;
    int panelY;

    bool ballOnPlate = false;
};


#endif // SCENE2D_H
