#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


namespace constants{
    const float g{9.8};
    const float pmp{500.f};
}
struct Particle{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;

    Particle(sf::Vector2f position, sf::Vector2f position_last, sf::Vector2f acceleration)
        :position(position),position_last(position_last),acceleration(acceleration)
        {
        }
    Particle()
        : position(0.f,0.f),position_last(0.f,0.f),acceleration(0.f,0.f)
        {
        }


    void apply_gravity(float dt){
        acceleration.y +=constants::g*constants::pmp*dt;
    }

    void update(float dt){
        sf::Vector2f new_pos = 2.f*position - position_last +  acceleration*(dt*dt);
        position_last = position; // sfml methods only work with float
        position = new_pos;         // using .f makes 2 -> 2.0
        // reseting acc for next frame
        acceleration = sf::Vector2f(0,0);
        //return position
    }

    void draw(sf::RenderWindow& window, sf::CircleShape& circle){
        circle.setPosition(position);
        window.draw(circle);
    }
};
int main(){
    sf::RenderWindow window(sf::VideoMode(800,800),"Particle System");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    sf::Font font;
    font.loadFromFile("JetBrainsMono-VariableFont_wght.ttf");
    std::vector<Particle> particles;
    sf::CircleShape circle(20.f);
    circle.setFillColor(sf::Color::Green);
    sf::CircleShape boundary(200.f);
    Particle p_boundary(sf::Vector2f((window.getSize().x / 2.f) - boundary.getRadius(), (window.getSize().y / 2.f) - boundary.getRadius()),sf::Vector2f(400.f,200.f),sf::Vector2f(0.f,0.f));
    boundary.setFillColor(sf::Color::White);
    auto frame_clock = sf::Clock{};
    float lasttime=0;
    while(window.isOpen()){
        sf::Event event;
        window.clear(sf::Color::Black);
        p_boundary.draw(window,boundary);
        auto dt = frame_clock.restart().asSeconds();
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                particles.emplace_back(sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),sf::Vector2f(0.f,0.f));
            }
        }
        for(auto& particle:particles){
            particle.apply_gravity(dt);
            particle.update(dt);
            particle.draw(window,circle);
        }
        float fps = 1.f/dt-lasttime;
        sf::Text FPS(std::to_string(fps),font,20);
        //FPS.setFont(font);
        //FPS.setString(std::to_string(fps));
        FPS.setFillColor(sf::Color::Red);
        window.draw(FPS);
        lasttime=dt;
        window.display();
    }
    return 0;
}
//g++ physicsengine.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
