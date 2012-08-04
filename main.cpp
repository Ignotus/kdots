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
#include <QApplication>
#include "mainwindow.hpp"
#include "gameconfig.hpp"
#include "point.hpp"

void registerMeta ()
{
	qRegisterMetaType<KDots::GameConfig> ("GameConfig");
	qRegisterMetaTypeStreamOperators<KDots::GameConfig> ("GameConfig");
	qRegisterMetaType<KDots::Point> ("Point");
	qRegisterMetaTypeStreamOperators<KDots::Point> ("Point");
}


int main (int argc, char **argv)
{
	QApplication app (argc, argv);
	
	registerMeta ();
	
	KDots::MainWindow w;
	w.show ();

	return app.exec ();
}
