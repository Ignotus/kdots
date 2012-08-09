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
#include "config.hpp"

namespace KDots
{
	PluginContainer& PluginContainer::instance ()
	{
		static PluginContainer obj;
		return obj;
	}
	
	bool PluginContainer::findPlugin (const QDir& pluginsDir)
	{
		bool foundFlag = false;
		for (const QString& fileName : pluginsDir.entryList ({PLUGIN_SUFFIX + "*"}, QDir::Files))
		{
			QPluginLoader pluginLoader (pluginsDir.absoluteFilePath (fileName));
			IPlugin *iplugin = qobject_cast<IPlugin *> (pluginLoader.instance ());

			if (iplugin)
			{
				foundFlag = true;
				qDebug () << Q_FUNC_INFO << "Loading the plugin:" << iplugin->name ();
				m_pluginMap.insert (iplugin->name (), iplugin);
			}
			else
			{
				qDebug () << Q_FUNC_INFO << pluginLoader.errorString ();
				qDebug () << Q_FUNC_INFO << "Cannot load the plugin " << fileName;
			}
		}
		
		return foundFlag;
	}

	void PluginContainer::loadPlugins ()
	{
		QDir currentDir (qApp->applicationDirPath ());
		if (!currentDir.cd ("plugins") || !findPlugin (currentDir))
		{
#ifdef Q_OS_UNIX
			QDir libdir (PLUGINS_DIR);
			if (!libdir.exists () || !findPlugin (libdir))
				qDebug () << Q_FUNC_INFO << "Plugins not found in " << libdir.absolutePath ();
#else
			qDebug () << Q_FUNC_INFO << "Plugins not found";
#endif	
		}

		
	}
}
