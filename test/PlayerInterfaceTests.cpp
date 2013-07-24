#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE UndoStackTests
#include <boost/test/unit_test.hpp>
#include "DotTable.h"
#include "IPlayer.h"

class Player : public IPlayer
{
    bool mHasTurn;
public:
    Player(char player);
    
    void onOpponentPointAdded(std::size_t x, std::size_t y);
    void onPointAdded(std::size_t x, std::size_t y);
    void onPlayerTurn();
    
    bool hasTurn() const;
};

Player::Player(char player)
    : IPlayer(player)
    , mHasTurn(false)
{
}

//TODO: Implement this one
void Player::onOpponentPointAdded(std::size_t x, std::size_t y)
{
}

//TODO: Implement this one
void Player::onPointAdded(std::size_t x, std::size_t y)
{
}

void Player::onPlayerTurn()
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

bool Player::hasTurn() const
{
    return mHasTurn;
}

BOOST_AUTO_TEST_CASE(PlayerInterfaceTests)
{
    DotTable table(2, 2);
    
    Player player1(Cell::FIRST_OWNER);
    Player player2(Cell::SECOND_OWNER);
    
    player1.registerTable(&table);
    player2.registerTable(&table);
    
    table.put(0, 0);
    
    BOOST_CHECK(player1.hasTurn() == true);
    BOOST_CHECK(player2.hasTurn() == true);
    
    for (std::size_t x = 0, maxx = table.width(), maxy = table.height(); x != maxx; ++x)
    {
        for (std::size_t y = 0; y != maxy; ++y)
            BOOST_CHECK(table.cell(x, y)->owner() != Cell::NO_OWNER);
    }
}