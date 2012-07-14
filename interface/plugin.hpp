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
#ifndef KDOTS_PLUGIN_HPP
#define KDOTS_PLUGIN_HPP
#include <memory>
#include <QString>
#include <QIcon>
#include <point.hpp>
#include <dottable.hpp>

class QWidget;

namespace KDots
{
  
  class IPlugin
  {
  public:
    virtual void init (DotTable *table) = 0;
    virtual ~IPlugin () {}
    
    /** @brief Returns a configuration widget for the current plugin.
     */
    virtual QWidget* configurationWidget () = 0;
    
    /** @brief Returns a plugin name.
     */
    virtual QString name () const = 0;
    
    /** @brief Returns a plugin description.
     */
    virtual QString description () const = 0;
    
    /** @brief Returns a plugin icon.
     */
    virtual QIcon icon () const
    {
      return QIcon ();
    }
  };
  
  Q_DECLARE_INERFACE (IPlugin, "com.github.ignotus.kdots.IPlugin/1.0");
  
}

#endif