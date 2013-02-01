#include <QLayout>
#include <QWidget>

#include "iplugin.h"

IPlugin::IPlugin(QObject *parent, const QVariantList&)
: QObject(parent) {
}

IPlugin::~IPlugin() {}

void IPlugin::placeWidgetToOwner(QWidget *w, QObject *owner) {
  if (owner == NULL)
    return;
  
  QLayout *layout = qobject_cast<QLayout*>(owner);
  
  if (layout) {
    layout->addWidget(w);
  } else {
    QWidget *widget = qobject_cast<QWidget*>(owner);
    if (widget)
      w->setParent(widget);
  }
}

void IPlugin::configure(QObject *owner) {
}

void IPlugin::completeConfiguring() {
}