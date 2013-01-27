#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablemodel.h"
#include "tableview.h"
#include "ownerdetector.h"
#include "polygondfsfinder.h"
#include "pointcounter.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, m_ui(new Ui::MainWindow) {
  m_ui->setupUi(this);
}

MainWindow::~MainWindow() {
}

void MainWindow::init() {
  TableView *view = new TableView(this);
  TableModel *model = new TableModel(QSize(10, 10), view);
  
  model->setOwnerDetector(new AdditionalMove(3, 1));
  
  model->setPolygonFinder(new PolygonDFSFinder(model->data()));
  
  view->setModel(model);
  
  view->show();
  
  PointCounter *counter = new PointCounter(model->ownerCount(), view);
  
  connect(model, SIGNAL(pointCaptured(int, int)), counter, SLOT(onPointCaptured(int, int)));
  
  setCentralWidget(view);
}