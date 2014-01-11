#pragma once
#include <memory>
#include <QMainWindow>
#include "IPlayerManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
private slots:
    void makeGameOver();
private:
    std::unique_ptr<IPlayerManager> m_player_manager;
};