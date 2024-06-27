#include <iostream>

using namespace std;

#include <MinimalSocket/udp/UdpSocket.h>
//#include "stringutils.h"
//#include "types.h"
//#include "parsemessages.h"
//#include "tictoc.h"
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <cmath>

// Include headers
#include "functions.h"
#include "structs.h"

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


// main with two args
int main(int argc, char *argv[])
{
    // check if the number of arguments is correct
    if (argc != 3)
    {
        cout << "Usage: " << argv[0] << " <team-name> <this-port>" << endl;
        return 1;
    }

    // get the team name and the port
    string team_name = argv[1];
    MinimalSocket::Port this_socket_port = std::stoi(argv[2]);

    cout << "Creating a UDP socket" << endl;

    MinimalSocket::udp::Udp<true> udp_socket(this_socket_port, MinimalSocket::AddressFamily::IP_V6);

    cout << "Socket created" << endl;

    bool success = udp_socket.open();

    if (!success)
    {
        cout << "Error opening socket" << endl;
        return 1;
    }

    MinimalSocket::Address other_recipient_udp = MinimalSocket::Address{"127.0.0.1", 6000};
    cout << "(init " + team_name + "(version 15))";

    udp_socket.sendTo("(init " + team_name + "(version 15))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // update upd port to provided by the other udp
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    // Create objects
    Player player{team_name, "", "", false, 0, 0, 0};
    Ball ball{"0", "0", "0", "0"};
    Goal own_goal{"0", "0", "init"};
    Goal opponent_goal{"0", "0", "init"};
    Field field;
    
    // Reference vector to the flags
    std::vector<std::reference_wrapper<std::vector<float>>> flags = {std::ref(field.flag_center), std::ref(field.flag_center_top), std::ref(field.flag_center_bottom), std::ref(field.flag_left_top), std::ref(field.flag_left_bottom), std::ref(field.flag_right_top), std::ref(field.flag_right_bottom)};
    // parse the initial message
    player = parseInitialMessage(received_message_content, player);
    
    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

    // Configure the goals
    if (player.side == "r")
    {
        opponent_goal.side = "l";
        own_goal.side = "r";
    }
    else
    {
        opponent_goal.side = "r";
        own_goal.side = "l";
    }

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        vector<string> parsed_message = separate_string(received_message_content);

        

        // Search for see message
        if (parsed_message[0].find("see") <= 5)
        {
            vector<string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, own_goal, opponent_goal, field);
        
            // Trilateration
            if (player.flags_seen >= 3) // Trilateration can be calculated with 3 flags
            {
                cout << "The player sees 3 flags or more" << endl;
                int flags_used = 0;
                vector<float> P1;
                vector<float> P2;
                vector<float> P3;
                // Recorre todas las variables de la estructura field
                for (auto &flag : flags)
                {
                    // If the flag coordinates are (999, 999) then the flag is not seen
                    if (flag.get()[0] != 999)
                    {
                        flags_used++;
                        if (flags_used == 1)
                        {
                            P1 = {flag.get()[0], flag.get()[1]};
                        }
                        else if (flags_used == 2)
                        {
                            P2 = {flag.get()[0], flag.get()[1]};
                        }
                        else if (flags_used == 3)
                        {
                            P3 = {flag.get()[0], flag.get()[1]};
                        }
                    }
                }
                if (flags_used == 3)
                {
                    vector<float> result = trilateration(P1, P2, P3);
                    cout << "Trilateration result: " << result[0] << " " << result[1] << endl;
                }
            }

            // Logic of the player
            if (player.see_ball == true)
            {
                switch (player.unum)
                {
                case 1:
                    // Goalkeeper
                    if (ball.distance < 1.5)
                    {
                        // Kick the ball
                        int power = 100;
                        std::string kick_command = "(kick " + to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else if (ball.distance < 20)
                    {
                        // Dash to the ball
                        int power = 100;
                        std::string dash_command = "(dash " + to_string(power) + " 0)";
                        udp_socket.sendTo(dash_command, server_udp);
                    }
                    else
                    {
                        // Rotate because he mad
                        std::string rotate_command = "(turn " + to_string(30) + ")";
                    }

                    break;
                default:
                    if (ball.distance < 1.5)
                    {
                        // Kick the ball
                        int power = 100;
                        std::string kick_command = "(kick " + to_string(power) + " 0)";
                        udp_socket.sendTo(kick_command, server_udp);
                    }
                    else
                    {
                        int i = 0;
                        if (abs(ball.angle) >= 10)
                        {
                            int division = 1;
                            if (ball.distance < 6)
                            {
                                division = 20;
                            }
                            else
                            {
                                division = 5;
                            }
                            // Rotate the player to the ball
                            std::string rotate_command = "(turn " + to_string(ball.angle / division) + ")";
                            udp_socket.sendTo(rotate_command, server_udp);
                        }

                        else
                        {
                            int power = 100;
                            if (ball.distance < 3)
                            {
                                power = 60;
                            }
                            else if (ball.distance < 7)
                            {
                                power = 80;
                            }
                            // In this moment, the player should be looking to the ball
                            // Create the dash command
                            std::string dash_command = "(dash " + to_string(power) + " 0)";
                            udp_socket.sendTo(dash_command, server_udp);
                        }
                    }
                    break;
                }

            // Saber donde estan las porterias y chutar hacia ellas en el caso de llegar a la pelota [2]

            // Que corran hacia la pelota mientras giran (IDEA) [3]

            // Revisar codigo y optimizar [4]
            }
            else
            {
                cout << "----------------" << endl;
                // Rotate to find the ball
                if (player.y < 0)
                {
                    std::string rotate_command = "(turn " + to_string(-80) + ")";
                    udp_socket.sendTo(rotate_command, server_udp);
                }
                else
                {
                    std::string rotate_command = "(turn " + to_string(80) + ")";
                    udp_socket.sendTo(rotate_command, server_udp);
                }
            }
        }
    }
return 0;
}