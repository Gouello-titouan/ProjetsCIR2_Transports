#include "Car.hpp"

Car::Car(float x, float y, float angle, float speed, sf::Color color)
    : _x(x), _y(y), _angle(angle), _speed(speed), _stopped(false), _decision_made(false)
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

void Car::move()
{
    if (!_stopped)
    {
        _x += static_cast<float>(cos(_angle * M_PI / 180.0) * _speed);
        _y += static_cast<float>(sin(_angle * M_PI / 180.0) * _speed);
        _shape.setPosition(_x, _y);
    }
}

void Car::turnLeft() { _angle += 90.f; _shape.setRotation(_angle); }
void Car::turnRight() { _angle -= 90.f; _shape.setRotation(_angle); }
void Car::stop() { _stopped = true; }
void Car::resume() { _stopped = false; }

bool Car::decisionMade() const { return _decision_made; }
void Car::makeDecision() { _decision_made = true; }

const sf::RectangleShape& Car::getShape() const { return _shape; }
