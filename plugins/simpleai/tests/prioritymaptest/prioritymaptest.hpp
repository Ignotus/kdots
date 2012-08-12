/*
 * kdots
 * Copyright (C) 2011-2012 Minh Ngo <nlminhtl@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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

