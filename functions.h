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

string returnToZone(Player const &player);

vector<double> trilateration(vector<double> &P1, vector<double> &P2, vector<double> &P3, double &D1, double &D2, double &D3);

string moveToZone(double &orientation, vector<double> const &P_player, Posicion const &P_zone);