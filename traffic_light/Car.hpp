#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Graphics.hpp>

class Car
{
public:
    Car(float x, float y, float angle, float speed, sf::Color color);

    float getX() const;
    float getY() const;
    float getAngle() const;

    void move();
    void turnLeft();
    void turnRight();
    void stop();
    void resume();

    bool decisionMade() const;
    void makeDecision();

    const sf::RectangleShape& getShape() const;

private:
    float _x, _y;
    float _angle;
    float _speed;
    bool _stopped;
    bool _decision_made; // Has the car made a decision after the traffic light?
    sf::RectangleShape _shape;
};
