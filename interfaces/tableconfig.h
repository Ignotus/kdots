#pragma once
#include <QSize>

enum GameType {
  WITH_ADDITIONAL_MOVE,
  WITHOUT_ADDITIONAL_MOVE
};

struct TableConfig {
  int gameType;
  int firstPlayer;
  QSize size;
};