#include "../header/Car.hpp"
#include "../Header/traffic_light.hpp"
#include <iostream>
#include <thread>
#include <mutex>

#define safety_distance  50.f

Car::Car(float x, float y, float angle, float speed, sf::Color color): _x(x), _y(y), _angle(angle), _speed(speed), _stopped(false), _light_is_green(true), _light_is_red(false)
{
    _shape.setSize(sf::Vector2f(30.f, 15.f));
    _shape.setFillColor(color);
    _shape.setOrigin(_shape.getSize().x / 2, _shape.getSize().y / 2);
    _shape.setPosition(_x, _y);
    _shape.setRotation(_angle);
}

float Car::getX() const { return _x; }
float Car::getY() const { return _y; }
float Car::getAngle() const { return _angle; }

void Car::setPosition(float newX, float newY)
{
    _x = newX;
    _y = newY;
    _shape.setPosition(_x, _y);
}

bool Car::distanceTo(const std::vector<Car>& otherCars) const
{
    for (const auto& car : otherCars)
    {
        if (this == &car) continue;

        float dx = car.getX() - _x;
        float dy = car.getY() - _y;
        float distance = std::sqrt(dx * dx + dy * dy);

        if (distance < safety_distance)
        {
            return true;
        }
    }
    return false;
}

void Car::move(const std::vector<Car>& otherCars)
{
    if (_light_is_green && !distanceTo(otherCars))
    {
        followPath();
        _shape.setPosition(_x, _y);
    }
    if (_light_is_red)
    {
        stop();
    }
}

void Car::turnLeft()
{
    _angle += 90.f;
    _nextDirection = Direction::Left;
    _shape.setRotation(_angle);
}

void Car::turnRight()
{
    _angle -= 90.f;
    _nextDirection = Direction::Right;
    _shape.setRotation(_angle);
}

void Car::stop() { _stopped = true; }
void Car::resume() { _stopped = false; }
bool Car::isStopped() const { return _stopped; }

void Car::light_is_red()
{
    _light_is_red = true;
    _light_is_green = false;
}

void Car::light_is_green()
{
    _light_is_green = true;
    _light_is_red = false;
}

void Car::followPath()
{
    _x += cos(_angle * M_PI / 180.0) * _speed;
    _y += sin(_angle * M_PI / 180.0) * _speed;
}

const sf::RectangleShape& Car::getShape() const { return _shape; }

void Car::run(std::vector<Car>& cars, Traffic_light& traffic_light, std::stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        //std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
        if (traffic_light.get_traffic_color() == Traffic_color::red && getY() > 300 && getY() < 400)
        {
            light_is_red();
            std::cout << "Traffic light is red at " << getX() << ", " << getY() << "\n";
        }
        else if (traffic_light.get_traffic_color() == Traffic_color::green && getY() > 300 && getY() < 400)
        {
            light_is_green();
            resume();
            std::cout << "Traffic light is green at " << getX() << ", " << getY() << "\n";
        }

        move(cars);
    }
}

