#include "car.hpp"
#include <cmath>

Car::Car(float x, float y, float angle, float speed, const sf::Color& color)
    : _x(x), _y(y), _angle(angle), _speed(speed), _stopped(false)
{
    _shape.setSize({ 30, 15 }); // Car dimensions
    _shape.setOrigin(_shape.getSize() / 2.f);
    _shape.setFillColor(color);
    _shape.setPosition(_x, _y);
    _shape.setRotation(_angle);
}

float Car::getX() const { return _x; }
float Car::getY() const { return _y; }
float Car::getAngle() const { return _angle; }

void Car::move()
{
    if (!_stopped)
    {
        _x += std::cos(_angle * M_PI / 180.0f) * _speed;
        _y += std::sin(_angle * M_PI / 180.0f) * _speed;
        _shape.setPosition(_x, _y);
    }
}

void Car::stop()
{
    _stopped = true;
}

void Car::resume()
{
    _stopped = false;
}

void Car::turnLeft()
{
    _angle -= 90.f; // Turn left
    _shape.setRotation(_angle);
}

void Car::turnRight()
{
    _angle += 90.f; // Turn right
    _shape.setRotation(_angle);
}

sf::RectangleShape Car::getShape() const
{
    return _shape;
}
