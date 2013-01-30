#pragma once
#include <QItemDelegate>

class QStandardItemModel;
class KIcon;
class PluginFactory;

class PluginDelegate : public QItemDelegate {
  public:
    PluginDelegate(PluginFactory *factory, QObject *parent = 0);
    
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const;
    
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const;
                              
    void paint(QPainter *painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const;
               
    QStandardItemModel *itemModel(QObject *parent = 0) const;
    
  private:
    void drawIcon(QPainter *painter,
                  const QStyleOptionViewItem& option,
                  const KIcon& icon) const;
                  
  private:
    PluginFactory * const m_pluginFactory;
};