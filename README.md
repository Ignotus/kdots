# KDots

[![KDots](http://i.imgur.com/Izm74a6.png)](http://i.imgur.com/Izm74a6.png)

About
-----
KDots is a simple implementation of the game of dots written with Qt Toolkit
and KDELibs.

The purpose of the Dots game is to catch your opponent's dots by placing your
dots on the game board where the lines cross.

Game mode can be extended via plugins. Currently, 3 plugins are available for
supporting AI, online and offline games between two players.

License
-------

All source code is under the BSD license. Look the [COPYING](https://github.com/Ignotus/kdots/blob/master/COPYING)
file for more information.

Dependencies
------------
* Qt5::Core, Qt5::Widgets, Qt5::Network
* cmake 2.8.12 or later
* extra-cmake-modules
* Boost (tested on 1.55)
* KF5::TextWidgets, KF5::XmlGui, KF5::KDELibs4Support
* KF5KDEGames 4.9.0
* A C++ compiler with C++11 support.

Building
--------
To build current version of source code, execute following commands:

    $ mkdir build && cd build
    $ cmake ../ -DCMAKE_INSTALL_PREFIX=/usr
    $ make

Installing
----------
    # make install

Other
-----

CI for the master branch is available [here](http://build.kde.org/job/kdots_master_qt5/)
