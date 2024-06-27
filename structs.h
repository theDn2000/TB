#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <string>
#include<iostream>
#include <vector>

using namespace std;


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
    vector<float> flag_center_abs = {0, 0};
    vector<float> flag_center_top;
    vector<float> flag_center_top_abs = {0, -33.5};
    vector<float> flag_center_bottom;
    vector<float> flag_center_bottom_abs = {0, 33.5};
    vector<float> flag_left_top;
    vector<float> flag_left_top_abs = {-52.2, -33.5};
    vector<float> flag_left_bottom;
    vector<float> flag_left_bottom_abs = {-52.2, 33.5};
    vector<float> flag_right_top;
    vector<float> flag_right_top_abs = {52.5, -33.5};
    vector<float> flag_right_bottom;
    vector<float> flag_right_bottom_abs = {52.5, 33.5};

    vector<float> flag_penalty_left_top;
    vector<float> flag_penalty_left_top_abs = {-36, -20};
    vector<float> flag_penalty_left_center;
    vector<float> flag_penalty_left_center_abs = {-36, 0};
    vector<float> flag_penalty_left_bottom;
    vector<float> flag_penalty_left_bottom_abs = {-36, 20};
    vector<float> flag_penalty_right_top;
    vector<float> flag_penalty_right_top_abs = {36, -20};
    vector<float> flag_penalty_right_center;
    vector<float> flag_penalty_right_center_abs = {36, 0};
    vector<float> flag_penalty_right_bottom;
    vector<float> flag_penalty_right_bottom_abs = {36, 20};

    vector<float> flag_goal_left_top;
    vector<float> flag_goal_left_top_abs = {-52.5, -7.32};
    vector<float> flag_goal_left_bottom;
    vector<float> flag_goal_left_bottom_abs = {-52.5, 7.32};
    vector<float> flag_goal_right_top;
    vector<float> flag_goal_right_top_abs = {52.5, -7.32};
    vector<float> flag_goal_right_bottom;
    vector<float> flag_goal_right_bottom_abs = {52.5, 7.32};
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