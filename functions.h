#pragma once
#include<iostream>
#include<vector>
#include<string>
#include <cmath>
using namespace std;

Player parseInitialMessage(std::string &message, Player &player);

vector<string> separate_string(string & s);

vector<string> separate_string_separator(string & s, string separator);