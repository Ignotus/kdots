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

#include <enums.hpp>

class QStatusBar;
class KgDifficultyLevel;

namespace KDots
{
  class BoardModel;
  class Point;
  class GameConfig;
  class IConfigurationWidget;
  
  /*
   * KDots board opponent interface
   */
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
    
    // Returns a configuration widget if it's needed to configure a game
    // in addition
    virtual IConfigurationWidget* configureWidget()
    {
      return nullptr;
    }
    
    // Sets the current game board
    virtual void setBoardModel(const BoardModel *board)
    {
      Q_UNUSED(board);
    }
    
    // This method is called if KDots requires game configuration from
    // the plugin. This configuration can be returned later by the signal
    // needCreateBoard()
    virtual void requestGameConfig()
    {
    }
    
    // Returns the class owner
    virtual Owner owner() const
    {
      return Owner::NONE;
    }
    
    // Determines if in this game mode it's possible to undo moves
    virtual bool canUndo() const
    {
      return false;
    }
    
  public: //slots
    // This slot is called after the appearance of the new point on the board
    virtual void onPointAdded(const Point& point) = 0;
    
    // This slot is called after the difficulty level changed.
    // Please disable difficulty changes in the class constructor if it's not needed.
    virtual void onDifficultyChanged(const KgDifficultyLevel *difficulty) = 0;
    
  protected: //signals
    // Emit this signal if a new board with following configuration is needed to be
    // created.
    virtual void needCreateBoard(const GameConfig& config) = 0;
    
    // Emit this signal if the current game is needed to be destroyed
    virtual void needDestroy() = 0;
    
    // Emit this signal to add a new point to the game board
    virtual void needAddPoint(const Point&) = 0;
  };
}

Q_DECLARE_INTERFACE(KDots::IRival, "com.github.ignotus.kdots.IRival/1.0.1")