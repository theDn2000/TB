#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include<iostream>
#include <vector>


struct Player
{
    std::string team_name;
    std::string playmode;
    std::string side;
    bool see_ball;
    bool see_own_goal;
    bool see_opponent_goal;
    int flags_seen;
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
    double distance;
    double angle;
};

struct Field
{
    vector<float> flag_center;
    vector<float> flag_center_top;
    vector<float> flag_center_bottom;
    vector<float> flag_left_top;
    vector<float> flag_left_bottom;
    vector<float> flag_right_top;
    vector<float> flag_right_bottom;
};

struct Goal
{
    std::string x;
    std::string y;
    std::string side;
};

struct JugadorCercano
{
    std::string x;
    std::string y;
    std::string distancia;
    std::string unum;
};

struct JugadoresAlrededor
{
    std::vector<int> x;
    std::vector<int> y;
    std::vector<int> distancia;
    std::vector<int> unum;
};


#endif // ESTRUCTURAS_H