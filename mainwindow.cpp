#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablemodel.h"
#include "tableview.h"
#include "ownerdetector.h"
#include "polygondfsfinder.h"
#include "pointcounter.h"
#include "pluginfactory.h"
#include "pluginfactoryview.h"
#include "newgamedialog.h"
#include "interfaces/iplugin.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, m_ui(new Ui::MainWindow)
, m_loader(new PluginFactory(this)) {
  m_ui->setupUi(this);
  
  connect(m_ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(init()));
  
  m_loader->loadPlugins();
  
  connect(m_loader, SIGNAL(pluginSet(IPlugin*)), this, SLOT(onPluginSet(IPlugin*)));
}

MainWindow::~MainWindow() {
  delete m_ui;
}

OwnerDetector* MainWindow::createOwnerDetector(int type, int firstPlayer) const {
  switch (type) {
    case WITH_ADDITIONAL_MOVE:
      return new AdditionalMove(2, firstPlayer);
    case WITHOUT_ADDITIONAL_MOVE:
      return new WithoutAdditionalMove(2, firstPlayer);
  }
  
  return NULL;
}

void MainWindow::onCreateTable(const TableConfig& config) {
  TableView *view = new TableView(this);
  TableModel *model = new TableModel(config.size, view);
  
  model->setOwnerDetector(createOwnerDetector(config.gameType, config.firstPlayer));
  
  model->setPolygonFinder(new PolygonDFSFinder(model->data()));
  
  view->setModel(model);
  
  view->show();
  
  PointCounter *counter = new PointCounter(model->ownerCount(), view);
  
  connect(model, SIGNAL(pointCaptured(int, int)), counter, SLOT(onPointCaptured(int, int)));
  
  setCentralWidget(view);
}

void MainWindow::onPluginSet(IPlugin *plugin) {
  m_plugin.reset(plugin);
}

void MainWindow::init() {
  NewGameDialog w;
  
  connect(&w, SIGNAL(createTable(TableConfig)), this, SLOT(onCreateTable(TableConfig)));
  
  w.setPluginFactory(m_loader);
  
  w.exec();
  

  

}