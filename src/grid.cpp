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

//////////////////////////////////////////////////////////////////////////
Cell* Grid::GetCell( unsigned int rowIdx, unsigned int colIdx )
{
   if (rowIdx < _numRows && colIdx < _numColumn)
      return _cellVec[rowIdx* _numColumn + colIdx];

   return NULL;
}


//////////////////////////////////////////////////////////////////////////
void GameOfLife::SetGrid( Grid* pGrid )
{
   _pGrid = pGrid;
   _cells.clear();
   _cells.resize(_pGrid->GetNumColumn() * _pGrid->GetNumRows());
}

//////////////////////////////////////////////////////////////////////////
void GameOfLife::Step()
{

   std::cout << "stepping...";


   unsigned int numRows = _pGrid->GetNumRows();
   unsigned int numCol = _pGrid->GetNumColumn();

   // run the inside loop before
   for (unsigned int i=1; i<numRows-1; ++i)
   {
      for (unsigned int j=1; j<numCol-1; ++j)
      {
         // check the num of neighbors that are alive
         Cell* pCell = _pGrid->GetCell(i, j);
         unsigned int neighbors = 0;
         
         // previous column
         unsigned int col = j-1;
         for (unsigned k = i-1; k<i+2; ++k)
         {
            if (_pGrid->GetCell(k, col)->_state == 1)
               ++neighbors;
         }
         // next column
         col = j+1;
         for (unsigned k = i-1; k<i+2; ++k)
         {
            if (_pGrid->GetCell(k, col)->_state == 1)
               ++neighbors;
         }

         // up ^ down
         if (_pGrid->GetCell(i-1, j)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, j)->_state == 1)
            ++neighbors;


         unsigned cellIdx = i*numCol + j;

         ApplyGolRule(pCell, neighbors, cellIdx);


      }
   }



   // now run on the borders










   // now apply the new state to the atual cells to draw
   for (unsigned int i=0; i<numRows; ++i)
   {
      for (unsigned int j=0; j<numCol; ++j)
      {
         Cell* pCell = _pGrid->GetCell(i, j);
         if (pCell->_state != -1)
            pCell->_state = _cells[i*numCol+j];
      }
   }

   std::cout << " done!" << std::endl;

}

//////////////////////////////////////////////////////////////////////////
void GameOfLife::ApplyGolRule( Cell* pCell, unsigned int neighbors, unsigned cellIdx )
{
   // now apply the GOL rule:
   if(pCell->_state == 0)
   {
      if (neighbors == 3)
         _cells[cellIdx] = 1;
      else
         _cells[cellIdx] = 0;
   }
   else if (pCell->_state == 1)
   {
      if (neighbors < 2)
         _cells[cellIdx] = 0;
      else if (neighbors > 3)
         _cells[cellIdx] = 0;
      else
         _cells[cellIdx] = 1;
   }
}
