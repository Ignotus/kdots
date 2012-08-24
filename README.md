KDots
=====

[![KDots](http://i.imgur.com/koAZ5.png)](http://i.imgur.com/koAZ5.png)

About
=====
KDots is the simple implementation of the game of dots written with Qt Toolkit.

The purpose of Dots game is to catch your opponent's dots by placing your dots
on the game board where the lines cross.

License
=======
Look the [COPYING](https://github.com/Ignotus/kdots/blob/master/COPYING) file for more information.

Dependencies for building
=========================
* Qt Framework (QtGui, QtCore, QtNetwork) 4.8 or later.
* cmake 2.6 or later.
* KDELibs 4.8 or later
* LibKDEGames 4.9
* A C++ compiler with C++11 support.

Building
========
To build current version of source code, execute following commands:

    $ cd kdots
    $ mkdir build && cd build
    $ cmake ../ -DCMAKE_INSTALL_PREFIX=/usr
    $ make

Installing
==========
    # make install

Uninstalling
============
    # make uninstall

Contributing
============
You are welcome! ;)

Other
=====
Screenshots and addtional information are located in the [wiki](https://github.com/Ignotus/kdots/wiki).
