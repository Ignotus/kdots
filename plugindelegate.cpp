#include <QPainter>
#include <QPixmap>

#include <KIcon>
#include <KApplication>
#include <KDebug>

#include "plugindelegate.h"
#include "pluginfactory.h"

PluginDelegate::PluginDelegate(PluginFactory *factory, QObject *parent)
: QItemDelegate(parent)
, m_pluginFactory(factory) {
  
}

void PluginDelegate::updateEditorGeometry(QWidget *editor,
                                          const QStyleOptionViewItem& option,
                                          const QModelIndex&) const {
  editor->setGeometry(option.rect);
}

QSize PluginDelegate::sizeHint(const QStyleOptionViewItem& option,
                               const QModelIndex& index) const {
  Q_UNUSED(index);
  return QSize(option.rect.width(), 50);
}

void PluginDelegate::paint(QPainter *painter,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const {
  QStyleOptionViewItemV4 myOption = option;
  KApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
  const QString& pluginName = index.data().toString();
 
  const int padding = 30;
  const QRect textRect(option.rect.x(), option.rect.y() + padding,
                       option.rect.width(), option.rect.height() - padding);
                        
  painter->drawText(textRect, Qt::AlignCenter, pluginName);
  
  const KService::Ptr service = m_pluginFactory->info(pluginName);
  
  if (service.isNull())
    return;
  
  const QString& plugIcon = service->icon();
  if (plugIcon.isNull()) {
    const KIcon newIcon("applications-boardgames");
    drawIcon(painter, option, newIcon);
  } else {
    drawIcon(painter, option, KIcon(plugIcon));
  }
}

void PluginDelegate::drawIcon(QPainter *painter,
                              const QStyleOptionViewItem& option,
                              const KIcon& icon) const {
  const int doublePadding = 32;
  const int iconSize = 28;
  const int x = option.rect.x() + (option.rect.width() - doublePadding) / 2;
  const int y = option.rect.y() + (doublePadding - iconSize) / 2;
  
  const QRect iconRect(x, y, iconSize, iconSize);
  const QPixmap& pixMap = icon.pixmap(iconRect.size());
  painter->drawPixmap(iconRect, pixMap, pixMap.rect());
}
