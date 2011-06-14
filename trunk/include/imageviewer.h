
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

// civico13gol
#include <grid.h>


// Qt
#include <QtGui/QMainWindow>
#include <QtGui/QPrinter>
#include <QtGui/QtGui>


QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE


#include <iostream>







//! [0]
class ImageViewer : public QMainWindow
{
   Q_OBJECT

public:
   ImageViewer();

   void wheelEvent(QWheelEvent * event);

   private slots:
      void open();
      void print();
      void createGrid();
      void zoomOut();
      void normalSize();
      void fitToWindow();
      void about();

private:
   void createActions();
   void createMenus();
   void updateActions();
   void scaleImage(double factor);
   void adjustScrollBar(QScrollBar *scrollBar, double factor);

   QLabel *imageLabel;
   QScrollArea *scrollArea;
   double scaleFactor;

#ifndef QT_NO_PRINTER
   QPrinter printer;
#endif

   QAction *openAct;
   QAction *printAct;
   QAction *exitAct;
   QAction *_pCreateGridAction;
   QAction *zoomOutAct;
   QAction *normalSizeAct;
   QAction *fitToWindowAct;
   QAction *aboutAct;
   QAction *aboutQtAct;

   QMenu *fileMenu;
   QMenu *_pToolMenu;
   QMenu *helpMenu;


   QGraphicsScene* _pScene;
   QGraphicsView* _pGraphicsView;
};
//! [0]

#endif
