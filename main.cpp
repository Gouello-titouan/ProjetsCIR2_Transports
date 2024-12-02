#include "Header/Car.hpp"
#include "Header/traffic_light.hpp"
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <stop_token>
#include <memory>


std::mutex cars_mutex;

// Function to spawn cars in a separate thread
void car_spawn_thread(std::vector<std::unique_ptr<std::jthread>>& car_threads,
    std::vector<Car>& cars,
    Traffic_light& traffic_light,
    std::stop_token stop_token,
    std::mutex& cars_mutex)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_time(2, 5);  // Délai aléatoire pour générer des voitures
    std::uniform_int_distribution<> dist_color(0, 255); // Couleur aléatoire des voitures

    float spawn_x = 450.f;

    while (!stop_token.stop_requested()) {
        // Délai avant de faire apparaître une nouvelle voiture
        int wait_time = dist_time(gen);
        std::this_thread::sleep_for(std::chrono::seconds(wait_time));

        // Création de la voiture avec une couleur aléatoire
        sf::Color random_color(dist_color(gen), dist_color(gen), dist_color(gen));
        Car car(spawn_x, 0.f, 90.f, 20.f, random_color);

        // Verrouiller l'accès à cars pour éviter les conflits entre threads
        {
            std::lock_guard<std::mutex> lock(cars_mutex);
            cars.push_back(std::move(car));  // Ajouter la voiture à la liste
        }

        // Démarrer un thread pour cette voiture
        car_threads.push_back(std::make_unique<std::jthread>( [&car = cars.back(), &cars, &cars_mutex, &traffic_light](std::stop_token stop_token) {
                {
                    std::lock_guard<std::mutex> lock(cars_mutex); 
                    car.run(cars, traffic_light, stop_token);    
                }
            }
        ));
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

    std::vector<std::unique_ptr<std::jthread>> car_threads;
    std::vector<Car> cars;

    std::jthread spawn_thread(car_spawn_thread, std::ref(car_threads), std::ref(cars), std::ref(traffic_light_slave), stopping.get_token());

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stopping.request_stop();
                window.close();
                return 0;
            }
        }

        // Render the scene
        window.clear(sf::Color::Black);

        // Draw roads
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

        // Visual indicator for "Turn Right" area
        sf::VertexArray TurnRightFromUp(sf::Lines, 2);
        TurnRightFromUp[0].position = sf::Vector2f(100, 800); // Start of the turn section
        TurnRightFromUp[1].position = sf::Vector2f(450, 800); // End of the turn section
        TurnRightFromUp[0].color = sf::Color::Yellow;         // Highlighted with yellow
        TurnRightFromUp[1].color = sf::Color::Yellow;
        window.draw(TurnRightFromUp);

        // Draw cars
        cars_mutex.lock();
        for (const auto& car : cars) {
            window.draw(car.getShape());
        }
        cars_mutex.unlock();

        // Traffic light positions and rendering (using circle shapes)
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
