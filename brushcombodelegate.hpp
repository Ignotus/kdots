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
#ifndef KDOTS_BRUSHCOMBODELEGATE_HPP
#define KDOTS_BRUSHCOMBODELEGATE_HPP
#include <QStyledItemDelegate>
#include <QVector>

namespace KDots
{
  class BrushComboDelegate : public QStyledItemDelegate
  {
  public:
    static const QVector<Qt::BrushStyle>& brushes();
    static Qt::BrushStyle getBrushStyle(int index);
    
    BrushComboDelegate(QWidget* parent = 0);
    
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& option,
        const QModelIndex& index) const;
    void setEditorData(QWidget *editor, const QModelIndex& index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
        const QModelIndex& index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem& option,
        const QModelIndex& index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem& option,
        const QModelIndex& index) const;
    QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
  };
}

#endif