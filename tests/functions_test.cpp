#include "gtest/gtest.h"
#include "../functions.h"


using namespace std;

// Parse strings tests

TEST(ParseInitialMessage, ParseInitialMessageTest)
{
    Player player;
    string message = "(init l 1 before_kick_off)";
    player = parseInitialMessage(message, player);
    EXPECT_EQ(player.side, "l");
    EXPECT_EQ(player.unum, 1);
}

// Store data see tests
TEST(StoreDataSee, StoreDataSeeTest)
{
    string see = "(see 2620 ((f c) 51.4 0) ((f c t) 61.6 -34) ((f c b) 61.6 33) ((f r t) 108.9 -18) ((f r b) 108.9 18) ((f g r b) 103.5 4) ((g r) 103.5 0) ((f g r t) 103.5 -4) ((G) 1.3 -171) ((f p r b) 89.1 13) ((f p r c) 87.4 0) ((f p r t) 89.1 -13) ((f p l b) 25 53 0 0) ((f p l c) 15.2 -1 0 0) ((f p l t) 25.3 -53 0 0) ((f t 0) 64.7 -37) ((f t r 10) 73 -33) ((f t r 20) 81.5 -29) ((f t r 30) 90 -26) ((f t r 40) 99.5 -23) ((f t r 50) 108.9 -21) ((f t l 10) 56.8 -44) ((f t l 20) 49.9 -52) ((f b 0) 64.1 37) ((f b r 10) 72.2 32) ((f b r 20) 81.5 29) ((f b r 30) 90 26) ((f b r 40) 99.5 23) ((f b r 50) 108.9 21) ((f b l 10) 56.8 43) ((f b l 20) 49.9 51) ((f r 0) 108.9 0) ((f r t 10) 108.9 -5) ((f r t 20) 111.1 -11) ((f r t 30) 113.3 -16) ((f r b 10) 108.9 5) ((f r b 20) 109.9 10) ((f r b 30) 112.2 15) ((b) 49.4 0) ((l r) 103.5 90))";
    Player player;
    Ball ball;
    CentroPorteriaDerecha goal_R;
    CentroPorteriaIzquierda goal_L;
    vector<string> see_message = separate_string(see);
    store_data_see(see_message, player, ball, goal_R, goal_L);
    EXPECT_EQ(player.see_goal, true);
    EXPECT_EQ(ball.x, "49.4");
    EXPECT_EQ(ball.y, "0");
    EXPECT_EQ(goal_R.x, "103.5");
    EXPECT_EQ(goal_R.y, "0");
}