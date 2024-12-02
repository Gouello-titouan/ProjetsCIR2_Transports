#ifndef CAR_HPP
#define CAR_HPP

#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <stop_token>
#include "traffic_light.hpp"

class Car
{
public:
    Car(float x, float y, float angle, float speed, sf::Color color);

    float getX() const;
    float getY() const;
    float getAngle() const;
    void setPosition(float newX, float newY);

    bool distanceTo(const std::vector<Car>& otherCars) const;
    void move(const std::vector<Car>& otherCars);

    void turnLeft();
    void turnRight();
    void stop();
    void resume();
    bool isStopped() const;

    void light_is_red();
    void light_is_green();

    void followPath();
    void run(std::vector<Car>& cars, Traffic_light& traffic_light, std::stop_token stop_token);

    const sf::RectangleShape& getShape() const;

private:
    float _x, _y;
    float _angle;
    float _speed;
    bool _stopped;
    bool _light_is_red;
    bool _light_is_green;
    sf::RectangleShape _shape;

    enum class Direction { Straight, Left, Right };
    Direction _nextDirection;
};

#endif // CAR_HPP
