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
#ifndef KDOTS_PLUGINS_SINGLEPC_PLUGIN_HPP
#define KDOTS_PLUGINS_SINGLEPC_PLUGIN_HPP
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>

namespace KDots
{
	namespace singlepc
	{
		class Rival : public KDots::IRival
		{
			Q_OBJECT
		public:
			Rival (QObject *parent = 0)
				: IRival (parent)
			{
			}

			bool isAllow () const
			{
				return true;
			}

		public slots:
			void nextStep (const Point& point)
			{
			}
			
			void ready ()
			{
			}
		};

		class Plugin : public KDots::IPlugin
		{
			Q_OBJECT
			Q_INTERFACES (KDots::IPlugin)
		public:
			Plugin (QObject *parent = 0)
				: IPlugin (parent)
			{
			}

			IRival* createRival ()
			{
				return new Rival;
			}

			QString name () const
			{
				return "singlepc";
			}

			QString description () const
			{
				return "Playing in the single PC";
			}
		};

	}
}

#endif
