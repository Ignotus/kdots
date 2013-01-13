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
#ifndef KDOTS_CONSTANTS_HPP
#define KDOTS_CONSTANTS_HPP
#include <QString>
#include <kdemacros.h>
#include "point.hpp"
#ifndef KDOTS_EXPORT
# if defined(MAKE_KDOTS_LIB)
// We are building this library
#  define KDOTS_EXPORT KDE_EXPORT
# else
// We are using this library
#  define KDOTS_EXPORT KDE_IMPORT
# endif
#endif

# ifndef KDOTS_EXPORT_DEPRECATED
#  define KDOTS_EXPORT_DEPRECATED KDE_DEPRECATED KDOTS_EXPORT
# endif

namespace KDots {
  enum GameMode {
    EXTRA_TURN_MODE,
    DEFAULT_MODE
  };
  
  enum Owner {
    NONE,
    FIRST,
    SECOND
  };
  
  const int DIRECTION_COUNT = 8;
  
  const int GRAPH_DX[DIRECTION_COUNT] = {0, 1, 1, 1, 0, -1, -1, -1};
  const int GRAPH_DY[DIRECTION_COUNT] = {1, 1, 0, -1, -1, -1, 0, 1};
  
  const QString PLUGIN_SUFFIX = "kdots_";
}

#endif