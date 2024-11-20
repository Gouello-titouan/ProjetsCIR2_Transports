// condition_variable::wait (with predicate)
#include <iostream> // std::cout
#include <thread>   // std::thread, std::this_thread::yield
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


/*
F_verticale_D = Feu Vericalte Droit
F_verticale_G = Feu Vericalte Gauche 

F_horizontale_D = Feu Horizontale Droit
F_horizontale_G = Feu Horizontale Gauche

*/

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

static const sf::Color Orange(255, 165, 0);

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

    /* Image feu triclore
    Texture trafficlight;
    Sprite trafficlightspirite;

    if (!trafficlight.loadFromFile(path_image + "trafficlight.png")) {
        cerr << "Erreur pendant le chargement des images" << endl;
        return EXIT_FAILURE; // On ferme le programme
    }

    trafficlightspirite.setTexture(trafficlight);*/
    



    std::stop_source stopping;
    Traffic_light traffic_light_master{Traffic_color::red};
    Traffic_light traffic_light_slave{Traffic_color::red};
    std::jthread thread_traffic_light_master(run_traffic_light,
                                             std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    std::jthread write_traffic_light(print_traffic_light,
                                     std::ref(traffic_light_master), std::ref(traffic_light_slave), stopping.get_token());

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "My window");

    float busa = 370, busb = 600, busc = 615, busd = 365, size = 1000, radius = 10;
    sf::Vertex line1[] = {sf::Vertex(sf::Vector2f(0, busa)), sf::Vertex(sf::Vector2f(size, busa))};
    sf::Vertex line2[] = {sf::Vertex(sf::Vector2f(0, busb)), sf::Vertex(sf::Vector2f(size, busb))};
    //sf::Vertex line5[] = { sf::Vertex(sf::Vector2f(0, busc)), sf::Vertex(sf::Vector2f(size, busc)) };
    //sf::Vertex line6[] = { sf::Vertex(sf::Vector2f(0, busd)), sf::Vertex(sf::Vector2f(size, busd)) };

    sf::Vertex line3[] = {sf::Vertex(sf::Vector2f(busa, 0)), sf::Vertex(sf::Vector2f(busa, size))};
    sf::Vertex line4[] = {sf::Vertex(sf::Vector2f(busb, 0)), sf::Vertex(sf::Vector2f(busb, size))};
   /* sf::Vertex line7[] = { sf::Vertex(sf::Vector2f(busc, 0)), sf::Vertex(sf::Vector2f(busc, size)) };
    sf::Vertex line8[] = { sf::Vertex(sf::Vector2f(busd, 0)), sf::Vertex(sf::Vector2f(busd, size)) };*/


    sf::CircleShape F_verticale_D(radius);
    F_verticale_D.setFillColor(sf::Color::Red);
    F_verticale_D.setOrigin(F_verticale_D.getRadius() / 2, F_verticale_D.getRadius() / 2);
    F_verticale_D.setPosition(busb + radius / 2, busb + radius / 2);
    sf::CircleShape F_horizontale_D(radius);
    F_horizontale_D.setFillColor(sf::Color::Green);
    F_horizontale_D.setOrigin(F_horizontale_D.getRadius() / 2, F_horizontale_D.getRadius() / 2);
    F_horizontale_D.setPosition(busb + radius / 2, busa - radius);


    sf::CircleShape F_verticale_G(radius);
    F_verticale_G.setFillColor(sf::Color::Red);
    F_verticale_G.setOrigin(F_verticale_G.getRadius() / 2, F_verticale_G.getRadius() / 2);
    F_verticale_G.setPosition(busd + radius / 2, busd - radius);
    sf::CircleShape F_horizontal_G(radius);
    F_horizontal_G.setFillColor(sf::Color::Green);
    F_horizontal_G.setOrigin(F_horizontal_G.getRadius() / 2, F_horizontal_G.getRadius() / 2);
    F_horizontal_G.setPosition(busd + radius / 2, busc - radius);

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

       // window.draw(trafficlightspirite);

        window.draw(line1, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
        window.draw(line3, 2, sf::Lines);
        window.draw(line4, 2, sf::Lines);
       /* window.draw(line5, 2, sf::Lines);
        window.draw(line6, 2, sf::Lines);
        window.draw(line7, 2, sf::Lines);
        window.draw(line8, 2, sf::Lines);*/
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