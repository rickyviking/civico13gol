
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
      _state(0)
      {
         //setAcceptHoverEvents(true);
         // disable mouse press event
         setAcceptedMouseButtons(0);
      }

      void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
         QWidget *)
      {
         QColor color(Qt::blue);
         if (_state == 1)
            color = Qt::yellow;
         else if (_state == -1)
            color = Qt::red;

         color.setAlphaF(0.5);
         painter->fillRect(rect(), color);
      }


      // -1 = edge
      //  0 = dead
      //  1 = alive
      //  2 = path (vediamo...)
      int _state; 
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
