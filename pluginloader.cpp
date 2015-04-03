/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "pluginloader.hpp"

#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include <QDebug>

#include <interface/iplugin.hpp>

#include <config.hpp>
#include <constants.hpp>

namespace KDots
{
  PluginLoader::PluginLoader()
  {
    loadPlugins();
  }
  
  PluginLoader& PluginLoader::instance()
  {
    static PluginLoader obj;
    return obj;
  }
  
  const QSet<QString>& PluginLoader::availablePlugins() const
  {
    return m_availablePlugins;
  }

  IPlugin* PluginLoader::plugin(const QString& name)
  {
    auto itr = m_pluginMap.find(name);
    if (itr == m_pluginMap.end())
      return nullptr;
    
    return *itr;
  }
  
  bool PluginLoader::findPlugin(const QDir& pluginsDir)
  {
    bool foundFlag = false;
    for (const QString& fileName : pluginsDir.entryList({PLUGIN_SUFFIX + QLatin1String("*")}, QDir::Files))
    {
      QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
      IPlugin *iplugin = qobject_cast<IPlugin *>(pluginLoader.instance());

      if (iplugin)
      {
        foundFlag = true;
        qDebug() << "Loading the plugin:" << iplugin->name();
        m_pluginMap.insert(iplugin->name(), iplugin);
        m_availablePlugins.insert(iplugin->name());
      }
      else
      {
        qWarning() << pluginLoader.errorString();
        qWarning() << "Cannot load the plugin " << fileName;
      }
    }
    
    return foundFlag;
  }

  void PluginLoader::loadPlugins()
  {
    QDir libdir(QLatin1String(PLUGINS_DIR));
    if (!libdir.exists() || !findPlugin(libdir))
      qWarning() << "Plugins not found in " << libdir.absolutePath();
  }
}
