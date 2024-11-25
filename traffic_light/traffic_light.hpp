#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <stop_token>
#include <chrono>

enum class Traffic_color
{
    green = 0,
    orange = 1,
    red = 2
};

Traffic_color operator++(Traffic_color& traffic_color);

class Traffic_light
{
private:
    Traffic_color traffic_color_;

public:
    explicit Traffic_light(const Traffic_color& traffic_color);
    void operator++();
    void set_traffic_color(const Traffic_color& traffic_color);
    const Traffic_color& get_traffic_color() const;
};

const sf::Color& get_SFML_color(const Traffic_light& traffic_light);
std::ostream& operator<<(std::ostream& os, const Traffic_light& traffic_light);
void run_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, std::stop_token stop_token);
void print_traffic_light(Traffic_light& traffic_light_master, Traffic_light& traffic_light_slave, std::stop_token stop_token);
