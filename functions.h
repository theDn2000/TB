#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
#include"structs.h"
using namespace std;

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);

void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field);

vector<double> trilateration(vector<double> &P1, vector<double> &P2, vector<double> &P3, double &D1, double &D2, double &D3);