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
#include "gameconfig.hpp"

#include <QDataStream>

QDataStream& operator<<(QDataStream& out, const KDots::GameConfig& obj)
{
  out << (quint32) obj.m_firstOwner << (quint32) obj.m_height << (quint32) obj.m_width << (quint32) obj.m_mode;
  return out;
}

QDataStream& operator>>(QDataStream& in, KDots::GameConfig& obj)
{
  quint32 owner, mode;
  in >> owner >> (quint32&) obj.m_height >> (quint32&) obj.m_width >> mode;
  obj.m_firstOwner = static_cast<KDots::Owner>(owner);
  obj.m_mode = static_cast<KDots::GameMode>(mode);
  return in;
}

namespace KDots
{
  GameConfig::GameConfig()
    : m_width(-1)
  {}
    
  bool GameConfig::isInititialized() const
  {
    return m_width != -1;
  }
  
  void GameConfig::registerMeta()
  {
    qRegisterMetaType<KDots::GameConfig>("GameConfig");
    qRegisterMetaTypeStreamOperators<KDots::GameConfig>("GameConfig");
  }
}
