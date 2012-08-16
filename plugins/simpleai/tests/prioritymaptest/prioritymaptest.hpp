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
#pragma once
#include <QtTest>
#include <QObject>
#include "prioritymap.hpp"
using namespace KDots::simpleai;

const MapData TEST_DATA {
	{
		{NM, CU, SE},
		{PF, EM, PF},
		{PS, NM, PS},
		{EM, EM, EM}
	},
	{1, 0},
	0.1
};

const MapData TEST_DATA_1 {
	{
		{NM, SE},
		{PF, EM},
		{PS, CU},
		{EM, EM}
	},
	{1, 2},
	0.1
};

class PriorityMapTest : public QObject
{
	Q_OBJECT
private slots:
	void sizeTest ()
	{
		const std::list<MapData>& dataList = PriorityMap::instance ().priorityMap ();
		QVERIFY (dataList.size () == 33 * 8);
	}
	
	void currenPointTest ()
	{
		const std::list<MapData>& dataList = PriorityMap::instance ().priorityMap ();
		int k = 0;
		for (const MapData& data : dataList)
		{
			++k;
			qDebug () << Q_FUNC_INFO << __LINE__ << k;
			QVERIFY (data.m_map[data.m_current.y ()][ data.m_current.x ()] == MapElement::CU);
			
		}
	}
	
	void inverseTest ()
	{
		const std::list<MapData>& dataList = PriorityMap::instance ().priorityMap ();
		
		const MapData& inverted = PriorityMap::inverse (TEST_DATA);
		const MapData answer {
			{
				{SE, CU, NM},
				{PF,EM, PF},
				{PS, NM, PS},
				{EM, EM,EM}
			},
			{1, 0},
			0.1
		};
		
		QVERIFY (answer == inverted);
		
		const MapData& inverted_1 = PriorityMap::inverse (TEST_DATA_1);
		const MapData answer_1 {
			{
				{SE, NM},
				{EM, PF},
				{CU, PS},
				{EM, EM}
			},
			{0, 2},
			0.1
		};
		
		QVERIFY (answer_1 == inverted_1);
	}
	
	void rotateTest ()
	{
		const std::list<MapData>& dataList = PriorityMap::instance ().priorityMap ();
		
		const MapData& rotated = PriorityMap::rotate (TEST_DATA);
		const MapData answer {
			{
				{EM, PS, PF, NM},
				{EM, NM, EM, CU},
				{EM, PS, PF, SE}
			},
			{3, 1},
			0.1
		};
		
		QVERIFY (rotated == answer);
		
		const MapData& rotated_1 = PriorityMap::rotate (TEST_DATA_1);
		const MapData answer_1 {
			{
				{EM, PS, PF, NM},
				{EM, CU, EM, SE}
			},
			{1, 1},
			0.1
		};
		
		QVERIFY (rotated_1 == answer_1);
	}
};

