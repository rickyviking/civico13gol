
// civico
#include <GridScene.h>
#include <grid.h>

// stl
#include <iostream>

//////////////////////////////////////////////////////////////////////////
GridScene::GridScene() :
   _markEdges(false)
{

}



//////////////////////////////////////////////////////////////////////////
void GridScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if (event->button() != Qt::RightButton)
      return;

   QGraphicsItem* pItem = itemAt(event->scenePos());
   Cell* pCell = dynamic_cast<Cell*>(pItem);
   if (!pCell != NULL)
      return;

   // marking edges
   if (_markEdges)
   {
      if (pCell->_state == -1)
         pCell->_state = 0;
      else
         pCell->_state = -1;
   }
   // marking dead / alive
   else
   {
      if (pCell->_state == 0)
         pCell->_state = 1;
      else if(pCell->_state == 1)
         pCell->_state = 0;
   }

   pCell->update();
   //update(sceneRect());
}

//////////////////////////////////////////////////////////////////////////
void GridScene::ChangeMouseMode( int buttonID )
{
   // button ID decided upstairs...

   if (buttonID == 101)
      _markEdges = false;
   else
      _markEdges = true;
}
