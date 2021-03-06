
#ifndef CIVICO13_GOL_H
#define CIVICO13_GOL_H

// Qt
#include <QtWidgets/QGraphicsWidget>
#include <QtGui/QPainter>
#include <QtCore/QPointer>

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
         else if (_state == 2)
            color = Qt::green;
         else if (_state == -1)
            color = Qt::red;

         color.setAlphaF(0.5);
         painter->fillRect(rect(), color);

         //std::cout << "paint";
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

    Cell* GetCell(unsigned int rowIdx, unsigned int colIdx);

    unsigned int GetNumColumn(){return _numColumn;}
    unsigned int GetNumRows(){return _numRows;}


    void SaveToFile(const std::string& fileName);
    void LoadFromFile(const std::string& fileName);



protected:

   std::vector<QPointer<Cell> > _cellVec;
   unsigned int _numColumn;
   unsigned int _numRows;

   QGraphicsScene* _pScene;

};



//////////////////////////////////////////////////////////////////////////
class GameOfLife : public QObject
{
   Q_OBJECT

public:

   typedef std::vector<int> LifeHistory;

   GameOfLife() : _markPath(false){}

   void SetGrid(Grid* pGrid);

   // disabled by default
   bool GetUseMarkPath() const { return _markPath; }
   void SetUseMarkPath(bool val) { _markPath = val; }

   void Step();

   const LifeHistory& GetLifeHistory() const { return _lifeHistory; }
   void ResetHistory();

   void ExportHistory(const std::string& fileName);

protected:

   void ApplyGolRule( Cell* pCell, unsigned int neighbors, unsigned cellIdx );


   QPointer<Grid> _pGrid;
   std::vector<int> _cells;

   LifeHistory _lifeHistory;
   unsigned int _aliveCounter;

   bool _markPath;   
};

#endif
