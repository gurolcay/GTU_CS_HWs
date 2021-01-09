#include "scene2d.h"
#include <QGraphicsSceneMouseEvent>

Scene2d::Scene2d(QObject * parent):QGraphicsScene(parent)
{
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);
}

void Scene2d::draw(){
    this->clear();
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);

    if(getBallOnPlate()){
        QPen ballPen(Qt::blue);
        QBrush ballBrush(Qt::blue);

        qreal x = coordinateToPositionX(getPanelX());
        qreal y = coordinateToPositionY(getPanelY());
        this->addEllipse(collisionX(x)-BALL_RADIUS, collisionY(y)-BALL_RADIUS, 2*BALL_RADIUS, 2*BALL_RADIUS, ballPen, ballBrush);
        QString coordinates = QString("X: ") + QString::number(getPanelX()) + QString(" Y: ") + QString::number(getPanelY());
        this->addText(coordinates);
    }
}

bool Scene2d::getBallOnPlate()
{
    return ballOnPlate;
}

int Scene2d::getPanelX()
{
    return panelX;
}

int Scene2d::getPanelY()
{
    return panelY;
}

void Scene2d::setBallOnPlate(bool onPlate)
{
    ballOnPlate = onPlate;
}

void Scene2d::setPanel(int x, int y)
{
    ballOnPlate = true;
    panelX = x;
    panelY = y;
}

qreal Scene2d::coordinateToPositionX(int x)
{
    const qreal X_MAX = 1000;
    //return (x-LEFT_BORDER)*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
    return LEFT_BORDER + x*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
}

qreal Scene2d::coordinateToPositionY(int y)
{
    const qreal Y_MAX = 1000;
    //return (y-TOP_BORDER)*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
    return TOP_BORDER + y*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
}

qreal Scene2d::collisionX(qreal x)
{
    if(x-BALL_RADIUS < LEFT_BORDER)
        return LEFT_BORDER + BALL_RADIUS;

    if(x+BALL_RADIUS > RIGHT_BORDER)
        return RIGHT_BORDER - BALL_RADIUS;

    return x;
}

qreal Scene2d::collisionY(qreal y)
{
    if(y-BALL_RADIUS < TOP_BORDER)
        return TOP_BORDER + BALL_RADIUS;

    if(y+BALL_RADIUS > BOTTOM_BORDER)
        return BOTTOM_BORDER - BALL_RADIUS;

    return y;
}
