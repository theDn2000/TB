#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <MinimalSocket/udp/UdpSocket.h>
using namespace std;

// Include headers
#include "functions.h"
#include "structs.h"

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

// Move players to the field
void sendInitialMoveMessage(Player &player, MinimalSocket::udp::Udp<true> &udp_socket, MinimalSocket::Address const &recipient)
{
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

// Configure player
void configurePlayer(Player &player)
{
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
                      
    const std::vector<Posicion> flags_config = 
    {
        {0, 0},        // Center of the field
        {0, -33.5},       // Top center
        {0, 33.5},       // Bottom center
        {-52.5, -33.5},    // Corner top-left
        {-52.5, 33.5},     // Corner bottom-left
        {52.5, -33.5},     // Corner top-right
        {52.5, 33.5},      // Corner bottom-right
        {-36, -20},        // Penalty top-left
        {-36, 0},           // Penalty center-left
        {-36, 20},         // Penalty bottom-left
        {36, -20},         // Penalty top-right
        {36, 0},           // Penalty center-right
        {36, 20},          // Penalty bottom-right
        {-52.5, -7.32},    // Goal top-left
        {-52.5, 7.32},     // Goal bottom-left
        {52.5, -7.32},     // Goal top-right
        {52.5, 7.32}       // Goal bottom-right
    };
    if (player.unum == 1)
    {
        player.rol = "PORTERO";
        player.range = 10;
        player.zone = posiciones[player.unum - 1];
        player.zone_name = "goalkeeper"; // Does not apply
    }
    else if (player.unum == 3)
    {
        player.rol = "DEFENSA";
        player.range = 20;
        player.zone = flags_config[7];
        player.zone_name = "(f " + player.side + " t)";
    }
    else if (player.unum == 5)
    {
        player.rol = "DEFENSA";
        player.range = 20;
        player.zone = flags_config[9];
        player.zone_name = "(f " + player.side + " b)";
    }
    else if (player.unum == 2)
    {
        player.rol = "DEFENSA";
        player.range = 20;
        player.zone = posiciones[player.unum - 1];
        player.zone_name = "(f p " + player.side + " t)";
    }
    else if (player.unum == 4)
    {
        player.rol = "DEFENSA";
        player.range = 20;
        player.zone = posiciones[player.unum - 1];
        player.zone_name = "(f p " + player.side + " b)";
    }
    else if (player.unum == 8)
    {
        player.rol = "DEFENSA";
        player.range = 30;
        player.zone = flags_config[0];
        player.zone_name = "(f p " + player.side + " t)";
    }
    else if (player.unum == 6)
    {
        player.rol = "DEFENSA";
        player.range = 30;
        player.zone = flags_config[2];
        player.zone_name = "(f p " + player.side + " b)";
    }
    else if (player.unum == 9)
    {
        if (player.side == "l")
        {
            player.rol = "DELANTERO";
            player.range = 20;
            player.zone = flags_config[1];
            player.zone_name = "(f p r c)";
        }
        else
        {
            player.rol = "DELANTERO";
            player.range = 20;
            player.zone = flags_config[1];
            player.zone_name = "(f p l c)";
        }
    }
    else if (player.unum ==10)
    {
        player.rol = "DELANTERO";
        player.range = 15;
        player.zone = posiciones[player.unum - 1];
        player.zone_name = "(f c)";
    }
    else if (player.unum == 7)
    {
        player.rol = "DELANTERO";
        player.range = 25;
        player.zone = flags_config[12];
        player.zone_name = "(f c b)";
    }
    else if (player.unum == 11)
    {
        player.rol = "DELANTERO";
        player.range = 25;
        player.zone = flags_config[10];
        player.zone_name = "(f c t)";
    }
    else
    {
        player.rol = "DELANTERO";
        player.range = 65;
        player.zone = flags_config[11];
        player.zone_name = "(f c)";
    }
}

// Functions



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
void store_data_see(vector<string> &see_message, Player &player, Ball &ball, Goal &own_goal, Goal &opponent_goal, Field &field)
{
    vector<string> ball_coords;
    player.flags_seen = 0;

    player.see_ball = false;
    player.seeing_zone = false;

    // All the flags are not seen at the beginning
    field.flag_center = {999, 999};
    field.flag_center_top = {999, 999};
    field.flag_center_bottom = {999, 999};
    field.flag_left_top = {999, 999};
    field.flag_left_bottom = {999, 999};
    field.flag_right_top = {999, 999};
    field.flag_right_bottom = {999, 999};
    field.flag_penalty_left_top = {999, 999};
    field.flag_penalty_left_center = {999, 999};
    field.flag_penalty_left_bottom = {999, 999};
    field.flag_penalty_right_top = {999, 999};
    field.flag_penalty_right_center = {999, 999};
    field.flag_penalty_right_bottom = {999, 999};
    field.flag_goal_left_top = {999, 999};
    field.flag_goal_left_bottom = {999, 999};
    field.flag_goal_right_top = {999, 999};
    field.flag_goal_right_bottom = {999, 999};

    if (own_goal.side == "l")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }
    if (own_goal.side == "r")
    {
        player.see_own_goal = false;
    }
    else
    {
        player.see_opponent_goal = false;
    }




    for (size_t i = 0; i < see_message.size(); i++)
    {
        // Search for the ball
        if (see_message[i].find("(b)")!=string::npos)
        {
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
            ball.angle = angle * 180 / M_PI;
        } 

        // Search for the right goal
        if (see_message[i].find("(g r)")!=string::npos)
        {
            cout << "The player sees the right goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "r")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the left goal
        if (see_message[i].find("(g l)")!=string::npos)
        {
            cout << "The player sees the left goal" << endl;
            vector<string> goal_coords = separate_string_separator(see_message[i], " ");

            if (own_goal.side == "l")
            {
                own_goal.x = goal_coords[2];
                own_goal.y = goal_coords[3];
                cout << "Own goal coordinates: " << own_goal.x << " " << own_goal.y << endl;
                player.see_own_goal = true;
            }
            else
            {
                opponent_goal.x = goal_coords[2];
                opponent_goal.y = goal_coords[3];
                opponent_goal.angle = atan2(stof(goal_coords[3]), stof(goal_coords[2])) * 180 / M_PI;
                cout << "Opponent goal coordinates: " << opponent_goal.x << " " << opponent_goal.y << endl;
                player.see_opponent_goal = true;
            }
        }

        // Search for the flags
        // Search for the center flag
        if (see_message[i].find("(f c)") != string::npos)
        {
            player.flags_seen++;
            vector<string> center_coords = separate_string_separator(see_message[i], " ");
            field.flag_center = {stof(center_coords[2]), stof(center_coords[3])};
            cout << "Center flag coordinates: " << field.flag_center[0] << " " << field.flag_center[1] << endl;
        }
        if (player.zone_name =="(f c)")
            {
                player.seeing_zone = true;
            }

        // Search for the center top flag
        if (see_message[i].find("(f c t)") != string::npos)
        {
            vector<string> center_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_top = {stof(center_top_coords[3]), stof(center_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f c t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the center bottom flag
        if (see_message[i].find("(f c b)") != string::npos)
        {
            vector<string> center_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_center_bottom = {stof(center_bottom_coords[3]), stof(center_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f c b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left top flag
        if (see_message[i].find("(f l t)") != string::npos)
        {
            vector<string> left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_top = {stof(left_top_coords[3]), stof(left_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the left bottom flag
        if (see_message[i].find("(f l b)") != string::npos)
        {
            vector<string> left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_left_bottom = {stof(left_bottom_coords[3]), stof(left_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right top flag
        if (see_message[i].find("(f r t)") != string::npos)
        {
            vector<string> right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_top = {stof(right_top_coords[3]), stof(right_top_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the right bottom flag
        if (see_message[i].find("(f r b)") != string::npos)
        {
            vector<string> right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_right_bottom = {stof(right_bottom_coords[3]), stof(right_bottom_coords[4])};
            player.flags_seen++;
            if (player.zone_name =="(f r b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left top flag
        if (see_message[i].find("(f p l t)") != string::npos)
        {
            vector<string> penalty_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_top = {stof(penalty_left_top_coords[4]), stof(penalty_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left center flag
        if (see_message[i].find("(f p l c)") != string::npos)
        {
            vector<string> penalty_left_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_center = {stof(penalty_left_center_coords[4]), stof(penalty_left_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p l c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area left bottom flag
        if (see_message[i].find("(f p l b)") != string::npos)
        {
            vector<string> penalty_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_left_bottom = {stof(penalty_left_bottom_coords[4]), stof(penalty_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right top flag
        if (see_message[i].find("(f p r t)") != string::npos)
        {
            vector<string> penalty_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_top = {stof(penalty_right_top_coords[4]), stof(penalty_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right center flag
        if (see_message[i].find("(f p r c)") != string::npos)
        {
            vector<string> penalty_right_center_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_center = {stof(penalty_right_center_coords[4]), stof(penalty_right_center_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p r c)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the penalty area right bottom flag
        if (see_message[i].find("(f p r b)") != string::npos)
        {
            vector<string> penalty_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_penalty_right_bottom = {stof(penalty_right_bottom_coords[4]), stof(penalty_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f p r b)")
            {
                player.seeing_zone = true;
            }
        }
    
        // Search for the goal left top flag
        if (see_message[i].find("(f g l t)") != string::npos)
        {
            vector<string> goal_left_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_top = {stof(goal_left_top_coords[4]), stof(goal_left_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f g l t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal left bottom flag
        if (see_message[i].find("(f g l b)") != string::npos)
        {
            vector<string> goal_left_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_left_bottom = {stof(goal_left_bottom_coords[4]), stof(goal_left_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f g l b)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right top flag
        if (see_message[i].find("(f g r t)") != string::npos)
        {
            vector<string> goal_right_top_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_top = {stof(goal_right_top_coords[4]), stof(goal_right_top_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f g r t)")
            {
                player.seeing_zone = true;
            }
        }

        // Search for the goal right bottom flag
        if (see_message[i].find("(f g r b)") != string::npos)
        {
            vector<string> goal_right_bottom_coords = separate_string_separator(see_message[i], " ");
            field.flag_goal_right_bottom = {stof(goal_right_bottom_coords[4]), stof(goal_right_bottom_coords[5])};
            player.flags_seen++;
            if (player.zone_name =="(f g r b)")
            {
                player.seeing_zone = true;
            }
        }
    }
}

// Return to zone function
string returnToZone(Player const &player)
{
    cout << "Not in zone" << endl;
    if (player.seeing_zone == false)
    {
        std::string rotate_command = "(turn " + to_string(50) + ")";
        return rotate_command;
    }
    else
    {
        if (player.flags_seen <= 3)
        {
            std::string dash_command = "(turn 180)";
            return dash_command;
        }
        else
        {
            std::string dash_command = "(dash 100 0)";
            return dash_command;
        }
        
    }
}

// Trilateration 2D function
vector<double> trilateration(vector<double> &P1, vector<double> &P2, vector<double> &P3, double &D1, double &D2, double &D3)
{
    // Calculate matrix A
    vector<double> A = {2*(P2[0]-P1[0]), 2*(P2[1]-P1[1]),
                        2*(P3[0]-P1[0]), 2*(P3[1]-P1[1])};

    // Calculate matrix B
    vector<double> B = {static_cast<double>(pow(D1, 2) - pow(D2, 2) + pow(P2[0], 2) - pow(P1[0], 2) + pow(P2[1], 2) - pow(P1[1], 2)),
                        static_cast<double>(pow(D1, 2) - pow(D3, 2) + pow(P3[0], 2) - pow(P1[0], 2) + pow(P3[1], 2) - pow(P1[1], 2))};

    // Calculate the inverse of A
    double det = A[0]*A[3] - A[1]*A[2];
    vector<double> A_inv = {A[3]/det, -A[1]/det,
                           -A[2]/det, A[0]/det};

    // Calculate the result
    vector<double> result = {A_inv[0]*B[0] + A_inv[1]*B[1],
                            A_inv[2]*B[0] + A_inv[3]*B[1]};

    return result;
}

// Calculate distance between two points
double distance(vector<double> &P1, vector<double> &P2)
{
    return sqrt(pow(P1[0] - P2[0], 2) + pow(P1[1] - P2[1], 2));
}

bool imInZone(Player const &player)
{
    if (player.x - player.zone.x < player.range && player.y - player.zone.y < player.range)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// moveToPosition function
string moveToZone(double &orientation, vector<double> const &P_player, Posicion const &P_zone)
{
    // Compute the angle between the player and the zone with respect to the orientation of the player
    double angle = atan2(P_zone.y - P_player[1], P_zone.x - P_player[0]) * 180 / M_PI;
    if (angle < 0)
    {
        angle += 360;
    }
    // Normalize theorientation
    if (orientation < 0)
    {
        while (orientation < 0)
        {
            orientation += 360;
        }
    }
    else if (orientation > 360)
    {
        while (orientation > 360)
        {
            orientation -= 360;
        }
    }
    // Compute the difference between the orientation of the player and the angle
    double diff = (angle - orientation) + 180;
    std::string dash_command = "(dash 2 " + to_string(diff) + ")"; 
    return dash_command;
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