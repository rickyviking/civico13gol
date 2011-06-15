//////////////////////////////////////////////////////////////////////////
// main grid class
// @author: Riccardo Corsi
//////////////////////////////////////////////////////////////////////////\


#include <grid.h>

// stl
#include <iostream>
#include <fstream>


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
void Grid::SaveToFile( const std::string& fileName )
{
   std::ofstream fileOut;
   std::cout << "save to " << fileName << std::endl;
   fileOut.open(fileName.c_str());

   /*
   if (!fileOut.is_open())
   {
      std::cout << "FILE ISSUE" << std::endl;
      return;
   }
   */

   fileOut << _numRows << " " << _numColumn << std::endl;
   for(unsigned int i =0; i<_numRows; ++i)
   {
      for (unsigned int j=0; j<_numColumn; ++j)
      {
         fileOut << GetCell(i, j)->_state << " ";
      }

      fileOut << std::endl;
   }

   fileOut.close();

}

//////////////////////////////////////////////////////////////////////////
void Grid::LoadFromFile( const std::string& fileName )
{
   ResetGrid();

   std::ifstream fileIn(fileName.c_str());
   unsigned int numRows;
   fileIn >> numRows;
   unsigned int numCol;
   fileIn >> numCol;

   _numColumn = numCol;
   _numRows = numRows;

   // compute dimension
   const QRectF& rect = _pScene->sceneRect();
   float size = rect.width() / (float) numCol;
   float offset = 0.1f * size;
   float cellSize = size - 2.f*offset;

   int value;
   for(unsigned int i = 0; i< numRows; ++i)
   {
      for ( unsigned int j=0; j<numCol; ++j)
      {
         fileIn >> value;
         
         Cell* pCell = new Cell;
         pCell->setZValue(1);
         pCell->setGeometry(j*size+offset, i*size + offset, cellSize, cellSize);
         pCell->_state = value;

         _cellVec.push_back(pCell);
         _pScene->addItem(pCell);

      }
   }

   _pScene->update(_pScene->sceneRect());
   
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

   // reset alive counter for this step
   _aliveCounter = 0;

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

   // upper row
   {
      unsigned int i=0;

      for (unsigned int j=1; j<numCol-1; ++j)
      {
         // check the num of neighbors that are alive
         Cell* pCell = _pGrid->GetCell(i, j);
         unsigned int neighbors = 0;

         // previous column
         unsigned int col = j-1;
         if (_pGrid->GetCell(i, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, col)->_state == 1)
            ++neighbors;

         // next column
         col = j+1;
         if (_pGrid->GetCell(i, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, col)->_state == 1)
            ++neighbors;

         // down         
         if (_pGrid->GetCell(i+1, j)->_state == 1)
            ++neighbors;


         unsigned cellIdx = i*numCol + j;

         ApplyGolRule(pCell, neighbors, cellIdx);
      }
   }

   // bottom row
   {
      unsigned int i=numRows-1;

         for (unsigned int j=1; j<numCol-1; ++j)
         {
            // check the num of neighbors that are alive
            Cell* pCell = _pGrid->GetCell(i, j);
            unsigned int neighbors = 0;

            // previous column
            unsigned int col = j-1;
            if (_pGrid->GetCell(i, col)->_state == 1)
               ++neighbors;
            if (_pGrid->GetCell(i-1, col)->_state == 1)
               ++neighbors;

            // next column
            col = j+1;
            if (_pGrid->GetCell(i, col)->_state == 1)
               ++neighbors;
            if (_pGrid->GetCell(i-1, col)->_state == 1)
               ++neighbors;

            // up
            if (_pGrid->GetCell(i-1, j)->_state == 1)
               ++neighbors;


            unsigned cellIdx = i*numCol + j;

            ApplyGolRule(pCell, neighbors, cellIdx);
         }
   }

   // left column
   {
      unsigned int j=0;

      for (unsigned int i=1; i<numRows-1; ++i)
      {
         // check the num of neighbors that are alive
         Cell* pCell = _pGrid->GetCell(i, j);
         unsigned int neighbors = 0;
        
         // next column
         unsigned int col = j+1;
         if (_pGrid->GetCell(i-1, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, col)->_state == 1)
            ++neighbors;

         // up ^ down
         if (_pGrid->GetCell(i-1, j)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, j)->_state == 1)
            ++neighbors;


         unsigned cellIdx = i*numCol + j;

         ApplyGolRule(pCell, neighbors, cellIdx);
      }
   }


   // right column
   {
      unsigned int j=numCol-1;

      for (unsigned int i=1; i<numRows-1; ++i)
      {
         // check the num of neighbors that are alive
         Cell* pCell = _pGrid->GetCell(i, j);
         unsigned int neighbors = 0;

         // previous column
         unsigned int col = j-1;
         if (_pGrid->GetCell(i-1, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i, col)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, col)->_state == 1)
            ++neighbors;

         // up ^ down
         if (_pGrid->GetCell(i-1, j)->_state == 1)
            ++neighbors;
         if (_pGrid->GetCell(i+1, j)->_state == 1)
            ++neighbors;


         unsigned cellIdx = i*numCol + j;

         ApplyGolRule(pCell, neighbors, cellIdx);
      }
   }


   // finally the corners
   // top left
   {
      // check the num of neighbors that are alive
      Cell* pCell = _pGrid->GetCell(0, 0);
      unsigned int neighbors = 0;

      // previous column
      if (_pGrid->GetCell(1, 0)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(0, 1)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(1, 1)->_state == 1)
         ++neighbors;

      ApplyGolRule(pCell, neighbors, 0);
   }

   // top right
   {

      // check the num of neighbors that are alive
      unsigned int col = numCol -1;
      Cell* pCell = _pGrid->GetCell(0, col);
      unsigned int neighbors = 0;

      // previous column
      if (_pGrid->GetCell(0, col-1)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(1, col)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(1, col-1)->_state == 1)
         ++neighbors;

      ApplyGolRule(pCell, neighbors, numCol-1);
   }


   // bottom right
   {
      // check the num of neighbors that are alive
      unsigned int row = numRows-1;
      unsigned int col = numCol -1;
      Cell* pCell = _pGrid->GetCell(row, col);
      unsigned int neighbors = 0;

      // previous column
      if (_pGrid->GetCell(row, col-1)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(row-1, col)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(row-1, col-1)->_state == 1)
         ++neighbors;

      ApplyGolRule(pCell, neighbors, numCol*numRows-1);
   }

   // bottom left
   {
      // check the num of neighbors that are alive
      unsigned int row = numRows-1;      
      Cell* pCell = _pGrid->GetCell(row, 0);
      unsigned int neighbors = 0;

      // previous column    
      if (_pGrid->GetCell(row-1, 0)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(row-1, 1)->_state == 1)
         ++neighbors;
      if (_pGrid->GetCell(row, 1)->_state == 1)
         ++neighbors;

      ApplyGolRule(pCell, neighbors, numCol*(numRows-1));
   }




   // now apply the new state to the actual cells to draw
   for (unsigned int i=0; i<numRows; ++i)
   {
      for (unsigned int j=0; j<numCol; ++j)
      {
         Cell* pCell = _pGrid->GetCell(i, j);
         if (pCell->_state != -1)
            pCell->_state = _cells[i*numCol+j];
      }
   }

   _lifeHistory.push_back(_aliveCounter);


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

   if (_cells[cellIdx] == 1)
      ++_aliveCounter;
}

//////////////////////////////////////////////////////////////////////////
void GameOfLife::ResetHistory()
{
   _lifeHistory.clear();
}
