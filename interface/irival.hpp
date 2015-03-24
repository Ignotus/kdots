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
#include <QObject>
#include <constants.hpp>

class QStatusBar;
class KgDifficultyLevel;

namespace KDots
{
  class BoardModel;
  class Point;
  class GameConfig;
  class IConfigurationWidget;
  
  class IRival : public QObject
  {
  public:
    IRival(QObject *parent = 0)
      : QObject(parent)
    {
    }
    
    virtual ~IRival()
    {
    }
    
    virtual IConfigurationWidget* configureWidget()
    {
      return nullptr;
    }
    
    virtual void setBoardModel(BoardModel *board)
    {
      Q_UNUSED(board);
    }
    
    virtual void requestGameConfig()
    {
    }
    
    virtual Owner owner() const
    {
      return Owner::NONE;
    }
    
    virtual bool canUndo() const
    {
      return false;
    }
    
  public: //slots
    virtual void onPointAdded(const Point& point) = 0;
    virtual void onDifficultyChanged(const KgDifficultyLevel *difficulty) = 0;
    
  protected: //signals
    virtual void needCreateBoard(const GameConfig& config) = 0;
    virtual void needDestroy() = 0;
    virtual void needAddPoint(const Point&) = 0;
  };
}

Q_DECLARE_INTERFACE(KDots::IRival, "com.github.ignotus.kdots.IRival/1.0.1")