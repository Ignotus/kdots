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
#ifndef KDOTS_IPLUGIN_HPP
#define KDOTS_IPLUGIN_HPP
#include <QString>
#include <QIcon>
#include <QtPlugin>
#include <kexportplugin.h>
#include <kpluginfactory.h>
#include <point.hpp>


class QDialog;

namespace KDots
{
	class DotTable;
	class IRival;

	class IPlugin : public QObject
	{
		Q_OBJECT
	public:
		IPlugin (QObject *parent = 0)
			: QObject (parent)
		{
		}
		
		virtual ~IPlugin () {}

		/** @brief Returns a plugin name.
		*/
		virtual QString name () const = 0;

		/** @brief Returns a plugin description.
		*/
		virtual QString description () const = 0;

		virtual IRival* createRival () = 0;

		/** @brief Returns a plugin icon.
		*/
		virtual QIcon icon () const
		{
			return QIcon ();
		}
	};

}

#define KDOTS_EXPORT_PLUGIN( libname, classname )             \
    K_PLUGIN_FACTORY(factory, registerPlugin<classname>();) \
        K_EXPORT_PLUGIN(factory(#libname))

Q_DECLARE_INTERFACE (KDots::IPlugin, "com.github.ignotus.kdots.IPlugin/1.0");
#endif
