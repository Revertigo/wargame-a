//
// Created by osboxes on 5/19/20.
//
#include "doctest.h"
#include "Board.hpp"
#include "FootSoldier.hpp"
#include "FootCommander.hpp"
#include "Sniper.hpp"
#include "SniperCommander.hpp"
#include "Paramedic.hpp"
#include "ParamedicCommander.hpp"

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

    int move = 0;//First move UP
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
        CHECK_NOTHROW(b.move(2, pair(1, y_loc), (Board::MoveDIR)move));//Moves FootCommander
        CHECK_NOTHROW(b.move(1, pair(0, y_loc), (Board::MoveDIR)move));//Moves FootSoldier
        move = 1 - move;
        y_loc = 1 - y_loc;
    }

    //player's b FootCommander should be dead after exactly 5 rounds
    CHECK(b.has_soldiers(2) == true);//Player 2 has won ! wonder why
    CHECK(b.has_soldiers(1) == false);
}

TEST_CASE("Check Sniper behavior"){
    Board b(5,5);

    b[{2,2}] = new Sniper(1);
    b[{3,3}] = new Sniper(2);
    b[{1,1}] = new FootSoldier(1);
    b[{4,4}] = new FootSoldier(2);

    CHECK(b.has_soldiers(1) == true);
    CHECK(b.has_soldiers(2) == true);

    int move = 0; //First move UP
    int y_loc_s1 = 2;
    int y_loc_s2 = 3;
    //Player 1 should win after 4 moves with sniper
    for(int i = 0; i < 2; i++)
    {
        CHECK(b.has_soldiers(2) == true);
        CHECK(b.has_soldiers(1) == true);
        CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
        CHECK_NOTHROW(b.move(2, pair(3, y_loc_s2), (Board::MoveDIR)move));
        move = 1 - move;
        y_loc_s1 = 3 - y_loc_s1;
        y_loc_s2 = 5 - y_loc_s2;
    }

    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
    move = 1 - move;
    y_loc_s1 = 3 - y_loc_s1;
    CHECK(b.has_soldiers(2) == true);
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));

    CHECK(b.has_soldiers(2) == false);//Player 1 should win
    CHECK(b.has_soldiers(1) == true);
}

TEST_CASE("Check SniperCommander behavior"){
    Board b(5,5);

    b[{2,2}] = new SniperCommander(1);
    b[{3,3}] = new SniperCommander(2);
    b[{1,1}] = new Sniper(1);
    b[{4,4}] = new FootSoldier(2);

    CHECK(b.has_soldiers(1) == true);
    CHECK(b.has_soldiers(2) == true);

    int move = 0; //First move UP
    int y_loc_s1 = 2;
    int y_loc_s2 = 3;
    //Player 1 should win after 2 moves with sniper

    CHECK(b.has_soldiers(2) == true);
    CHECK(b.has_soldiers(1) == true);
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
    CHECK_NOTHROW(b.move(2, pair(3, 3), (Board::MoveDIR)move));
    move = 1 - move;
    y_loc_s1 = 3 - y_loc_s1;
    y_loc_s2 = 5 - y_loc_s2;

    CHECK(b.has_soldiers(2) == true);
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
    CHECK_THROWS(b.move(2, pair(3, y_loc_s2), (Board::MoveDIR)move));//The SC2 should be dead
    CHECK_THROWS(b.move(2, pair(4, 4), (Board::MoveDIR)move));//The FS should be dead
    move = 1 - move;
    y_loc_s1 = 3 - y_loc_s1;
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));//SC1 Should be alive
    CHECK_NOTHROW(b.move(1, pair(1, 1), (Board::MoveDIR)move));//S1 Should be alive

    CHECK(b.has_soldiers(2) == false);//Player 1 should win
    CHECK(b.has_soldiers(1) == true);
}

TEST_CASE("Check Paramdedic behavior"){
    Board b(5,5);

    b[{2,2}] = new Paramedic(1);
    b[{3,3}] = new Paramedic(2);
    b[{1,1}] = new Sniper(1);
    b[{4,4}] = new Sniper(2);
    b[{3,1}] = new FootCommander(1);

    CHECK(b.has_soldiers(1) == true);
    CHECK(b.has_soldiers(2) == true);

    int move = 0; //First move UP
    int y_loc_p1 = 2;
    int y_loc_p2 = 3;
    int y_loc_s1 = 1;
    int y_loc_s2 = 4;
    int y_loc_f1 = 1;

    //After 5 rounds p2 should be dead
    for(int i = 0; i < 4; i++)
    {
        CHECK(b.has_soldiers(2) == true);
        CHECK(b.has_soldiers(1) == true);
        CHECK_NOTHROW(b.move(1, pair(3, y_loc_f1), (Board::MoveDIR)move));//Always target adversary paramedic
        CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
        CHECK_NOTHROW(b.move(2, pair(3, y_loc_s2), (Board::MoveDIR)move));
        CHECK_NOTHROW(b.move(1, pair(2, y_loc_p1), (Board::MoveDIR)move));
        CHECK_NOTHROW(b.move(2, pair(3, y_loc_p2), (Board::MoveDIR)move));

        move = 1 - move;
        y_loc_p1 = 3 - y_loc_p1;
        y_loc_p2 = 5 - y_loc_p2;
        y_loc_s1 = 1 - y_loc_s1;
        y_loc_s2 = 7 - y_loc_s2;
        y_loc_f1 = 1 - y_loc_f1;
    }

    CHECK(b.has_soldiers(2) == true);
    CHECK(b.has_soldiers(1) == true);
    CHECK_NOTHROW(b.move(1, pair(3, y_loc_f1), (Board::MoveDIR)move));//Always target adversary paramedic
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));
    CHECK_NOTHROW(b.move(2, pair(3, y_loc_s2), (Board::MoveDIR)move));
    CHECK_NOTHROW(b.move(1, pair(2, y_loc_p1), (Board::MoveDIR)move));
    CHECK_THROWS(b.move(2, pair(3, y_loc_p2), (Board::MoveDIR)move));//Check p2 is dead
    CHECK(b.has_soldiers(2) == true);//Check p2 still has soldiers(s2)

    move = 1 - move;
    y_loc_s1 = 1 - y_loc_s1;
    y_loc_s2 = 7 - y_loc_s2;

    CHECK_NOTHROW(b.move(1, pair(2, y_loc_s1), (Board::MoveDIR)move));//Should kill s2(he now has 50 HP)
    CHECK_THROWS(b.move(2, pair(3, y_loc_s2), (Board::MoveDIR)move));//Should be dead

    CHECK(b.has_soldiers(2) == false);//Player 1 should win
    CHECK(b.has_soldiers(1) == true);
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