#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UndoStackTests
#include <boost/test/unit_test.hpp>
#include "DotTable.h"
#include "IPlayer.h"

namespace
{
    class Player : public IPlayer
    {
        bool mHasTurn;
    public:
        Player(char player);
        
        bool hasTurn() const;
        
    protected:
        void onOpponentPointAddedImpl(std::size_t x, std::size_t y);
        void onPointAddedImpl(std::size_t x, std::size_t y);
        void onPlayerTurnImpl();
    };

    Player::Player(char player)
        : IPlayer(player)
        , mHasTurn(false)
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    //TODO: Implement this one
    void Player::onOpponentPointAddedImpl(std::size_t x, std::size_t y)
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    //TODO: Implement this one
    void Player::onPointAddedImpl(std::size_t x, std::size_t y)
    {
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    void Player::onPlayerTurnImpl()
    {
        mHasTurn = true;
        const DotTable *table = dotTable();
        for (std::size_t x = 0, maxx = table->width(), maxy = table->height(); x != maxx; ++x)
        {
            for (std::size_t y = 0; y != maxy; ++y)
            {
                if (table->cell(x, y)->owner() == Cell::NO_OWNER)
                {
                    put(x, y);
                    return;
                }
            }
        }
        
        // Game over
    }
    
    ////////////////////////////////////////////////////////////////////////////////

    bool Player::hasTurn() const
    {
        return mHasTurn;
    }
}

////////////////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(PlayerInterfaceTests)
{
    DotTable table(2, 2);
    
    Player player1(Cell::FIRST_OWNER);
    Player player2(Cell::SECOND_OWNER);
    player1.init();
    player2.init();
    
    table.registerPlayer(&player1);
    table.registerPlayer(&player2);
    
    table.putFirstPlayer(0, 0);
    
    BOOST_CHECK(player1.hasTurn() == true);
    BOOST_CHECK(player2.hasTurn() == true);
    
    for (std::size_t x = 0, maxx = table.width(), maxy = table.height(); x != maxx; ++x)
    {
        for (std::size_t y = 0; y != maxy; ++y)
            BOOST_CHECK(table.cell(x, y)->owner() != Cell::NO_OWNER);
    }
}