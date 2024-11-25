#pragma once
#include <SFML/Graphics.hpp>
#include <random>

class Car
{
public:
    Car(float x, float y, float angle, float speed, const sf::Color& color);

    float getX() const;
    float getY() const;
    float getAngle() const;

    void move();
    void stop();
    void resume();
    void turnLeft();
    void turnRight();

    sf::RectangleShape getShape() const;

private:
    float _x, _y;
    float _angle; // In degrees
    float _speed;
    sf::RectangleShape _shape;
    bool _stopped;
};
