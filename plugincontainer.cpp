/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "plugincontainer.hpp"
#include <QDebug>
#include <QDir>
#include <QApplication>
#include <QPluginLoader>
#include "interface/iplugin.hpp"
#include "constants.hpp"

namespace KDots
{
  PluginContainer&
  PluginContainer::instance ()
  {
    static PluginContainer obj;
    return obj;
  }
  
  void
  PluginContainer::loadPlugins ()
  {
    //TODO: Plugins must be situated in the system unix directory
    QDir pluginsDir (qApp->applicationDirPath ());
    pluginsDir.cd ("plugins");
    qDebug () << Q_FUNC_INFO << "Loading plugins..."; 
    for (const QString& fileName : pluginsDir.entryList ({PLUGIN_SUFFIX + "*"}, QDir::Files))
      {
        QPluginLoader pluginLoader (pluginsDir.absoluteFilePath (fileName));
        IPlugin *iplugin = qobject_cast<IPlugin*> (pluginLoader.instance ());
        if (iplugin)
          {
            qDebug () << Q_FUNC_INFO << "Loading the plugin:" << iplugin->name ();
            m_pluginMap.insert (iplugin->name (), iplugin);
          }
        else
          {
            qDebug () << Q_FUNC_INFO << pluginLoader.errorString ();
            qDebug () << Q_FUNC_INFO << "Cannot load the plugin " << fileName;
          }
      }
  }
}
