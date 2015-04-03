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
#include "brushcombodelegate.hpp"

#include <QComboBox>
#include <QPainter>

#include <KApplication>

namespace KDots
{
  BrushComboDelegate::BrushComboDelegate(QWidget* parent)
    : QStyledItemDelegate(parent)
  {
  }
  
  const QVector<Qt::BrushStyle>& BrushComboDelegate::brushes()
  {
    static QVector<Qt::BrushStyle> brush = {
        Qt::NoBrush, Qt::Dense1Pattern, Qt::Dense2Pattern,
        Qt::Dense3Pattern, Qt::Dense4Pattern, Qt::Dense5Pattern,
        Qt::Dense6Pattern, Qt::Dense6Pattern, Qt::Dense7Pattern,
        Qt::HorPattern, Qt::VerPattern, Qt::CrossPattern,
        Qt::BDiagPattern, Qt::FDiagPattern, Qt::DiagCrossPattern
    };
        
    return brush;
  }
  
  Qt::BrushStyle BrushComboDelegate::getBrushStyle(int index)
  {
    return brushes()[index];
  }
  
  QWidget* BrushComboDelegate::createEditor(QWidget *parent,
      const QStyleOptionViewItem&, const QModelIndex&) const
  {
    QComboBox* editor = new QComboBox(parent);
    for (int i = 0, size = brushes().size(); i < size; ++i)
      editor->addItem(QString::number(i));
    
    return editor;
  }
  
  void BrushComboDelegate::setEditorData(QWidget *editor, const QModelIndex& index) const
  {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int value = index.model()->data(index, Qt::EditRole).toUInt();
    comboBox->setCurrentIndex(value);
  }
  
  void BrushComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
      const QModelIndex& index) const
  {
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    model->setData(index, comboBox->currentIndex(), Qt::EditRole);
  }
  
  void BrushComboDelegate::updateEditorGeometry(QWidget *editor,
      const QStyleOptionViewItem& option, const QModelIndex&) const
  {
    editor->setGeometry(option.rect);
  }
  
  void BrushComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem& option,
      const QModelIndex& index) const
  {
    QStyleOptionViewItemV4 myOption = option;
    const QRect& currentRect = myOption.rect;
    const QRect pixmapRect(QPoint(0, 0), currentRect.size() - QSize(4, 4));
    const QRect itemRect(currentRect.x() + 2, currentRect.y() + 2,
        currentRect.width() - 4, currentRect.height() - 4);
    
    QPixmap pixmap(pixmapRect.size());
    QPainter pixPainter(&pixmap);
    pixPainter.setRenderHint(QPainter::Antialiasing);
    pixPainter.fillRect(pixmap.rect(), Qt::white);
    pixPainter.fillRect(pixmapRect, brushes()[index.row()]);
    pixPainter.drawRect(pixmapRect);
 
    KApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
    painter->drawPixmap(itemRect, pixmap);
  }
  
  QSize BrushComboDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const
  {
    return QSize(60, 36);
  }
}
