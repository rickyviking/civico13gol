
#ifndef CIVICO13_GOL_H
#define CIVICO13_GOL_H

#include <QtGui/QGraphicsWidget>
#include <QtGui/QtGui>

//////////////////////////////////////////////////////////////////////////
class Cell : public QGraphicsWidget
{
public:

   Cell() :
      _alive(false)
      {

      }


      void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
         QWidget *)
      {
         QColor color(Qt::blue);
         if (_alive)
            color = Qt::yellow;

         color.setAlphaF(0.7);
         painter->fillRect(rect(), color);
      }

      void mousePressEvent(QGraphicsSceneMouseEvent *event)
      {
         _alive = !_alive;
         update();
      }

      bool _alive;
};




//////////////////////////////////////////////////////////////////////////
class Grid : public QObject
{
public:
    Grid();
};

#endif
