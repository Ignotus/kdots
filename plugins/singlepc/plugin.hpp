/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <ignotusp@fedoraproject.org>
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
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef KDOTS_PLUGINS_SINGLEPC_PLUGIN_HPP
#define KDOTS_PLUGINS_SINGLEPC_PLUGIN_HPP
#include <kdemacros.h>
#include <interface/iplugin.hpp>
#include <interface/irival.hpp>

namespace KDots
{
	namespace singlepc
	{
		class KDE_EXPORT Rival : public KDots::IRival
		{
			Q_OBJECT
			Q_INTERFACES (KDots::IRival)
		public:
			Rival (QObject *parent = 0)
				: IRival (parent)
			{
			}

			bool isAllow () const
			{
				return true;
			}

		public slots:
			void nextStep (const Point& point)
			{
				Q_UNUSED (point);
			}
		signals:
			void createDotTable (const GameConfig& config);
			void needDestroy ();
		};

		class KDE_EXPORT Plugin : public KDots::IPlugin
		{
			Q_OBJECT
			Q_INTERFACES (KDots::IPlugin)
		public:
			Plugin (QObject *parent = 0)
				: IPlugin (parent)
			{
			}

			IRival* createRival ()
			{
				return new Rival;
			}

			QString name () const
			{
				return "singlepc";
			}

			QString description () const
			{
				return "Playing in the single PC";
			}
		};

	}
}

#endif
