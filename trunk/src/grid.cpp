//////////////////////////////////////////////////////////////////////////
// main grid class
// @author: Riccardo Corsi
//////////////////////////////////////////////////////////////////////////\


#include <grid.h>

#include <iostream>


//////////////////////////////////////////////////////////////////////////
void Grid::CreateGrid(unsigned int numColumn)
{
   // reset any previous grid
   ResetGrid();

   // compute dimension
   const QRectF& rect = _pScene->sceneRect();
   float size = rect.width() / (float) numColumn;
   float offset = 0.1f * size;
   float cellSize = size - 2.f*offset;

   _numColumn = numColumn;
   _numRows = (float) numColumn * rect.height() / rect.width();

   std::cout << "columns: " << _numColumn << " rows: " << _numRows << std::endl;

   // fill the actual grid
   for (unsigned int i =0; i<_numRows; ++i)
   {
      for (unsigned int j=0; j<_numColumn; ++j)
      {
         Cell* pCell = new Cell;
         pCell->setZValue(1);
         pCell->setGeometry(j*size+offset, i*size + offset, cellSize, cellSize);

         _cellVec.push_back(pCell);
         _pScene->addItem(pCell);
      }
   }

}

//////////////////////////////////////////////////////////////////////////
void Grid::ResetGrid()
{
   for (unsigned int i =0; i<_cellVec.size(); ++i)
      _pScene->removeItem(_cellVec[i]);

   _cellVec.clear();
}

//////////////////////////////////////////////////////////////////////////
Grid::Grid( QGraphicsScene* pScene ) :
   _pScene(pScene)
{

}
