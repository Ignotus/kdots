#include "configuration.h"

Configuration::Configuration() {
}

Configuration::~Configuration() {
}

Configuration& Configuration::instance() {
  static Configuration obj;
  return obj;
}
    
QColor Configuration::pointColor(int owner) const {
  //TODO: Move it to the configuration file
  switch (owner) {
    case 1:
      return QColor(Qt::red);
    case 2:
      return QColor(Qt::blue);
  }
  
  return QColor(Qt::black);
}

