#include <QApplication>
#include "Board.h"
#include "BoardView.h"
#include "IPlayerManager.h"

class PlayerManger : public IPlayerManager
{
    int m_player;
public:
    PlayerManger()
    {
        m_player = 0;
    }
    
    inline int nextPlayer(bool is_captured)
    {
        return (m_player = (m_player == 0 ? 1 : 0));
    }
    
    inline int currentPlayer() const
    {
        return m_player;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Board board(10, 10);
    PlayerManger manager;
    board.setPlayerManager(&manager);
    
    BoardView view;
    view.setModel(&board);
    
    view.show();
    
    return app.exec();
}