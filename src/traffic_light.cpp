#include "../header/traffic_light.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;


// Define the increment operator for Traffic_color
Traffic_color operator++(Traffic_color& traffic_color)
{
    switch (traffic_color)
    {
    case Traffic_color::red:
        traffic_color = Traffic_color::green;
        break;

    case Traffic_color::green:
        std::this_thread::sleep_for(3s); // Transit time for orange
        traffic_color = Traffic_color::orange;
        std::this_thread::sleep_for(3s); // Stay on orange for a while
        traffic_color = Traffic_color::red;
        break;

    case Traffic_color::orange:
        traffic_color = Traffic_color::red;
        break;
    }
    return traffic_color;
}

// Constructor
Traffic_light::Traffic_light(const Traffic_color& traffic_color) : traffic_color_{ traffic_color } {}

// Increment traffic light state
void Traffic_light::operator++()
{
    ++traffic_color_;
}

// Set traffic light color
void Traffic_light::set_traffic_color(const Traffic_color& traffic_color)
{
    traffic_color_ = traffic_color;
}

// Get traffic light color
const Traffic_color& Traffic_light::get_traffic_color() const
{
    return traffic_color_;
}

// Convert traffic light color to SFML color
const sf::Color& get_SFML_color(const Traffic_light& traffic_light)
{
    static const sf::Color Orange(255, 165, 0);
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::orange:
        return Orange;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return sf::Color::Black; // Default fallback
}

// Print traffic light color to ostream
std::ostream& operator<<(std::ostream& os, const Traffic_light& traffic_light)
{
    std::string color_name;
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::red:
        color_name = "Red";
        break;
    case Traffic_color::green:
        color_name = "Green";
        break;
    case Traffic_color::orange:
        color_name = "Orange";
        break;
    }
    os << color_name;
    return os;
}

// Run traffic lights in a loop, switching between master and slave
void run_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, std::stop_token stop_token)
{
    // Initial state
    traffic_light_master.set_traffic_color(Traffic_color::green);
    traffic_light_slave.set_traffic_color(Traffic_color::red);

    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(8s); // Waiting time for green light
        if (traffic_light_master.get_traffic_color() == Traffic_color::green)
        {
            ++traffic_light_master;
            ++traffic_light_slave;
        }
        else
        {
            ++traffic_light_slave;
            ++traffic_light_master;
        }
    }
}

// Print traffic lights to the console periodically
void print_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, std::stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(1s);
        std::cout << "Traffic light master: " << traffic_light_master
            << " | Traffic light slave: " << traffic_light_slave << std::endl;
    }
}
