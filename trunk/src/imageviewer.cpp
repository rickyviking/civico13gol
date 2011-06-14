


#include <QtGui/QtGui>

#include <imageviewer.h>

ImageViewer::ImageViewer()
{
   createActions();
   createMenus();



   // try to add a graphics scene+window with 4 squares 
   // in overlay to the image

   Cell *button1 = new Cell;
   Cell *button2 = new Cell;
   Cell *button3 = new Cell;
   Cell *button4 = new Cell;
   button2->setZValue(1);
   button2->setZValue(2);
   button3->setZValue(3);
   button4->setZValue(4);

   button1->setGeometry(0, 0, 50, 50);
   button2->setGeometry(300, 0, 50, 50);
   button3->setGeometry(0, 300, 50, 50);
   button4->setGeometry(300, 480, 50, 50);

   // initialized a caso - resize the scene rect when loading an image
   _pScene = new QGraphicsScene(0, 0, 800, 500);

   //scene.setBackgroundBrush(Qt::black);
   _pScene->addItem(button1);
   _pScene->addItem(button2);
   _pScene->addItem(button3);
   _pScene->addItem(button4);

   _pGraphicsView = new QGraphicsView(_pScene);
   _pGraphicsView->setFrameStyle(0);
   _pGraphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);
   _pGraphicsView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
   //_pGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   //_pGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   _pGraphicsView->setRenderHint(QPainter::Antialiasing);
   //! [4] //! [5]
   //_pGraphicsView->setCacheMode(QGraphicsView::CacheBackground);
   //_pGraphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
   _pGraphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
   //! [5] //! [6]
   _pGraphicsView->setWindowTitle(QT_TRANSLATE_NOOP(QGraphicsView, "Colliding Mice"));
   //view.resize(400, 300);
   //view.show();

   setCentralWidget(_pGraphicsView);




   setWindowTitle(tr("Civico13-GOL"));
   resize(400, 300);
}
//! [0]

//! [1]
void ImageViewer::open()
//! [1] //! [2]
{
   QString fileName = QFileDialog::getOpenFileName(this,
      tr("Open File"), QDir::currentPath());
   if (!fileName.isEmpty()) {
      QImage image(fileName);
      if (image.isNull()) {
         QMessageBox::information(this, tr("Image Viewer"),
            tr("Cannot load %1.").arg(fileName));
         return;
      }
      //! [2] //! [3]
      //imageLabel->setPixmap(QPixmap::fromImage(image));

      QGraphicsPixmapItem* pImageItem = new QGraphicsPixmapItem;
      pImageItem->setPixmap(QPixmap::fromImage(image));
      // use 0 as background level...
      pImageItem->setZValue(-1);
      _pScene->addItem(pImageItem);

      _pScene->setSceneRect(pImageItem->boundingRect());
      

      _pCreateGridAction->setEnabled(true);


#if 0
      scaleFactor = 1.0;

      printAct->setEnabled(true);
      fitToWindowAct->setEnabled(true);
      updateActions();

      if (!fitToWindowAct->isChecked())
         imageLabel->adjustSize();
#endif

   }
}
//! [4]

//! [5]
void ImageViewer::print()
//! [5] //! [6]
{
   Q_ASSERT(imageLabel->pixmap());
#ifndef QT_NO_PRINTER
   //! [6] //! [7]
   QPrintDialog dialog(&printer, this);
   //! [7] //! [8]
   if (dialog.exec()) {
      QPainter painter(&printer);
      QRect rect = painter.viewport();
      QSize size = imageLabel->pixmap()->size();
      size.scale(rect.size(), Qt::KeepAspectRatio);
      painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
      painter.setWindow(imageLabel->pixmap()->rect());
      painter.drawPixmap(0, 0, *imageLabel->pixmap());
   }
#endif
}
//! [8]

//! [9]
void ImageViewer::createGrid()
//! [9] //! [10]
{
   std::cout << "create the grid"<< std::endl;
}

void ImageViewer::zoomOut()
{
   scaleImage(0.8);
}

//! [10] //! [11]
void ImageViewer::normalSize()
//! [11] //! [12]
{
   imageLabel->adjustSize();
   scaleFactor = 1.0;
}
//! [12]

//! [13]
void ImageViewer::fitToWindow()
//! [13] //! [14]
{
   bool fitToWindow = fitToWindowAct->isChecked();
   scrollArea->setWidgetResizable(fitToWindow);
   if (!fitToWindow) {
      normalSize();
   }
   updateActions();
}
//! [14]


//! [15]
void ImageViewer::about()
//! [15] //! [16]
{
   QMessageBox::about(this, tr("About Image Viewer"),
      tr("<p>The <b>Image Viewer</b> example shows how to combine QLabel "
      "and QScrollArea to display an image. QLabel is typically used "
      "for displaying a text, but it can also display an image. "
      "QScrollArea provides a scrolling view around another widget. "
      "If the child widget exceeds the size of the frame, QScrollArea "
      "automatically provides scroll bars. </p><p>The example "
      "demonstrates how QLabel's ability to scale its contents "
      "(QLabel::scaledContents), and QScrollArea's ability to "
      "automatically resize its contents "
      "(QScrollArea::widgetResizable), can be used to implement "
      "zooming and scaling features. </p><p>In addition the example "
      "shows how to use QPainter to print an image.</p>"));
}
//! [16]

//! [17]
void ImageViewer::createActions()
//! [17] //! [18]
{
   openAct = new QAction(tr("&Open..."), this);
   openAct->setShortcut(tr("Ctrl+O"));
   connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

   printAct = new QAction(tr("&Print..."), this);
   printAct->setShortcut(tr("Ctrl+P"));
   printAct->setEnabled(false);
   connect(printAct, SIGNAL(triggered()), this, SLOT(print()));

   exitAct = new QAction(tr("E&xit"), this);
   exitAct->setShortcut(tr("Ctrl+Q"));
   connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

   _pCreateGridAction = new QAction(tr("&Create Grid"), this);
   _pCreateGridAction->setShortcut(tr("Ctrl+g"));
   _pCreateGridAction->setEnabled(false);
   connect(_pCreateGridAction, SIGNAL(triggered()), this, SLOT(createGrid()));

   zoomOutAct = new QAction(tr("Zoom &Out (25%)"), this);
   zoomOutAct->setShortcut(tr("Ctrl+-"));
   zoomOutAct->setEnabled(false);
   connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));

   normalSizeAct = new QAction(tr("&Normal Size"), this);
   normalSizeAct->setShortcut(tr("Ctrl+S"));
   normalSizeAct->setEnabled(false);
   connect(normalSizeAct, SIGNAL(triggered()), this, SLOT(normalSize()));

   fitToWindowAct = new QAction(tr("&Fit to Window"), this);
   fitToWindowAct->setEnabled(false);
   fitToWindowAct->setCheckable(true);
   fitToWindowAct->setShortcut(tr("Ctrl+F"));
   connect(fitToWindowAct, SIGNAL(triggered()), this, SLOT(fitToWindow()));

   aboutAct = new QAction(tr("&About"), this);
   connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

   aboutQtAct = new QAction(tr("About &Qt"), this);
   connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}
//! [18]

//! [19]
void ImageViewer::createMenus()
//! [19] //! [20]
{
   fileMenu = new QMenu(tr("&File"), this);
   fileMenu->addAction(openAct);
   fileMenu->addAction(printAct);
   fileMenu->addSeparator();
   fileMenu->addAction(exitAct);

   _pToolMenu = new QMenu(tr("&Tools"), this);
   _pToolMenu->addAction(_pCreateGridAction);   
   
   _pToolMenu->addSeparator();

   _pToolMenu->addAction(normalSizeAct);
   _pToolMenu->addAction(fitToWindowAct);

   helpMenu = new QMenu(tr("&Help"), this);
   helpMenu->addAction(aboutAct);
   helpMenu->addAction(aboutQtAct);

   menuBar()->addMenu(fileMenu);
   menuBar()->addMenu(_pToolMenu);
   menuBar()->addMenu(helpMenu);
}
//! [20]

//! [21]
void ImageViewer::updateActions()
//! [21] //! [22]
{
   _pCreateGridAction->setEnabled(!fitToWindowAct->isChecked());
   zoomOutAct->setEnabled(!fitToWindowAct->isChecked());
   normalSizeAct->setEnabled(!fitToWindowAct->isChecked());
}
//! [22]

//! [23]
void ImageViewer::scaleImage(double factor)
//! [23] //! [24]
{
   Q_ASSERT(imageLabel->pixmap());
   scaleFactor *= factor;
   imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

   adjustScrollBar(scrollArea->horizontalScrollBar(), factor);
   adjustScrollBar(scrollArea->verticalScrollBar(), factor);

   _pCreateGridAction->setEnabled(scaleFactor < 3.0);
   zoomOutAct->setEnabled(scaleFactor > 0.333);
}
//! [24]

//! [25]
void ImageViewer::adjustScrollBar(QScrollBar *scrollBar, double factor)
//! [25] //! [26]
{
   scrollBar->setValue(int(factor * scrollBar->value()
      + ((factor - 1) * scrollBar->pageStep()/2)));
}

//////////////////////////////////////////////////////////////////////////
void ImageViewer::wheelEvent( QWheelEvent * event )
{

   qreal factor = qPow(1.2, event->delta() / 240.0);
   _pGraphicsView->scale(factor, factor);
   event->accept();


}

//! [26]
