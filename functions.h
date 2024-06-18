#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
using namespace std;

struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    bool see_ball;
    int unum;
    double x;
    double y;
};

struct Ball
{
    std::string x;
    std::string y;
    std::string vx;
    std::string vy;
};

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);