/*
 * KDots
 * Copyright(c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
 *(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "pluginwidgetdelegate.hpp"
#include <QPainter>
#include <KApplication>
#include <KDebug>
#include "plugincontainer.hpp"
#include "interface/iplugin.hpp"

namespace KDots
{
  PluginWidgetDelegate::PluginWidgetDelegate(QObject *parent)
    : QItemDelegate(parent)
  {
    
  }
  
  void PluginWidgetDelegate::updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem& option, const QModelIndex&) const
  {
    editor->setGeometry(option.rect);
  }
  
  void PluginWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option,
      const QModelIndex& index) const
  {
    QStyleOptionViewItemV4 myOption = option;
    KApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    const QString& pluginName = index.data().toString();
    
    const QRect textRect(option.rect.x(), option.rect.y() + 30,
        option.rect.width(), option.rect.height() - 30);
    
    painter->drawText(textRect, Qt::AlignCenter, pluginName);
    IPlugin *plug = PluginContainer::instance().plugin(pluginName);
    if(plug)
    {
      const KIcon& plugIcon = plug->icon();
      if(plugIcon.isNull())
      {
        const KIcon newIcon("applications-boardgames");
        drawIcon(painter, option, newIcon);
      }
      else
        drawIcon(painter, option, plugIcon);
    }
  }
  
  void PluginWidgetDelegate::drawIcon(QPainter *painter,
      const QStyleOptionViewItem& option, const KIcon& icon) const
  {
    const int delta =(option.rect.width() - 32) / 2;
    const QRect iconRect(option.rect.x() + delta, option.rect.y() + 2, 28, 28);
    const QPixmap& pixMap = icon.pixmap(iconRect.size());
    painter->drawPixmap(iconRect, pixMap, pixMap.rect());
  }
}
