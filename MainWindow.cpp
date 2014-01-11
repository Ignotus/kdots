#include <QStatusBar>
#include <QMessageBox>
#include "MainWindow.h"
#include "Board.h"
#include "BoardView.h"
#include "PlayerManagerWET.h"
#include "ScoreCounter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_player_manager(new PlayerMangerWET)
{
    setMinimumSize(640, 480);
    Board *board = new Board(3, 3, this);
    board->setPlayerManager(m_player_manager.get());
    
    ScoreCounter *counter = new ScoreCounter(this);
    board->setScoreCounter(counter);
    
    QStatusBar *bar = statusBar();
    bar->showMessage("First: 0 - Second: 0");
    
    connect(counter,
            SIGNAL(statusUpdated(QString)),
            bar,
            SLOT(showMessage(const QString&)));
    
    connect(board,
            SIGNAL(gameOver()),
            this,
            SLOT(makeGameOver()));
            
    
    BoardView *view = new BoardView(this);
    view->setModel(board);
    
    setCentralWidget(view);
}

void MainWindow::makeGameOver()
{
    QMessageBox::information(this, "Game over!", "Game Over!");
}