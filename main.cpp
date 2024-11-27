<<<<<<< HEAD:traffic_light/main.cpp
#include "car.hpp"
#include "traffic_light.hpp"
#include <vector>
#include <random>

=======
#include "header/Car.hpp"
#include "header/traffic_light.hpp"
#include <vector>
#include <random>



>>>>>>> 2ed4821b682b6e273a3556e0195e0a9fe3e90d93:main.cpp
void car_thread(std::vector<Car>& cars, Traffic_light& traffic_light, std::stop_token stop_token)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_time(2, 5);
<<<<<<< HEAD:traffic_light/main.cpp
    std::uniform_int_distribution<> dist_direction(0, 2); // 0: straight, 1: left, 2: right
=======
>>>>>>> 2ed4821b682b6e273a3556e0195e0a9fe3e90d93:main.cpp
    std::uniform_int_distribution<> dist_color(0, 255);

    float spawn_x = 450.f; // X-coordinate for vertical cars (middle of the screen)

    while (!stop_token.stop_requested())
    {
        // Generate a new car
        int wait_time = dist_time(gen);
        std::this_thread::sleep_for(std::chrono::seconds(wait_time));

        sf::Color random_color(dist_color(gen), dist_color(gen), dist_color(gen));
<<<<<<< HEAD:traffic_light/main.cpp
        Car car(spawn_x, 0.f, 90.f, 1.f, random_color); // Starting at Y = 0, moving vertically (90 degrees)
=======
        Car car(spawn_x, 0.f, 90.f, 0.6f, random_color); // Starting at Y = 0, moving vertically (90 degrees)
>>>>>>> 2ed4821b682b6e273a3556e0195e0a9fe3e90d93:main.cpp

        cars.push_back(std::move(car));
    }
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Traffic Simulation");
    std::stop_source stopping;

    // Traffic lights
    Traffic_light traffic_light_master{ Traffic_color::red };
    Traffic_light traffic_light_slave{ Traffic_color::red };

    std::jthread traffic_thread(run_traffic_light, std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    // Cars
    std::vector<Car> cars;
    std::jthread car_spawn_thread(car_thread, std::ref(cars), std::ref(traffic_light_slave), stopping.get_token());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                stopping.request_stop();
                window.close();
                return 0;
            }
        }

        // Update cars
        for (auto& car : cars)
        {
<<<<<<< HEAD:traffic_light/main.cpp
            // Stop or move based on traffic light
            if (traffic_light_slave.get_traffic_color() == Traffic_color::red && car.getY() > 300 && car.getY() < 400)
                car.stop();
            else
                car.resume();

            // Make decision after passing the traffic light
            if (!car.decisionMade() && car.getY() > 400)
            {
                car.makeDecision();

                // Randomly determine whether to turn left, right, or continue
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dist_direction(0, 2);

                int direction = dist_direction(gen);
                if (direction == 1)
                    car.turnLeft();
                else if (direction == 2)
                    car.turnRight();
            }

=======
            // Check if the car is at the TurnRightFromUp area
            
            if (car.getY() >= 800 && car.getX() >= 100 && car.getX() <= 450 && !car.decisionMade())
            {
                car.makeDecision();
                car.setPosition(450, 800); // Snap to the exact turning position
                car.turnRight();           // Turn right
            }
            else if (traffic_light_slave.get_traffic_color() == Traffic_color::red && car.getY() > 300 && car.getY() < 400)
            {
                // Stop the car at the traffic light
                car.stop();
            }
            else
            {
                // Resume movement if the light is green
                car.resume();
            }

            // Move the car along its path
>>>>>>> 2ed4821b682b6e273a3556e0195e0a9fe3e90d93:main.cpp
            car.move();
        }

        // Render
        window.clear(sf::Color::Black);

<<<<<<< HEAD:traffic_light/main.cpp
=======
        // Draw roads as lines
        // Horizontal roads
        sf::VertexArray horizontalRoad1(sf::Lines, 2);
        horizontalRoad1[0].position = sf::Vector2f(100, 450); // Start of the first horizontal road
        horizontalRoad1[1].position = sf::Vector2f(900, 450); // End of the first horizontal road
        window.draw(horizontalRoad1);

        sf::VertexArray horizontalRoad2(sf::Lines, 2);
        horizontalRoad2[0].position = sf::Vector2f(100, 550); // Start of the second horizontal road
        horizontalRoad2[1].position = sf::Vector2f(900, 550); // End of the second horizontal road
        window.draw(horizontalRoad2);

        // Vertical roads
        sf::VertexArray verticalRoad1(sf::Lines, 2);
        verticalRoad1[0].position = sf::Vector2f(500, 100); // Start of the first vertical road
        verticalRoad1[1].position = sf::Vector2f(500, 900); // End of the first vertical road
        window.draw(verticalRoad1);

        sf::VertexArray verticalRoad2(sf::Lines, 2);
        verticalRoad2[0].position = sf::Vector2f(450, 100); // Start of the second vertical road
        verticalRoad2[1].position = sf::Vector2f(450, 900); // End of the second vertical road
        window.draw(verticalRoad2);

        // Turns (visual indicator for TurnRightFromUp area)
        sf::VertexArray TurnRightFromUp(sf::Lines, 2);
        TurnRightFromUp[0].position = sf::Vector2f(100, 800); // Start of the turn section
        TurnRightFromUp[1].position = sf::Vector2f(450, 800); // End of the turn section
        TurnRightFromUp[0].color = sf::Color::Yellow;         // Highlighted with yellow
        TurnRightFromUp[1].color = sf::Color::Yellow;
        window.draw(TurnRightFromUp);

        // Draw cars
>>>>>>> 2ed4821b682b6e273a3556e0195e0a9fe3e90d93:main.cpp
        for (const auto& car : cars)
        {
            window.draw(car.getShape());
        }

        // Traffic light positions and rendering
        float busa = 370, busb = 600, radius = 30;
        sf::CircleShape F_verticale_D(radius);
        F_verticale_D.setOrigin(radius, radius);
        F_verticale_D.setPosition(busb, busb);

        sf::CircleShape F_horizontale_D(radius);
        F_horizontale_D.setOrigin(radius, radius);
        F_horizontale_D.setPosition(busb, busa);

        sf::CircleShape F_verticale_G(radius);
        F_verticale_G.setOrigin(radius, radius);
        F_verticale_G.setPosition(busa, busa);

        sf::CircleShape F_horizontal_G(radius);
        F_horizontal_G.setOrigin(radius, radius);
        F_horizontal_G.setPosition(busa, busb);

        F_verticale_D.setFillColor(get_SFML_color(traffic_light_slave));
        F_horizontale_D.setFillColor(get_SFML_color(traffic_light_master));
        F_verticale_G.setFillColor(get_SFML_color(traffic_light_slave));
        F_horizontal_G.setFillColor(get_SFML_color(traffic_light_master));

        window.draw(F_verticale_D);
        window.draw(F_horizontale_D);
        window.draw(F_verticale_G);
        window.draw(F_horizontal_G);
        window.display();
    }

    return 0;
}
