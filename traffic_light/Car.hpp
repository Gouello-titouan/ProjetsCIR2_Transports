#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <SFML/Graphics.hpp>

enum class Direction { None, Left, Right, Straight };

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
    void makeDecision();
    void followPath();
    bool decisionMade() const;

    const sf::RectangleShape& getShape() const;

private:
    float _x, _y;
    float _angle;
    float _speed;
    bool _stopped;
    bool _decision_made; // Has the car made a decision after the traffic light?
    Direction _nextDirection; // The direction the car will take (left, right, straight)
    sf::RectangleShape _shape;
};
