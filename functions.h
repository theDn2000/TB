#pragma once
#include<iostream>
#include<vector>
#include<string>
using namespace std;

struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    int unum;
    double x;
    double y;
};

struct Ball
{
    double x;
    double y;
    double vx;
    double vy;
};

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);