/*
 * KDots
 * Copyright (c) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
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
#ifndef KDOTS_PLUGINS_SIMPLEAI_RIVAL_HPP
#define KDOTS_PLUGINS_SIMPLEAI_RIVAL_HPP
#include <memory>
#include <QLabel>
#include <interface/irival.hpp>

namespace KDots
{
	namespace simpleai
	{
		struct MapData;
		
		class Rival : public KDots::IRival
		{
			Q_OBJECT
			Q_INTERFACES (KDots::IRival)
			
			DotTable *m_table;
			std::vector<Point> m_pointStack;
			Owner m_current, m_other;
			int m_iterations;
		public:
			Rival (QObject *parent = 0);
			~Rival ();
			bool isAllow () const;
			
			void setDifficulty (int diff);
		public slots:
			void nextStep (const Point& point);
			
			void setDotTable (DotTable *table);
		private:
			bool hasMask (const Point& point, const MapData& mask);
			float calcImportance(const Point& point);
			void calcImportanceTree (float& importance, const Point& point, int iteration);
			void calcRange (int& min_x, int& min_y, int& max_x, int& max_y);
			bool hasCaptured (const Point& point, Owner current);
			
		signals:
			void createDotTable (const GameConfig& config);
			void needDestroy ();
		};
	}
}

#endif
