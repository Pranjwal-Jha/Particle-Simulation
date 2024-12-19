#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include "random.h"

// REWORK THE ENTIRE THE THING WITH CLASSES


namespace constants{
    const float g{9.8};
    const float pmp{800.f};
    const float spawn_interval= 1.0f;
}

namespace Colors{
    enum Color{
        RobinBlue,
        Cyclamen,
        ScreminGreen,
        Lime,
        Coral,
        Sunglow
    };

    sf::Color getColor(Color color){
        switch(color){
            case RobinBlue:
            return sf::Color(99, 204, 202);
            case Cyclamen:
            return sf::Color(254, 93, 159);
            case ScreminGreen:
            return sf::Color(119, 255, 148);
            case Lime:
            return sf::Color(206, 255, 26);
            case Coral:
            return sf::Color(255, 120, 79);
            case Sunglow:
            return sf::Color(255, 202, 58);
            default:
            return sf::Color(138, 201, 38);
        }
    }
}
static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * 3.14);
    const float b = sin(t + 0.66f * 2.0f * 3.14);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}


struct Particle{
    sf::Vector2f position;
    sf::Vector2f position_last;
    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Color color;
    Particle(sf::Vector2f position, sf::Vector2f position_last, sf::Vector2f acceleration,sf::Vector2f velocity,sf::Color color)
        :position(position),position_last(position_last),acceleration(acceleration),velocity(velocity),color(color)
        {
        }
    Particle()
        : position(0.f,0.f),position_last(0.f,0.f),acceleration(0.f,0.f),velocity(0.f,0.f),color(sf::Color::Black)
        {
        }


    void apply_gravity(float dt){
        acceleration.y +=constants::g*constants::pmp*dt;
        velocity.y+=constants::g*constants::pmp*dt;
    }
    // can make a velocity function
    void update(float dt){
        sf::Vector2f new_pos = 2.f*position - position_last +  acceleration*(dt*dt);
        //sf::Vector2f cur_velocity = getvelocity();
        //sf::Vector2f new_pos = position.x*cur_velocity.x+  acceleration*(dt*dt);
        position_last = position; // sfml methods only work with float
        position = new_pos;         // using .f makes 2 -> 2.0
        //velocity = getvelocity();
        // reseting acc for next frame
        acceleration = sf::Vector2f(0,0);
        //return position
    }

    void setVelocity(sf::Vector2f v,float dt){
        position_last = position - (v*dt);
    }

    sf::Vector2f getvelocity(){
        return position - position_last;
    }
    //void apply_boundary(Particle& boundary,sf::CircleShape& circle){
    //    //const sf::Vector2f r = position - circle.getPosition();
    //    const sf::Vector2f r = circle.getOrigin() - position;
    //    const float dist = sqrt(r.x*r.x + r.y*r.y);
    //    if(dist > 180.f){
    //        //acceleration.y = -acceleration.y;
    //        //acceleration.x = -acceleration.x;
    //        const sf::Vector2f unit_vector = r/dist;
    //        const sf::Vector2f perp = {-unit_vector.y,unit_vector.x};
    //        const sf::Vector2f vel = getvelocity();
    //        position = circle.getOrigin() - unit_vector*(circle.getRadius()-20.f);
    //        setVelocity(2.0f*(vel.x*perp.x+vel.y*perp.y)*perp-vel,1.0f);
    //        //position = boundary.position + unit_vector*180.f;

    //    }
    //}
    void apply_boundary(Particle& boundary, sf::CircleShape& circle) {
        const sf::Vector2f r = position - circle.getPosition(); // Distance from ball to circle center
        const float dist = sqrt(r.x * r.x + r.y * r.y); // Calculate distance magnitude

        if (dist > circle.getRadius() - 20.f) { // Check if ball is outside boundary
            const sf::Vector2f unit_vector = r / dist; // Unit vector pointing outwards
            const sf::Vector2f vel = getvelocity(); // Get current velocity

            // Reflect position to boundary edge
            position = circle.getPosition() + unit_vector * (circle.getRadius() - 10.f);

            // Reflect velocity
            const sf::Vector2f perp = {-unit_vector.y, unit_vector.x}; // Perpendicular vector
            setVelocity(2.0f * (vel.x * perp.x + vel.y * perp.y) * perp - vel, 1.0f);
        }
    }

    void checkCollision(std::vector<Particle>& particles){
        for(int i=0;i<particles.size();i++){
            Particle& particle1 = particles[i];
            for(int j=i+1;j<particles.size();j++){
                Particle& particle2 = particles[j];
                const sf::Vector2f axis = particle1.position - particle2.position;
                const float dist = sqrt(axis.x*axis.x+axis.y*axis.y);
                if(dist<20.0f){
                    const sf::Vector2f normal = axis/dist;
                    const float delta = 20.f - dist;
                    particle1.position +=0.5f*delta*normal;
                    particle2.position -=0.5f*delta*normal;
                    //CHATGPT SUGGESTION VELOCITY
                    sf::Vector2f relativeVelocity = particle1.getvelocity() - particle2.getvelocity();
                    float normalVelocity = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

                    if (normalVelocity < 0) { // Ensure they are moving towards each other
                        sf::Vector2f impulse = -2.f * normalVelocity * normal;
                        particle1.setVelocity(particle1.getvelocity() + 0.5f * impulse, 1.0f);
                        particle2.setVelocity(particle2.getvelocity() - 0.5f * impulse, 1.0f);
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window, sf::CircleShape& circle){
        circle.setPosition(position);
        circle.setFillColor(color);
        window.draw(circle);
    }
};

int ball_count{0};
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
    sf::CircleShape circle(10.f);
    circle.setFillColor(sf::Color(178, 237, 197));
    //circle.setOutlineThickness(2.f);
    //circle.setOutlineColor(sf::Color(123, 178, 217));

    //boundary definition
    sf::CircleShape boundary(200.f);
    boundary.setOrigin(boundary.getRadius()-10.f, boundary.getRadius()-10.f);  // Center the origin
    //boundary.setPosition(boundary.getRadius()-20.f, boundary.getRadius()-20.f);
    boundary.setPosition((window.getSize().x / 2.f), (window.getSize().y / 2.f));
    Particle p_boundary(
        //sf::Vector2f((window.getSize().x / 2.f) - boundary.getRadius(),
        //(window.getSize().y / 2.f) - boundary.getRadius()),
        //sf::Vector2f(400.f,200.f),
        boundary.getPosition(),
        boundary.getPosition(),
        sf::Vector2f(0.f,0.f),
        sf::Vector2f(0.f,0.f),
        sf::Color(sf::Color::White)
    );
    sf::Clock color_change;
    auto frame_clock = sf::Clock{};
    float lasttime=0;
    float time_spawn = 0.f;
    // can edit from here
    while(window.isOpen()){
        sf::Event event;
        window.clear(sf::Color::Black);
        p_boundary.draw(window,boundary);
        auto dt = frame_clock.restart().asSeconds();
        float t = color_change.getElapsedTime().asSeconds();
        time_spawn+=dt;
        //std::uniform_int_distribution<int> color_count{0,6};
        //int r_number{color_count(Random::mt)};
        //sf::Color r_color = Colors::getColor(static_cast<Colors::Color>(r_number));
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed){
                window.close();
            }
            else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                particles.emplace_back(sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),
                sf::Vector2f(float(mousePosition.x),float(mousePosition.y)),
                sf::Vector2f(0.f,0.f),sf::Vector2f(0.f,0.f)
                ,getRainbow(t)
                );
                ball_count++;
            }
        }
        //if (time_spawn>= constants::spawn_interval) {
        //     // Reset timer
        //     time_spawn = 0.f;

        //     // Spawn a new ball at a random position within the window
        //     std::uniform_real_distribution<float> x_dist(0, window.getSize().x);
        //     std::uniform_real_distribution<float> y_dist(0, window.getSize().y);
        //     float random_x = x_dist(Random::mt);
        //     float random_y = y_dist(Random::mt);
        //     particles.emplace_back(
        //         sf::Vector2f(boundary.getPosition().x, boundary.getPosition().y),  // Position
        //         sf::Vector2f(boundary.getPosition().x, boundary.getPosition().y),  // Initial position
        //         sf::Vector2f(0.f, 0.f),           // Velocity
        //         sf::Vector2f(0.f, 0.f),           // Acceleration
        //         getRainbow(t)                     // Color
        //     );

        //     ball_count++; // Update the ball count
        //}

        //circle.setFillColor(Colors::getColor(static_cast<Colors::Color>(r_number)));
        //rendering balls
        for(auto& particle:particles){
            particle.apply_gravity(dt);
            particle.update(dt);
            particle.apply_boundary(p_boundary,boundary);
            particle.checkCollision(particles);
            particle.draw(window,circle);
        }

        //frame rate counter
        float fps = 1.f/dt-lasttime;
        sf::Text FPS(std::to_string(fps),font,20);
        FPS.setFillColor(sf::Color::Red);

        //ball counter
        sf::Text balls(std::to_string(ball_count),font,20);
        balls.setFillColor(sf::Color::Yellow);
        balls.setPosition(0.f,20.f);
        window.draw(FPS);
        window.draw(balls);
        lasttime=dt;
        window.display();
    }
    return 0;
}
//g++ physicsengine.cpp -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
