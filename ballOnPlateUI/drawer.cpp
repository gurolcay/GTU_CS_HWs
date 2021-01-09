#include "drawer.h"
#include <QGraphicsSceneMouseEvent>

Drawer::Drawer(QObject * parent):QGraphicsScene(parent)
{
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);
}

void Drawer::mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(mouseEvent->button() == Qt::LeftButton && !finishPath){
        if(closeToStartPoint){
            closeToStartPoint = false;
            finishPath = true;
            //pathReady = true;
        }
        else{
            QPointF point = mouseEvent->scenePos();
            point = collision(point.x(), point.y());
            path.append(point);
        }
    }
    else if(mouseEvent->button() == Qt::RightButton){
        path.clear();
        closeToStartPoint = false;
        finishPath = false;
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void Drawer::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!finishPath && path.size() > 1){
        qreal dx = mouseEvent->scenePos().x() - path.at(0).x();
        qreal dy = mouseEvent->scenePos().y() - path.at(0).y();
        if(qSqrt(dx*dx+dy*dy) < MAX_RADIUS)
            closeToStartPoint = true;
        else
            closeToStartPoint = false;
    }
    setMouseX(positionToCoordinateX(mouseEvent->scenePos().x()));
    setMouseY(positionToCoordinateY(mouseEvent->scenePos().y()));

    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void Drawer::draw(){
    this->clear();
    this->addRect(LEFT_BORDER, TOP_BORDER, RIGHT_BORDER-LEFT_BORDER, BOTTOM_BORDER-TOP_BORDER);

    if(!path.empty()){
        QPen linePen(Qt::black);
        linePen.setWidth(POINT_RADIUS);
        linePen.setJoinStyle(Qt::RoundJoin);
        QPen pointPen(Qt::red);
        QBrush pointBrush(Qt::red);
        QPen startPointPen(Qt::green);
        QBrush startPointBrush(Qt::green);

        for(int i=1;i<path.size();++i)
            this->addLine(QLineF(path.at(i-1), path.at(i)), linePen);

        if(finishPath)
            this->addLine(QLineF(path.at(path.size()-1), path.at(0)), linePen);

        for(int i=1;i<path.size();++i)
            this->addEllipse(path.at(i).x()-POINT_RADIUS, path.at(i).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, pointPen, pointBrush);

        if(closeToStartPoint)
            this->addEllipse(path.at(0).x()-MAX_RADIUS, path.at(0).y()-MAX_RADIUS, 2*MAX_RADIUS, 2*MAX_RADIUS, startPointPen, startPointBrush);
        else
            this->addEllipse(path.at(0).x()-POINT_RADIUS, path.at(0).y()-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, startPointPen, startPointBrush);

        if(finishPath && getBallOnPlate()){
            QPen ballPen(Qt::blue);
            QBrush ballBrush(Qt::blue);

            this->addEllipse(coordinateToPositionX(getPanelX())-POINT_RADIUS, coordinateToPositionY(getPanelY())-POINT_RADIUS, 2*POINT_RADIUS, 2*POINT_RADIUS, ballPen, ballBrush);
        }
    }
    QString coordinates = QString("X: ") + QString::number(getMouseX()) + QString(" Y: ") + QString::number(getMouseY());
    this->addText(coordinates);
}

bool Drawer::getFinishPath()
{
    return finishPath;
}

bool Drawer::getBallOnPlate()
{
    return ballOnPlate;
}

bool Drawer::isPathReady()//returns true only once
{
    if(pathReady){
        pathReady = false;
        return true;
    }

    return false;
}

int Drawer::getPanelX()
{
    return panelX;
}

int Drawer::getPanelY()
{
    return panelY;
}

int Drawer::getMouseX()
{
    return mouseX;
}

int Drawer::getMouseY()
{
    return mouseY;
}

void Drawer::setMouseX(int x)
{
    mouseX = x;
}

void Drawer::setMouseY(int y)
{
    mouseY = y;
}

void Drawer::setBallOnPlate(bool onPlate)
{
    ballOnPlate = onPlate;
}

void Drawer::setPanel(int x, int y)
{
    ballOnPlate = true;
    panelX = x;
    panelY = y;
}

QList<QPoint> Drawer::getPathList()
{
    QList<QPoint> list;
    for(int i=0;i<path.size();++i)
        list.append(positionToCoordinate(path.at(i)));

    return list;
}

qreal Drawer::coordinateToPositionX(int x)
{
    const qreal X_MAX = 1000;
    //return (x-LEFT_BORDER)*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
    return LEFT_BORDER + x*(RIGHT_BORDER-LEFT_BORDER)/X_MAX;
}

qreal Drawer::coordinateToPositionY(int y)
{
    const qreal Y_MAX = 1000;
    //return (y-TOP_BORDER)*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
    return TOP_BORDER + y*(BOTTOM_BORDER-TOP_BORDER)/Y_MAX;
}

qreal Drawer::collisionX(qreal x)
{
    if(x-MAX_RADIUS < LEFT_BORDER)
        return LEFT_BORDER + MAX_RADIUS;

    if(x+MAX_RADIUS > RIGHT_BORDER)
        return RIGHT_BORDER - MAX_RADIUS;

    return x;
}

qreal Drawer::collisionY(qreal y)
{
    if(y-MAX_RADIUS < TOP_BORDER)
        return TOP_BORDER + MAX_RADIUS;

    if(y+MAX_RADIUS > BOTTOM_BORDER)
        return BOTTOM_BORDER - MAX_RADIUS;

    return y;
}

QPointF Drawer::collision(QPointF &point)
{
    return collision(point.x(), point.y());
}

QPointF Drawer::collision(qreal x, qreal y)
{
    return QPointF(collisionX(x), collisionY(y));
}

int Drawer::positionToCoordinateX(qreal x)
{
    const qreal X_MAX = 1000;
    return LEFT_BORDER + x*X_MAX/(RIGHT_BORDER-LEFT_BORDER);
}

int Drawer::positionToCoordinateY(qreal y)
{
    const qreal Y_MAX = 1000;
    return TOP_BORDER + y*Y_MAX/(BOTTOM_BORDER-TOP_BORDER);
}

QPoint Drawer::positionToCoordinate(const QPointF &point){
    return QPoint(positionToCoordinateX(point.x()), positionToCoordinateY(point.y()));
}

