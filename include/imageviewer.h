
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

// civico13gol
#include <grid.h>

// UI generated file
#include <ui_GOL.h>


// Qt
#include <QtWidgets/QMainWindow>

#include <QtPrintSupport/QPrinter>

#include <QtCore/QPointer>
#include <QtCore/QTimer>


QT_BEGIN_NAMESPACE
class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
QT_END_NAMESPACE


#include <iostream>


class ImageViewer : public QMainWindow
{
   Q_OBJECT

public:
   ImageViewer();
   ~ImageViewer();

   void wheelEvent(QWheelEvent * event);

   private slots:
      void open();
      void print();

      void createGrid();
      void resetGrid();

      void saveGrid();
      void loadGrid();

      void reloadGrid();

      void normalSize();
      void fitToWindow();


      void saveImage();
      void saveImageGridOnly();
      void exportLifeHistory();

      void markPath();

      void step();

      void play();
      void pause();

private:

#ifndef QT_NO_PRINTER
   QPrinter printer;
#endif


   QGraphicsScene* _pScene;
   QGraphicsView* _pGraphicsView;

   QGraphicsPixmapItem* _pImageItem;

   QPointer<Grid> _pGrid;

   QPointer<GameOfLife> _pGOL;

   // main win from designer
   Ui::MainWindow* _pMainWindow;

   QString _lastFilename;

   bool _pause;
   QTimer _timer;   


};


#endif
