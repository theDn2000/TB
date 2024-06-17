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

#include "functions.h"

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
    cout << "(init " + team_name + "(version 19))";

    udp_socket.sendTo("(init " + team_name + "(version 19))", other_recipient_udp);
    cout << "Init Message sent" << endl;

    std::size_t message_max_size = 1000000;
    cout << "Waiting for a message" << endl;
    auto received_message = udp_socket.receive(message_max_size);
    std::string received_message_content = received_message->received_message;

    // update upd port to provided by the other udp
    MinimalSocket::Address other_sender_udp = received_message->sender;
    MinimalSocket::Address server_udp = MinimalSocket::Address{"127.0.0.1", other_sender_udp.getPort()};

    // Create objects
    Player player{team_name, "", "", 0, 0, 0};
    Ball ball{"0", "0", "0", "0"};
    
    // parse the initial message
    player = parseInitialMessage(received_message_content, player);
    
    cout << player.unum << endl;
    sendInitialMoveMessage(player, udp_socket, server_udp);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        //cout << "Received message: " << received_message_content << endl;

        vector<string> parsed_message = separate_string(received_message_content);
        //cout << "Parsed message: " << parsed_message[0] << endl;

        // Search for see message if it is not in the first position don't parse it
        if (parsed_message[0].find("see") <= 5)
        {
            cout << "See message received" << endl;
            cout << "Parsed message: " << parsed_message[0] << endl;

            vector<string> see_message = separate_string(parsed_message[0]);


            // Find the "(b)" and save the position of the ball
            size_t ball_pos = 0;
            for (size_t i = 0; i < see_message.size(); i++)
            {
                if (see_message[i].find("(b)") <= 5)
                {
                    ball_pos = i;
                    break;
                }
            }
            vector<string> ball_coords = separate_string_separator(see_message[ball_pos], " ");
            ball.x = ball_coords[1];
            ball.y = ball_coords[2];

            cout << "Ball position: " << ball.x << " " << ball.y << endl;
            
        }
 
    }

return 0;
}