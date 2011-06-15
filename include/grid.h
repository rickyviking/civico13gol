
#ifndef CIVICO13_GOL_H
#define CIVICO13_GOL_H

// Qt
#include <QtGui/QGraphicsWidget>
#include <QtGui/QtGui>

// stl
#include <iostream>


//////////////////////////////////////////////////////////////////////////
class Cell : public QGraphicsWidget
{
public:

   Cell() :
      _alive(false)
      {
         //setAcceptHoverEvents(true);
         setAcceptedMouseButtons(Qt::RightButton);
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
         if (event->button() == Qt::RightButton)
         {
            _alive = !_alive;
            update();
         }
      }

      void hoverEnterEvent(QGraphicsSceneDragDropEvent *event)
      {
         std::cout << "hover!" << std::endl;
      }

      bool _alive;
};




//////////////////////////////////////////////////////////////////////////
class Grid : public QObject
{
public:
    Grid(QGraphicsScene* pScene);

    void CreateGrid(unsigned int numHorizontalCell);
    void ResetGrid();


protected:

   std::vector<QPointer<Cell> > _cellVec;
   unsigned int _numColumn;
   unsigned int _numRows;

   QGraphicsScene* _pScene;

};

#endif
