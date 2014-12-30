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
#ifndef KDOTS_PLUGINS_SIMPLEAI_PLUGIN_HPP
#define KDOTS_PLUGINS_SIMPLEAI_PLUGIN_HPP
#include <interface/iplugin.hpp>
#include "rival.hpp"

namespace KDots
{
  namespace simpleai
  {
    class Plugin : public KDots::IPlugin
    {
      Q_OBJECT
      Q_INTERFACES(KDots::IPlugin)
    public:
      Plugin(QObject *parent = 0)
        : IPlugin(parent)
      {
      }

      std::shared_ptr<IRival> createRival()
      {
        return std::shared_ptr<IRival>(new Rival);
      }

      QString name() const
      {
        return "simpleai";
      }

      QString description() const
      {
        return "Playing with the simple AI";
      }
      
      KIcon icon() const
      {
        return KIcon("games-config-board");
      }
    };

  }
}

#endif
