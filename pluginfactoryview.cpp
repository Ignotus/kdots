#include "pluginfactoryview.h"
#include "ui_pluginfactoryview.h"
#include "plugindelegate.h"
#include "pluginfactory.h"

PluginFactoryView::PluginFactoryView(QWidget *parent)
: QWidget(parent)
, m_ui(new Ui::PluginFactoryView)
, m_factory(NULL) {
  m_ui->setupUi(this);
}

PluginFactoryView::~PluginFactoryView() {
  delete m_ui;
}

void PluginFactoryView::setModel(PluginFactory *factory) {
  m_factory = factory;
  
  QListView *list = m_ui->pluginList;
  list->setItemDelegateForColumn(0, new PluginDelegate(factory, this));
  list->setModel(factory->itemModel(this));
  list->setSelectionMode(QAbstractItemView::SingleSelection);
  list->setEditTriggers(QAbstractItemView::NoEditTriggers);
  
  connect(list, SIGNAL(clicked(QModelIndex)), this, SLOT(onIndexChanged(QModelIndex)));
  connect(this, SIGNAL(onCurrentPluginChanged(QString)), factory, SLOT(setCurrentPlugin(QString)));
  
    //const QModelIndex& index = model->index(Settings::lastPlugin(), 0);
    //m_ui->PluginList->selectionModel()->select(index, QItemSelectionModel::Select);
    //onIndexChanged(index);
}

void PluginFactoryView::onIndexChanged(const QModelIndex& current) {
    const QString& pluginName = current.data().toString();
    
    KService::Ptr data = m_factory->info(pluginName);
    if (!data.isNull()) {
      m_ui->description->setText(data->comment());
    }
    
    emit onCurrentPluginChanged(pluginName);
}
  

