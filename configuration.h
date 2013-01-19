#pragma once
#include <QColor>

class Configuration {
  public:
    static Configuration& instance();
    
    QColor pointColor(int owner) const;
    
  private:
    Configuration();
    Configuration(const Configuration& other);
    ~Configuration();
    
};