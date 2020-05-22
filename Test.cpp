//
// Created by osboxes on 5/19/20.
//
#include "doctest.h"
#include "Board.hpp"
#include "FootSoldier.hpp"
#include "FootCommander.hpp"

using namespace std;
using namespace WarGame;

TEST_CASE("Check FootSoldier behavior"){
    Board b(3,3);

    CHECK(b.has_soldiers(1) == false);
    CHECK(b.has_soldiers(2) == false);

    b[{0,1}] = new FootSoldier(1);
    b[{1,2}] = new FootSoldier(2);

    CHECK(b.has_soldiers(1) == true);
    CHECK(b.has_soldiers(2) == true);

    int move = 0;
    int y_loc = 1;
    for(int i = 0; i < 10; i++)
    {
        CHECK_NOTHROW(b.move(1, pair(0, y_loc), (Board::MoveDIR)move));
        move = 1 - move;
        y_loc = 1 - y_loc;
    }

    CHECK(b.has_soldiers(2) == false);//Player 1 has won ! wonder why
    CHECK(b.has_soldiers(1) == true);
}

TEST_CASE("Check FootCommander behavior"){
    Board b(3,3);

    b[{0,1}] = new FootSoldier(1);
    b[{1,2}] = new FootCommander(2);
    b[{1,1}] = new FootCommander(2);

    int move = 0;//Starting move is UP
    int y_loc = 1;
    for(int i = 0; i < 5; i++)
    {
        //Move UP, then DOWN, then UP...
        CHECK_NOTHROW(b.move(1, pair(1, y_loc), (Board::MoveDIR)move));//Moves FootCommander
        CHECK_NOTHROW(b.move(1, pair(0, y_loc), (Board::MoveDIR)move));//Moves FootSoldier
        move = 1 - move;
        y_loc = 1 - y_loc;
    }

    //player's b FootCommander should be dead after exactly 5 rounds
    CHECK(b.has_soldiers(2) == false);//Player 1 has won ! wonder why
    CHECK(b.has_soldiers(1) == true);
}

TEST_CASE("Check FootCommander Sniper"){

}

TEST_CASE("Check 3 tropps vs 3 tropps"){
    Board b(8,8);

    CHECK(b.has_soldiers(1) == false);
    CHECK(b.has_soldiers(2) == false);

    CHECK_THROWS(b.has_soldiers(3));

    b[{0,1}] = new FootSoldier(1);
    b[{0,3}] = new FootCommander(1);
    b[{0,5}] = new FootSoldier(1);
    CHECK(b.has_soldiers(1) == true);

    // Add soldiers for player 2:
    b[{7,1}] = new FootSoldier(2);
    b[{7,3}] = new FootCommander(2);
    b[{7,5}] = new FootSoldier(2);
    CHECK(b.has_soldiers(2) == true);
}