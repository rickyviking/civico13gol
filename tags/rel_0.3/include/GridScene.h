
#ifndef CIVICO13_GOL_GRID_SCENE_H
#define CIVICO13_GOL_GRID_SCENE_H

// Qt
#include <QtGui/QtGui>

// stl
#include <iostream>


//////////////////////////////////////////////////////////////////////////
class GridScene : public QGraphicsScene
{
   Q_OBJECT


public:

   // default ctor
   GridScene();

   // reimplement this to change grid cells
   void mousePressEvent(QGraphicsSceneMouseEvent *event);

   
public slots:
   void ChangeMouseMode(int buttonID);
   

protected:
   bool _markEdges;
  
};


#endif // CIVICO13_GOL_GRID_SCENE_H