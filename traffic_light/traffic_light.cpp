// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

#ifdef _MSC_VER 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#define _PATH_IMG_ "C:/Program Files/SFML/img/"
#else
#define _PATH_IMG_ "../img/"
#endif

const std::string path_image(_PATH_IMG_);


using namespace std::chrono_literals;

static const sf::Color Orange(255, 165, 0);//dzfsytfytf

const auto time_transit = 3s;
const auto time_waiting = 8s;


enum class Traffic_color
{
    green = 0,
    orange = 1,
    red = 2
};

Traffic_color operator++(Traffic_color &traffic_color)
{
    switch (traffic_color)
    {
    case Traffic_color::red:
        traffic_color = Traffic_color::green;
        break;

    case Traffic_color::green:
        std::this_thread::sleep_for(time_transit);
        traffic_color = Traffic_color::orange;
        std::this_thread::sleep_for(time_transit);
        traffic_color = Traffic_color::red;
    }
    return traffic_color;
}

class Traffic_light
{
private:
    Traffic_color traffic_color_;

public:
    explicit Traffic_light(const Traffic_color &traffic_color) : traffic_color_{traffic_color} {}
    void operator++()
    {
        ++traffic_color_;
    }
    void set_traffic_color(const Traffic_color &traffic_color)
    {
        traffic_color_ = traffic_color;
    }
    const Traffic_color &get_traffic_color() const
    {
        return traffic_color_;
    }
};

const sf::Color &get_SFML_color(const Traffic_light &traffic_light)
{
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::green:
        return sf::Color::Green;
    case Traffic_color::red:
        return sf::Color::Red;
    }
    return Orange;
}

std::ostream &operator<<(std::ostream &os, const Traffic_light &traffic_light)
{
    std::string ptr;
    switch (traffic_light.get_traffic_color())
    {
    case Traffic_color::red:
        ptr = "Red";
        break;
    case Traffic_color::green:
        ptr = "Green";
        break;
    case Traffic_color::orange:
        ptr = "Orange";
    }
    os << ptr;
    return os;
}

void run_traffic_light(Traffic_light &traffic_light_master, Traffic_light &traffic_light_slave, std::stop_token stop_token)
{
    traffic_light_master.set_traffic_color(Traffic_color::green);
    traffic_light_slave.set_traffic_color(Traffic_color::red);
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(time_waiting);
        if (traffic_light_master.get_traffic_color() == Traffic_color::green)
        {
            ++traffic_light_master;
            ++traffic_light_slave;
        }
        else
        {
            ++traffic_light_slave;
            ++traffic_light_master;
        }
    }
}

void print_traffic_light(Traffic_light &traffic_light_master, Traffic_light &traffic_light_slave, std::stop_token stop_token)
{
    while (!stop_token.stop_requested())
    {
        std::this_thread::sleep_for(1s);
        std::cout << "Taffic light master : " << traffic_light_master << " Taffic light slave : " << traffic_light_slave << std::endl;
    }
}

int main()
{

    // Fond d'écran
    /*Texture backgroundImage;
    Sprite backgroundSprite;

    if (!backgroundImage.loadFromFile(path_image + "carrefour.jpg")) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }

    backgroundSprite.setTexture(backgroundImage);*/
    



    std::stop_source stopping;
    Traffic_light traffic_light_master{Traffic_color::red};
    Traffic_light traffic_light_slave{Traffic_color::red};
    std::jthread thread_traffic_light_master(run_traffic_light,
                                             std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
                                     std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "My window");

    float busa = 370, busb = 600, size = 1000, radius = 10;
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(0, busa)), sf::Vertex(sf::Vector2f(size, busa))};
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(0, busb)), sf::Vertex(sf::Vector2f(size, busb))};

    sf::Vertex line3[] = {sf::Vertex(sf::Vector2f(busa, 0)), sf::Vertex(sf::Vector2f(busa, size))};
    sf::Vertex line4[] = {sf::Vertex(sf::Vector2f(busb, 0)), sf::Vertex(sf::Vector2f(busb, size))};


    sf::CircleShape circle1(radius);
    circle1.setFillColor(sf::Color::Blue);
    circle1.setOrigin(circle1.getRadius() / 2, circle1.getRadius() / 2);
    circle1.setPosition(busb + radius / 2, busb + radius / 2);
    sf::CircleShape circle2(radius);
    circle2.setFillColor(sf::Color::Green);
    circle2.setOrigin(circle2.getRadius() / 2, circle2.getRadius() / 2);
    circle2.setPosition(busb + radius / 2, busa - radius);

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
        window.clear(sf::Color::Black);

        //window.draw(backgroundSprite);

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);
        circle1.setFillColor(get_SFML_color(traffic_light_slave));
        circle2.setFillColor(get_SFML_color(traffic_light_master));
        window.draw(circle1);
        window.draw(circle2);

        window.display();
    }

    return 0;
}