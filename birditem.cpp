#include "birditem.h"
#include <QTimer>
#include <QGraphicsScene>

BirdItem::BirdItem(QPixmap pixmap) :
    wingposition(WingPosition::Up),
    wingDirection(0)

{
    setPixmap(pixmap);

    QTimer * birdWingsTimer = new QTimer(this);
    connect(birdWingsTimer,&QTimer::timeout,[=]() {

        updatePixmap();

    });

    birdWingsTimer->start(90);

    groundPosition = scenePos().y() + 290;

    yAnimation = new QPropertyAnimation(this,"y",this);
    yAnimation->setStartValue(scenePos().y());
    yAnimation->setEndValue(groundPosition);
    yAnimation->setEasingCurve(QEasingCurve::InQuad);
    yAnimation->setDuration(1000);

  //  yAnimation->start();

    rotationAnimation = new QPropertyAnimation(this,"rotation",this);

   // rotateTo(90,1200,QEasingCurve::InQuad);



}

void BirdItem::updatePixmap()
{
    if(wingposition == WingPosition::Middle) {

        if(wingDirection){
            //up
            setPixmap(QPixmap(":/images/bird_wings_up.png"));
            wingposition = WingPosition::Up;
            wingDirection = 0;

        } else {
            //down
            setPixmap(QPixmap(":/images/bird_wings_down.png"));
            wingposition = WingPosition::Down;
            wingDirection = 1;
        }

    } else {
        setPixmap(QPixmap(":/images/bird_wings_middle.png"));
        wingposition = WingPosition::Middle;
    }

}

qreal BirdItem::rotation() const
{
    return m_rotation;
}

void BirdItem::setRotation(qreal newRotation)
{
    m_rotation = newRotation;

    QPointF c = boundingRect().center();

    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(newRotation);
    t.translate(-c.x(), -c.y());
    setTransform(t);

}

qreal BirdItem::y() const
{
    return m_y;
}

void BirdItem::setY(qreal newY)
{
    moveBy(0,newY-m_y);
    m_y = newY;
}

void BirdItem::shootUp()
{

    yAnimation->stop();
    rotationAnimation->stop();

    qreal curPosY = y();

    yAnimation->setStartValue(curPosY);
    yAnimation->setEndValue(curPosY - scene()->sceneRect().height()/8);
    yAnimation->setEasingCurve(QEasingCurve::OutQuad);
    yAnimation->setDuration(285);

    connect(yAnimation,&QPropertyAnimation::finished,[=]() {

        fallToGroundIfNecessary();

    });


    yAnimation->start();

    rotateTo(-20,200,QEasingCurve::OutCubic);


}

void BirdItem::startFlying()
{
    yAnimation->start();
    rotateTo(180,1200,QEasingCurve::InQuad);

}

void BirdItem::freezeInPlace()
{

    yAnimation->stop();
    rotationAnimation->stop();
}

void BirdItem::rotateTo(const qreal &end, const int &duration, const QEasingCurve &curve)
{
    rotationAnimation->setStartValue(rotation());
    rotationAnimation->setEndValue(end);
    rotationAnimation->setEasingCurve(curve);
    rotationAnimation->setDuration(duration);

    rotationAnimation->start();
}

void BirdItem::fallToGroundIfNecessary()
{
    if( y() < groundPosition) {

        rotationAnimation->stop();
       // yAnimation->stop();

        yAnimation->setStartValue(y());
        yAnimation->setEndValue(groundPosition);
        yAnimation->setEasingCurve(QEasingCurve::InQuad);
        yAnimation->setDuration(1200);

        yAnimation->start();

        rotateTo(90,1100,QEasingCurve::InCubic);

    }

}
