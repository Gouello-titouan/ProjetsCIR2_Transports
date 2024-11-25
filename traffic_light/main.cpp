#include "car.hpp"
#include "traffic_light.hpp"
#include <vector>
#include <random>

void car_thread(std::vector<Car>& cars, Traffic_light& traffic_light, std::stop_token stop_token)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_time(2, 5);
    std::uniform_int_distribution<> dist_direction(0, 2); // 0: straight, 1: left, 2: right
    std::uniform_int_distribution<> dist_color(0, 255);

    float spawn_y = 370.f; // Y-coordinate of line1

    while (!stop_token.stop_requested())
    {
        // Generate a new car
        int wait_time = dist_time(gen);
        std::this_thread::sleep_for(std::chrono::seconds(wait_time));

        sf::Color random_color(dist_color(gen), dist_color(gen), dist_color(gen));
        Car car(0.f, spawn_y, 0.f, 1.f, random_color);

        // Determine direction
        int direction = dist_direction(gen);
        if (direction == 1)
            car.turnLeft();
        else if (direction == 2)
            car.turnRight();

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
            if (traffic_light_slave.get_traffic_color() == Traffic_color::red && car.getX() > 300 && car.getX() < 400)
                car.stop();
            else
                car.resume();

            car.move();
        }

        // Render
        window.clear(sf::Color::Black);

        for (const auto& car : cars)
        {
            window.draw(car.getShape());
        }

        window.display();
    }

    return 0;
}
