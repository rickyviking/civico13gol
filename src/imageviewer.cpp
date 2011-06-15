


#include <QtGui/QtGui>

#include <imageviewer.h>

ImageViewer::ImageViewer()
{
   // create main window
   _pMainWindow = new Ui::MainWindow;
   _pMainWindow->setupUi(this);

   // connect from menu
   connect(_pMainWindow->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
   connect(_pMainWindow->actionReset_Zoom, SIGNAL(triggered()), this, SLOT(normalSize()));

   connect(_pMainWindow->createGrid, SIGNAL(pressed()), this, SLOT(createGrid()));
   connect(_pMainWindow->clearGrid, SIGNAL(pressed()), this, SLOT(resetGrid()));

   // initialized a caso - resize the scene rect when loading an image
   _pScene = new QGraphicsScene(0, 0, 800, 500);

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
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::open()
//! [1] //! [2]
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
      
      QGraphicsPixmapItem* pImageItem = new QGraphicsPixmapItem;
      pImageItem->setPixmap(QPixmap::fromImage(image));
      // use 0 as background level...
      pImageItem->setZValue(-1);
      _pScene->addItem(pImageItem);

      _pScene->setSceneRect(pImageItem->boundingRect());
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
}


//////////////////////////////////////////////////////////////////////////
void ImageViewer::resetGrid()
{
   std::cout << "reset the grid"<< std::endl;
   _pGrid->ResetGrid();

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
   delete _pGrid;

}



//! [26]
