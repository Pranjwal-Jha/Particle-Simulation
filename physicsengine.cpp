#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <math.h>
#include "random.h"


namespace constants{
    const float g{9.8};
    const float pmp{20.f};
}
struct Particle{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    Particle(sf::Vector2f position, sf::Vector2f position_last, sf::Vector2f acceleration,sf::Vector2f velocity)
        :position(position),position_last(position_last),acceleration(acceleration),velocity(velocity)
        {
        }
    Particle()
        : position(0.f,0.f),position_last(0.f,0.f),acceleration(0.f,0.f),velocity(0.f,0.f)
        {
        }


    void apply_gravity(float dt){
        acceleration.y +=constants::g*constants::pmp*dt;
    }
    // can make a velocity function
    void update(float dt){
        sf::Vector2f new_pos = 2.f*position - position_last +  acceleration*(dt*dt);
        //sf::Vector2f cur_velocity = getvelocity();
        //sf::Vector2f new_pos = position.x*cur_velocity.x+  acceleration*(dt*dt);
        position_last = position; // sfml methods only work with float
        position = new_pos;         // using .f makes 2 -> 2.0
        velocity = getvelocity();
        // reseting acc for next frame
        //acceleration = sf::Vector2f(0,0);
        //return position
    }

    void setVelocity(sf::Vector2f v,float dt){
        position_last = position - (v*dt);
    }

    sf::Vector2f getvelocity(){
        return position - position_last;
    }
    void apply_boundary(Particle& boundary){
        const sf::Vector2f r = position - boundary.position;
        const float dist = sqrt(r.x*r.x + r.y*r.y);
        if(dist > 180.f){
            //acceleration.y = -acceleration.y;
            //acceleration.x = -acceleration.x;
            sf::Vector2f unit_vector = r/dist;
            position = boundary.position + unit_vector*180.f;
        }
    }
    void draw(sf::RenderWindow& window, sf::CircleShape& circle){
        circle.setPosition(position);
        window.draw(circle);
    }
};
int main(){
    // do not edit here
    sf::RenderWindow window(sf::VideoMode(800,800),"Particle System");
    //window setting
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    //font for frame rate counter
    sf::Font font;
    font.loadFromFile("JetBrainsMono-VariableFont_wght.ttf");

    //basic balls
    std::vector<Particle> particles;
    sf::CircleShape circle(18.f);
    circle.setFillColor(sf::Color(178, 237, 197));
    circle.setOutlineThickness(2.f);
    circle.setOutlineColor(sf::Color(123, 178, 217));

    //boundary definition
    sf::CircleShape boundary(200.f);
    boundary.setOrigin(boundary.getRadius()-20.f, boundary.getRadius()-20.f);  // Center the origin
    boundary.setPosition((window.getSize().x / 2.f), (window.getSize().y / 2.f));
    Particle p_boundary(
        //sf::Vector2f((window.getSize().x / 2.f) - boundary.getRadius(),
        //(window.getSize().y / 2.f) - boundary.getRadius()),
        //sf::Vector2f(400.f,200.f),
        boundary.getPosition(),
        boundary.getPosition(),
        sf::Vector2f(0.f,0.f),
        sf::Vector2f(0.f,0.f)
    );
    boundary.setFillColor(sf::Color::White);


    auto frame_clock = sf::Clock{};
    float lasttime=0;

    // can edit from here
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
                    particles.emplace_back(sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),
                    sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),
                    sf::Vector2f(0.f,0.f),sf::Vector2f(0.f,0.f));
            }
        }
        //rendering balls
        for(auto& particle:particles){
            particle.apply_gravity(dt);
            particle.update(dt);
            particle.apply_boundary(p_boundary);
            particle.draw(window,circle);
        }

        //frame rate counter
        float fps = 1.f/dt-lasttime;
        sf::Text FPS(std::to_string(fps),font,20);
        FPS.setFillColor(sf::Color::Red);
        //const sf::Vector2f bound = p_boundary.position;
        //sf::Text boundary(std::to_string(bound),font,20);
        window.draw(FPS);
        lasttime=dt;
        window.display();
    }
    return 0;
}
//g++ physicsengine.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
