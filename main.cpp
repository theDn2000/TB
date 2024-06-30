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
#include <string>
#include <algorithm>
#include <random>

// Include headers
#include "functions.h"
#include "structs.h"






struct Point2D {
    double x;
    double y;
};

struct Particle {
    Point2D position;
    double weight;
};

class MonteCarloLocalization {
public:
    MonteCarloLocalization(int num_particles, const std::vector<Point2D>& flags, double sensor_noise)
        : num_particles(num_particles), flags(flags), sensor_noise(sensor_noise) {
        // Initialize particles randomly in the field
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_x(is_left_side ? 0.0 : 52.5, is_left_side ? 52.5 : 105.0);
        std::uniform_real_distribution<> dis_y(0.0, 68.0); // Height of the soccer field

        for (int i = 0; i < num_particles; ++i) {
            particles.push_back({{dis_x(gen), dis_y(gen)}, 1.0 / num_particles});
        }
    }

    void update_with_motion(double dx, double dy) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> motion_noise_x(0.0, 1.0); // Motion noise
        std::normal_distribution<> motion_noise_y(0.0, 1.0);

        for (auto& particle : particles) {
            particle.position.x += dx + motion_noise_x(gen);
            particle.position.y += dy + motion_noise_y(gen);
        }
    }

    void update_with_measurement(const std::vector<std::pair<Point2D, double>>& observations) {
        for (auto& particle : particles) {
            particle.weight = 1.0;
            for (const auto& obs : observations) {
                const auto& landmark = obs.first;
                double measured_dist = obs.second;
                double predicted_dist = distance(particle.position, landmark);
                double weight = gaussian(measured_dist, sensor_noise, predicted_dist);
                particle.weight *= weight;
            }
        }
        normalize_weights();
    }

    void resample_particles() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::vector<Particle> new_particles;
        std::uniform_real_distribution<> dis(0.0, 1.0);

        std::vector<double> cumulative_sum;
        double sum = 0.0;
        for (const auto& particle : particles) {
            sum += particle.weight;
            cumulative_sum.push_back(sum);
        }

        for (int i = 0; i < num_particles; ++i) {
            double sample = dis(gen) * sum;
            auto it = std::lower_bound(cumulative_sum.begin(), cumulative_sum.end(), sample);
            int index = it - cumulative_sum.begin();
            new_particles.push_back(particles[index]);
        }
        particles = new_particles;
    }

    Point2D estimate_position() const {
        double x_sum = 0.0, y_sum = 0.0, weight_sum = 0.0;
        for (const auto& particle : particles) {
            x_sum += particle.position.x * particle.weight;
            y_sum += particle.position.y * particle.weight;
            weight_sum += particle.weight;
        }
        return {x_sum / weight_sum, y_sum / weight_sum};
    }

private:
    int num_particles;
    double sensor_noise;
    std::vector<Point2D> flags;
    std::vector<Particle> particles;

    double distance(const Point2D& a, const Point2D& b) const {
        return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    double gaussian(double mu, double sigma, double x) const {
        return std::exp(-0.5 * std::pow((x - mu) / sigma, 2.0)) / (sigma * std::sqrt(2.0 * M_PI));
    }

    void normalize_weights() {
        double sum = 0.0;
        for (const auto& particle : particles) {
            sum += particle.weight;
        }
        for (auto& particle : particles) {
            particle.weight /= sum;
        }
    }
};























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
    

    const std::vector<Point2D> flags = 
    {
        {0.0, 0.0},        // Center of the field
        {0.0, -34},       // Top center
        {0.0, 34},       // Bottom center
        {-52.5, -34},    // Corner top-left
        {-52.5, 34},     // Corner bottom-left
        {52.5, -34},     // Corner top-right
        {52.5, 34},      // Corner bottom-right
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

    // Inicialización del sistema MCL
    double sensor_noise = 5.0;
    MonteCarloLocalization mcl(1000, flags, sensor_noise);

    // Reference vector to the flags
    //td::vector<std::reference_wrapper<std::vector<float>>> flags = {std::ref(field.flag_center), std::ref(field.flag_center_top), std::ref(field.flag_center_bottom), std::ref(field.flag_left_top), std::ref(field.flag_left_bottom), std::ref(field.flag_right_top), std::ref(field.flag_right_bottom)};
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
        auto received_message = udp_socket.receive(message_max_size);
        std::string received_message_content = received_message->received_message;

        vector<string> parsed_message = separate_string(received_message_content);

        

        // Search for see message
        if (parsed_message[0].find("see") <= 5)
        {
            vector<string> see_message = separate_string(parsed_message[0]);
            store_data_see(see_message, player, ball, own_goal, opponent_goal, field);
        


            // Trilateration
            vector<vector<double>> flags_rel = {field.flag_center, field.flag_center_top, field.flag_center_bottom, field.flag_left_top, field.flag_left_bottom, field.flag_right_top, field.flag_right_bottom, field.flag_penalty_left_top, field.flag_penalty_left_center, field.flag_penalty_left_bottom, field.flag_penalty_right_top, field.flag_penalty_right_center, field.flag_penalty_right_bottom, field.flag_goal_left_top, field.flag_goal_left_bottom, field.flag_goal_right_top, field.flag_goal_right_bottom};
            //vector<vector<double>> flags_abs = {field.flag_center_abs, field.flag_center_top_abs, field.flag_center_bottom_abs, field.flag_left_top_abs, field.flag_left_bottom_abs, field.flag_right_top_abs, field.flag_right_bottom_abs, field.flag_penalty_left_top_abs, field.flag_penalty_left_center_abs, field.flag_penalty_left_bottom_abs, field.flag_penalty_right_top_abs, field.flag_penalty_right_center_abs, field.flag_penalty_right_bottom_abs, field.flag_goal_left_top_abs, field.flag_goal_left_bottom_abs, field.flag_goal_right_top_abs, field.flag_goal_right_bottom_abs};


            if (player.flags_seen >= 3) // Trilateration can be calculated with 3 flags
            {
                cout << "The player sees 3 flags or more" << endl;
                int flags_used = 0;
                Point2D P1 = {0, 0};
                Point2D P2 = {0, 0};
                Point2D P3 = {0, 0};
                double D1 = 0;
                double D2 = 0;
                double D3 = 0;
                // Recorre todas las variables de la estructura field
                for (int i = 0; i < flags_rel.size(); i++)
                {
                    // If the flag coordinates are (999, 999) then the flag is not seen
                    if (flags_rel[i][0] != 999 && flags_used < 3) 
                    {
                        flags_used++;
                        if (flags_used == 1)
                        {
                            D1 = sqrt(pow(flags_rel[i][0], 2) + pow(flags_rel[i][1], 2));
                            P1 = flags[i];
                            cout << "D1: " << D1 << endl;
                        }
                        else if (flags_used == 2)
                        {
                            D2 = sqrt(pow(flags_rel[i][0], 2) + pow(flags_rel[i][1], 2));
                            P2 = flags[i];
                            cout << "D2: " << D2 << endl;
                        }
                        else if (flags_used == 3)
                        {
                            D3 = sqrt(pow(flags_rel[i][0], 2) + pow(flags_rel[i][1], 2));
                            P3 = flags[i];
                            cout << "D3: " << D3 << endl;
                        }
                    }
                }
                if (flags_used == 3)
                {
                    std::vector<std::pair<Point2D, double>> observations = {
                    {P1, D1},
                    {P2, D2},
                    {P3, D3}
                    };
                    /*
                    cout << "P1_tri: " << P1[0] << " " << P1[1] << endl;
                    cout << "D1: " << D1 << endl;
                    vector<double> result = trilateration(P1, P2, P3, D1, D2, D3);
                    cout << "Trilateration result: " << result[0] << " " << result[1] << endl;
                    */
                    // Simulate robot movement
                    mcl.update_with_motion(2.0, 2.0);

                    // Update based on measurement
                    mcl.update_with_measurement(observations);

                    // Resample particles
                    mcl.resample_particles();

                    // Estimate position
                    Point2D estimated_pos = mcl.estimate_position();
                    std::cout << "Estimated position: (" << estimated_pos.x << ", " << estimated_pos.y << ")\n";
                }
            }



            // Logic of the player
            if (player.see_ball == true)
            {
                cout << "The player sees the ball" << endl;
                switch (player.unum)
                {
                case 1:
                    // Goalkeeper
                    // Dash
                    int power = 100;
                    std::string dash_command = "(dash " + to_string(power) + " 0)";
                    udp_socket.sendTo(dash_command, server_udp);
                    cout << "Dash command sent" << endl;


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
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
return 0;
}