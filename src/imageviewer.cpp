// civico
#include <imageviewer.h>
#include <GridScene.h>

// Qt
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QMessageBox>
#include <QtCore/QtMath>

// stl
#include <sstream>


ImageViewer::ImageViewer() :
   _pImageItem(NULL)
{
   // create main window
   _pMainWindow = new Ui::MainWindow;
   _pMainWindow->setupUi(this);

   // connect from menu
   connect(_pMainWindow->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
   connect(_pMainWindow->actionReset_Zoom, SIGNAL(triggered()), this, SLOT(normalSize()));

   connect(_pMainWindow->actionSave_Grid, SIGNAL(triggered()), this, SLOT(saveGrid()));
   connect(_pMainWindow->actionLoad_Grid, SIGNAL(triggered()), this, SLOT(loadGrid()));

   connect(_pMainWindow->actionReload_Grid, SIGNAL(triggered()), this, SLOT(reloadGrid()));

   connect(_pMainWindow->actionSave_Image_Grid_Only, SIGNAL(triggered()), this, SLOT(saveImageGridOnly()));
   connect(_pMainWindow->actionSave_Image, SIGNAL(triggered()), this, SLOT(saveImage()));
   connect(_pMainWindow->actionExport_Life_History, SIGNAL(triggered()), this, SLOT(exportLifeHistory()));

   // connect grid buttons
   connect(_pMainWindow->createGrid, SIGNAL(pressed()), this, SLOT(createGrid()));
   connect(_pMainWindow->clearGrid, SIGNAL(pressed()), this, SLOT(resetGrid()));


   // connect GOL step
   connect(_pMainWindow->markPathCheckBox, SIGNAL(clicked()), this, SLOT(markPath()));

   connect(_pMainWindow->gol_step, SIGNAL(pressed()), this, SLOT(step()));

   connect(_pMainWindow->playButton, SIGNAL(pressed()), this, SLOT(play()));
   connect(_pMainWindow->pauseButton, SIGNAL(pressed()), this, SLOT(pause()));

   connect(&_timer, SIGNAL(timeout()), this, SLOT(step()));

   

   // initialized a caso - resize the scene rect when loading an image
   //_pScene = new QGraphicsScene(0, 0, 800, 500);
   _pScene = new GridScene();

   // assign proper ID to the button, gridScene will recognize them
   _pMainWindow->mouseModeGroup->setId(_pMainWindow->markLiveRadio, 101);
   _pMainWindow->mouseModeGroup->setId(_pMainWindow->markEdgeRadio, 102);
   
   connect(_pMainWindow->mouseModeGroup, SIGNAL(buttonClicked(int)), _pScene, SLOT(ChangeMouseMode(int)));

   _pGraphicsView = new QGraphicsView(_pScene);
   _pGraphicsView->setFrameStyle(0);
   _pGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
   _pGraphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
   //_pGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   //_pGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   _pGraphicsView->setRenderHint(QPainter::Antialiasing);
   //_pGraphicsView->setCacheMode(QGraphicsView::CacheBackground);
   //_pGraphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
   _pGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
   _pGraphicsView->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));

   //view.resize(400, 300);
   //view.show();

   setCentralWidget(_pGraphicsView);

   setWindowTitle(tr("Civico13-GOL"));
   resize(800, 600);

   // create the grid instance
   _pGrid = new Grid(_pScene);
   // create also a GOL instance
   _pGOL = new GameOfLife;
   _pGOL->SetGrid(_pGrid);
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::open()
{
   QString fileName = QFileDialog::getOpenFileName(this,
      tr("Open File"), QDir::currentPath());
   if (!fileName.isEmpty()) 
   {
      QImage image(fileName);
      if (image.isNull()) 
      {
         QMessageBox::information(this, tr("Image Viewer"),
            tr("Cannot load %1.").arg(fileName));
         return;
      }

      // remove old image if any
      if (_pImageItem != NULL)
      {
         _pScene->removeItem(_pImageItem);
         delete _pImageItem;
      }
      
      _pImageItem = new QGraphicsPixmapItem;
      _pImageItem->setPixmap(QPixmap::fromImage(image));
      // use 0 as background level...
      _pImageItem->setZValue(-1);
      _pScene->addItem(_pImageItem);

      _pScene->setSceneRect(_pImageItem->boundingRect());
   }
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::print()
{

   /*

   Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
   QPrintDialog dialog(&printer, this);

   if (dialog.exec()) 
   {
      QPainter painter(&printer);
      QRect rect = painter.viewport();
      QSize size = imageLabel->pixmap()->size();
      size.scale(rect.size(), Qt::KeepAspectRatio);
      painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
      painter.setWindow(imageLabel->pixmap()->rect());
      painter.drawPixmap(0, 0, *imageLabel->pixmap());
   }
#endif


   */
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::createGrid()
{
   std::cout << "create the grid"<< std::endl;

   unsigned int numCol = _pMainWindow->numColBox->value();
   _pGrid->CreateGrid(numCol);
   _pGOL->SetGrid(_pGrid);

}


//////////////////////////////////////////////////////////////////////////
void ImageViewer::resetGrid()
{
   std::cout << "reset the grid"<< std::endl;
   _pGrid->ResetGrid();
   _pGOL->SetGrid(_pGrid);

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::normalSize()
{
   QTransform xForm;
   _pGraphicsView->setTransform(xForm);
   std::cout << "NORMAL SIZE" << std::endl;
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::fitToWindow()
{
   /*
   bool fitToWindow = fitToWindowAct->isChecked();
   scrollArea->setWidgetResizable(fitToWindow);
   if (!fitToWindow) {
      normalSize();
   }
   updateActions();
   */
}


//////////////////////////////////////////////////////////////////////////
void ImageViewer::wheelEvent( QWheelEvent * event )
{

   qreal factor = qPow(1.2, event->delta() / 240.0);
   _pGraphicsView->scale(factor, factor);

   std::cout << factor << std::endl;

   event->accept();


}

//////////////////////////////////////////////////////////////////////////
ImageViewer::~ImageViewer()
{
   _pGrid->ResetGrid();
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::step()
{
   // update cells with the algorithm
   _pGOL->Step();
   // repaint the scene
   _pScene->update(_pGraphicsView->sceneRect());

   const GameOfLife::LifeHistory& lifeHistory = _pGOL->GetLifeHistory();

   std::stringstream ss;
   unsigned int gen = lifeHistory.size() -1;

   ss << gen;
   _pMainWindow->genCounter->setText(QString::fromStdString(ss.str()));
    std::stringstream ss2;
   ss2 << lifeHistory[gen];
   _pMainWindow->aliveCounter->setText(QString::fromStdString(ss2.str()));
}


//////////////////////////////////////////////////////////////////////////
void ImageViewer::saveGrid()
{
   QString fileName = QFileDialog::getSaveFileName(this,
                                                   tr("Save File"), 
                                                   QDir::currentPath());
   if (!fileName.isEmpty()) 
   {
      _pGrid->SaveToFile(fileName.toStdString());
   }

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::loadGrid()
{
   QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open File"), 
                                                   QDir::currentPath());
   if (!fileName.isEmpty()) 
   {
      _pGrid->LoadFromFile(fileName.toStdString());
      _pGOL->SetGrid(_pGrid);
      // reset history counter
      _pMainWindow->genCounter->setText("0");
      _pMainWindow->aliveCounter->setText("0");
    
      _lastFilename= fileName;
   }

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::saveImageGridOnly()
{
   QString fileName = QFileDialog::getSaveFileName(this,
                                                   tr("Save File"), 
                                                   QDir::currentPath());
   if (fileName.isEmpty()) 
   { 
      std::cout << "Cannot save to an empty filename!" << std::endl;
      return;
   }

   fileName.append(".png");

   if (_pImageItem != NULL)
      _pImageItem->setVisible(false);   

   QImage img(_pScene->sceneRect().width(),_pScene->sceneRect().height(),QImage::Format_ARGB32_Premultiplied);
   QPainter p(&img);
   _pScene->render(&p);
   p.end();
   img.save(fileName);

   if (_pImageItem != NULL)
      _pImageItem->setVisible(true);

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::reloadGrid()
{
   if (!_lastFilename.isEmpty()) 
   {
      _pGrid->LoadFromFile(_lastFilename.toStdString());
      _pGOL->SetGrid(_pGrid);
      // reset history counter
      _pMainWindow->genCounter->setText("0");
      _pMainWindow->aliveCounter->setText("0");
   }

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::exportLifeHistory()
{

   QString fileName = QFileDialog::getSaveFileName(this,
                                                   tr("Save File"), 
                                                   QDir::currentPath());
   if (fileName.isEmpty()) 
   { 
      std::cout << "Cannot save to an empty filename!" << std::endl;
      return;
   }

   _pGOL->ExportHistory(fileName.toStdString());

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::play()
{
   if (_timer.isActive())
      return;

   // TODO read from interface
   _timer.setInterval(_pMainWindow->timeInterval->value());
   _timer.start();
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::pause()
{
   _timer.stop();
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::saveImage()
{
   QString fileName = QFileDialog::getSaveFileName(this,
      tr("Save File"), 
      QDir::currentPath());
   if (fileName.isEmpty()) 
   { 
      std::cout << "Cannot save to an empty filename!" << std::endl;
      return;
   }

   fileName.append(".png");


   QImage img(_pScene->sceneRect().width(),_pScene->sceneRect().height(),QImage::Format_ARGB32_Premultiplied);
   QPainter p(&img);
   _pScene->render(&p);
   p.end();
   img.save(fileName);

}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::markPath()
{
   _pGOL->SetUseMarkPath(_pMainWindow->markPathCheckBox->isChecked());
}
