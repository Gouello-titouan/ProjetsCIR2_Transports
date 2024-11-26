#include "../Header/Car.hpp"

Car::Car(float x, float y, float angle, float speed, sf::Color color)
    : _x(x), _y(y), _angle(angle), _speed(speed), _stopped(false), _decision_made(false), _nextDirection(Direction::Straight)
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

void Car::move()
{
    if (!_stopped)
    {
        // Si la voiture doit suivre un chemin défini
        followPath();
        _shape.setPosition(_x, _y);
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

bool Car::decisionMade() const { return _decision_made; }
void Car::makeDecision() { _decision_made = true; }

void Car::followPath()
{
    if (_nextDirection == Direction::Straight)
    {
        // Déplacer la voiture suivant son angle actuel
        _x += cos(_angle * M_PI / 180.0) * _speed;
        _y += sin(_angle * M_PI / 180.0) * _speed;
    }
    else if (_nextDirection == Direction::Left)
    {
        // Déplacer la voiture à gauche (90°)
        _x += cos((_angle + 90.f) * M_PI / 180.0) * _speed;
        _y += sin((_angle + 90.f) * M_PI / 180.0) * _speed;
    }
    else if (_nextDirection == Direction::Right)
    {
        // Déplacer la voiture à droite (-90°)
        _x += cos((_angle - 90.f) * M_PI / 180.0) * _speed;
        _y += sin((_angle - 90.f) * M_PI / 180.0) * _speed;
    }
}

const sf::RectangleShape& Car::getShape() const { return _shape; }
