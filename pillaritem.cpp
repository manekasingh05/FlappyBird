#include "pillaritem.h"
#include <QRandomGenerator>
#include <QDebug>
#include <QGraphicsScene>
#include "scene.h"
#include "birditem.h"



PillarItem::PillarItem() :
    topPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillars.png"))),
    bottomPillar(new QGraphicsPixmapItem(QPixmap(":/images/pillars.png"))),
    pastBird(false)

{
    topPillar->setPos(QPointF(0,0) - QPointF(topPillar->boundingRect().width()/2,topPillar->boundingRect().height()/2 +550));
    bottomPillar->setPos(QPointF(0,0) - QPointF(bottomPillar->boundingRect().width()/2,topPillar->boundingRect().height()/2 -550));

    addToGroup(topPillar);
    addToGroup(bottomPillar);

   yPos = QRandomGenerator::global()->bounded(150);
   int xRandomizer = QRandomGenerator::global()->bounded(200);

   setPos(QPointF(0,0) + QPointF(260 + xRandomizer,yPos));


    xAnimation = new QPropertyAnimation(this,"x",this);
    xAnimation->setStartValue(260 + xRandomizer);
    xAnimation->setEndValue(-260);
    xAnimation->setEasingCurve(QEasingCurve::Linear);
    xAnimation->setDuration(3500);

    connect(xAnimation,&QPropertyAnimation::finished,[=]() {
        qDebug() << "Animation finished";
        scene()->removeItem(this);
        delete this;

    });

    xAnimation->start();


}

PillarItem::~PillarItem()
{

    qDebug() << "Deleting Pillars";
    delete topPillar;
    delete bottomPillar;

}

qreal PillarItem::x() const
{
    return m_x;
}

void PillarItem::setX(qreal newX)
{
   // qDebug() << "Pillar position:" << newX;
    m_x = newX;

    if(newX < 0 && !pastBird) {
        pastBird = true;
        QGraphicsScene * mScene = scene();
        Scene * myScene = dynamic_cast<Scene *>(mScene);
        if(myScene) {
            myScene->incrementScore();

        }

    }





    if(collidesWithBird()) {
        emit collideFail();
    }
    setPos(QPointF(0,0) + QPointF(newX,yPos));
}

void PillarItem::freezeInPlace()
{
    xAnimation->stop();
}

bool PillarItem::collidesWithBird()
{

    QList<QGraphicsItem*> collidingItems = topPillar->collidingItems();
    collidingItems.append(bottomPillar->collidingItems());

    foreach(QGraphicsItem * item, collidingItems) {

        BirdItem * birdItem = dynamic_cast<BirdItem*>(item);
        if(birdItem) {

            return true;
        }

    }
    return false;

}
