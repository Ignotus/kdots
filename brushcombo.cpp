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
#include "brushcombo.hpp"

#include <QPaintEvent>
#include <QStylePainter>

#include "brushcombodelegate.hpp"

namespace KDots
{
  BrushCombo::BrushCombo(QWidget* parent)
    : KComboBox(parent)
  {
    for (int i = 0; i < 15; ++i)
      addItem(QString::number(i));
    
    setItemDelegate(new BrushComboDelegate(this));
  }
  
  void BrushCombo::paintEvent(QPaintEvent*)
  {
    QStylePainter painter(this);
    QStyleOptionComboBox opt;
    initStyleOption(&opt);
    painter.drawComplexControl(QStyle::CC_ComboBox, opt);

    const QRect& rect = painter.style()->subControlRect(QStyle::CC_ComboBox,
        &opt, QStyle::SC_ComboBoxEditField, this);
    
    QPixmap pixmap(rect.size() - QSize(4, 4));
    QPainter pixPainter(&pixmap);
    pixPainter.setRenderHint(QStylePainter::Antialiasing);
    pixPainter.fillRect(pixmap.rect(), Qt::white);
    pixPainter.fillRect(pixmap.rect(),
        BrushComboDelegate::brushes()[currentIndex()]);
    pixPainter.drawRect(pixmap.rect());
    
    QPainter genPainter(this);
    genPainter.drawPixmap(rect.x() + 2, rect.y() + 2, pixmap);
  }
}
