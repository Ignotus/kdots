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
#ifndef KDOTS_PLUGINCONTAINER_HPP
#define KDOTS_PLUGINCONTAINER_HPP
#include <QMap>

class QDir;

namespace KDots
{
	class IPlugin;
	class PluginContainer
	{
		typedef QMap<QString, IPlugin*> PluginMap;
		PluginMap m_pluginMap;
	public:
		PluginContainer ()
		{
			loadPlugins ();
		}

		static PluginContainer& instance ();

		inline const PluginMap& plugins () const
		{
			return m_pluginMap;
		}

		inline IPlugin* plugin (const QString& name)
		{
			PluginMap::const_iterator itr = m_pluginMap.find (name);
			return itr != m_pluginMap.end () ? *itr : NULL;
		}

	private:
		void loadPlugins ();
		bool findPlugin (const QDir& dir);
	};
}

#endif
