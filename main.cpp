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
#include <KApplication>
#include <KAboutData>
#include <KLocale>
#include <KCmdLineArgs>
#include "mainwindow.hpp"
#include "gameconfig.hpp"

int main (int argc, char **argv)
{
	KAboutData aboutData ("KDots", 0,
		ki18n ("KDots"), "0.5b",
		ki18n ("A prototype of the game of dots."),
		KAboutData::License_GPL_V3,
		ki18n ("Copyright (c) 2011-2012 Minh ngo") );
	
	KCmdLineArgs::init (argc, argv, &aboutData);
 
	KApplication app;
	
	KDots::GameConfig::registerMeta ();
	
	KDots::MainWindow *w = new KDots::MainWindow ();
	w->show ();

	return app.exec ();
}
