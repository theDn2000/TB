#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
using namespace std;

// Include headers
#include "functions.h"
#include "structs.h"


// Functions

// Parse initial message function (init Side Unum PlayMode)
Player parseInitialMessage(std::string &message, Player &player)
{
    std::string delimiter = " ";
    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = message.find(delimiter)) != std::string::npos)
    {
        token = message.substr(0, pos);
        if (i == 1)
        {
            player.side = token;
        }
        else if (i == 2)
        {
            player.unum = std::stoi(token);
        }
        else if (i == 3)
        {
            player.playmode = token;
        }
        message.erase(0, pos + delimiter.length());
        i++;
    }
    cout << "Player: " << player.side << " " << player.unum << " " << player.playmode << endl;
    return player;
}

vector<string> separate_string_separator(string & s, string separator)
{
    vector<string> v;
    int pos = 0;
    while ((pos = s.find(separator)) != string::npos)
    {
        v.push_back(s.substr(0, pos));
        s.erase(0, pos + separator.length());
    }
    v.push_back(s);
    return v;
}

// Parse string function
vector<string> separate_string(string & s)
{
    // Separa por contenido entre parentesis, por nivel: "((hola) (soy) (dani)) (que tal) (estas)" -> {(hola) (soy) (dani), que tal, estas}
    vector<string> v;
    int pos{0};
    int level{0};
    string temp;
     /*
    // Compruebo que empieza con parentesis
    if (s[0] != '(')
    {
        throw std::runtime_error("No empieza con parentesis");
    }
   
    // Compruebo que termina con parentesis
    if (s[s.size()-1] != ')')
    {
        throw std::runtime_error("No termina con parentesis");
    }
    
    // Comprueba que no haya paréntesis sin ningún carácter dentro
    if (s[pos] == '(' && s[pos+1] == ')')
    {
        throw std::runtime_error("Parentesis vacio");
    }
    */

    // Recorro el string
    while (pos < s.size())
    {
        if (s[pos] == '(')
        {
            level++;
        }
        else if (s[pos] == ')')
        {
            level--;
        }

        // Dependiendo del nivel, se añade al vector o se reinicia el string temporal
        if (s[pos] == '(' && level == 1)
        {
            temp = "";
        }
        else if (s[pos] == ')' && level == 0)
        {
            v.push_back(temp);
        }
        else
        {
            // Comprueba que no hay palabras sin parentesis
            /*
            if (level == 0 && s[pos] != ' ')
            {
                throw std::runtime_error("Palabra sin parentesis");
            }
            */
            
            
                temp += s[pos];
            
        }
        pos++;
    }
    
    // Compruebo que se han abierto y cerrado los mismos parentesis
    if (level != 0)
    {
        throw std::runtime_error("No se han abierto y cerrado los mismos parentesis");
    }
    else
    {
        return v;
    }
}

// Find data in see message function
void store_data_see(vector<string> &see_message, Player &player, Ball &ball, CentroPorteriaDerecha &goal_R, CentroPorteriaIzquierda &goal_L)
{
    vector<string> ball_coords;
    bool found_ball = false;
    bool found_goal_R = false;
    bool found_goal_L = false;
    for (size_t i = 0; i < see_message.size(); i++)
    {
        // Search for the ball
        if (see_message[i].find("(b)")!=string::npos)
        {
            cout << "The player sees the ball" << endl;
            player.see_ball = true;
            ball_coords = separate_string_separator(see_message[i], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];
            cout << "Ball coordinates: " << ball.x << " " << ball.y << endl;
            // Calculate the distance to the ball
            double distance = sqrt(pow(stod(ball.x), 2) + pow(stod(ball.y), 2));
            ball.distance = distance;
            // Calculate the angle to the ball
            double angle = atan2(stod(ball.y), stod(ball.x));
            angle = angle * 180 / M_PI;
            found_ball = true;
            //cout << "Angle to the ball: " << angle << endl;
        } 
    }
    if (found_ball == false)
    {
        player.see_ball = false;
    }
}


























// Find data in see message function
/*
// Move command function
void sendInitialMoveMessage(const Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
    struct Posicion
    {
        int x;
        int y;
    };

    vector<Posicion>
        posiciones = {{-50, 0},
                      {-40, -10},
                      {-35, -28},
                      {-40, 10},
                      {-35, 28},
                      {-25, 11},
                      {-8, 20},
                      {-25, -11},
                      {-5, 0},
                      {-15, 0},
                      {-8, -20}};
  
    Posicion myPos = posiciones[player.unum - 1];

    auto moveCommand = "(move " + to_string(myPos.x) + " " + to_string(myPos.y) + ")";
    udp_socket.sendTo(moveCommand, recipient);
    cout << "Move command sent" << "Posicion: " << moveCommand << endl;
}
*/