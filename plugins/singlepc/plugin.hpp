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
#pragma once
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>

namespace KDots
{
namespace singlepc
{
  class Rival : public KDots::IRival
  {
    Q_OBJECT
    Q_INTERFACES(KDots::IRival)
  public:
    Rival(QObject *parent = 0);
    
    bool canUndo() const;

  public Q_SLOTS:
    void onPointAdded(const Point& point);
    void onDifficultyChanged(const KgDifficultyLevel *difficulty);

  Q_SIGNALS:
    void needCreateBoard(const GameConfig& config);
    void needDestroy();
    void needAddPoint(const Point&);
  };

  class Plugin : public KDots::IPlugin
  {
    Q_OBJECT
    Q_INTERFACES(KDots::IPlugin)
    Q_PLUGIN_METADATA(IID "com.github.ignotus.kdots.IPlugin" FILE "")
  public:
    Plugin(QObject *parent = 0);
    std::unique_ptr<IRival> createRival();
    QString name() const;
    QString description() const;
    QIcon icon() const;
  };

}
}