#include <QDebug>
#include "newgamedialog.h"
#include "ui_newgamedialog.h"
#include "pluginfactory.h"
#include "pluginfactoryview.h"
#include "newgamefactory.h"
#include "interfaces/iplugin.h"

NewGameDialog::NewGameDialog(QWidget *parent)
: QDialog(parent)
, m_ui(new Ui::NewGameDialog) {
  m_ui->setupUi(this);
}

void NewGameDialog::setPluginFactory(PluginFactory *factory) {
  PluginFactoryView *factoryView = new PluginFactoryView(this);
  factoryView->setModel(factory);
  m_ui->grid->addWidget(factoryView, 0, 0);
  
  connect(m_ui->nextButton, SIGNAL(clicked(bool)), factory, SLOT(emitCurrentPlugin())); 
  connect(factory, SIGNAL(pluginSet(IPlugin*)), this, SLOT(onPluginSet(IPlugin*)));
  connect(this, SIGNAL(hidePluginFactory()), factoryView, SLOT(hide()));
  
  m_ui->nextButton->setEnabled(true);
  m_ui->okButton->setEnabled(false);
}

void NewGameDialog::onPluginSet(IPlugin *plugin) {
  m_ui->nextButton->setEnabled(true);
  m_ui->okButton->setEnabled(false);
  
  disconnect(m_ui->nextButton, SIGNAL(clicked(bool)));
  disconnect(this, SLOT(onPluginSet(IPlugin*)));
  
  connect(m_ui->nextButton, SIGNAL(clicked(bool)), plugin, SLOT(completeConfiguring()));
  connect(m_ui->okButton, SIGNAL(clicked(bool)), plugin, SLOT(completeConfiguring()));
  
  connect(plugin, SIGNAL(createTable()), this, SLOT(showCreateTableWidget()));
  connect(plugin, SIGNAL(createTableByConfig(TableConfig)), this, SIGNAL(createTable(TableConfig)));
  
  plugin->configure(m_ui->grid);
}

void NewGameDialog::showCreateTableWidget() {
  m_ui->nextButton->setEnabled(false);
  m_ui->okButton->setEnabled(true);
  
  disconnect(m_ui->nextButton, SIGNAL(clicked(bool)));
  disconnect(m_ui->okButton, SIGNAL(clicked(bool)));
  disconnect(this, SLOT(showCreateTableWidget()));
  disconnect(this, SIGNAL(createTable(TableConfig)));
  
  NewGameFactory *factory = new NewGameFactory(this);
  emit hidePluginFactory();
  
  connect(m_ui->okButton, SIGNAL(clicked(bool)), factory, SLOT(emitTableConfig()));
  connect(factory, SIGNAL(createTable(TableConfig)), this, SIGNAL(createTable(TableConfig)));
  
  m_ui->grid->addWidget(factory, 0, 0);
}

NewGameDialog::~NewGameDialog() {
  delete m_ui;
}