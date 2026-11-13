#pragma once

/*
*/

#include <SFML/Graphics.hpp>
#include <format>
#include <print>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <list>

inline int rand_int(int min, int max)
{
    static std::default_random_engine ran{ unsigned(time(0)) };
    return std::uniform_int_distribution<>{min, max}(ran);
}
sf::Font font{ "OpenSans.ttf" };
//--------------------------------------------------------------------------------------------
class Snake : public sf::Drawable
{
public:
    Snake();

    auto getPosition() const {  return m_body.front().getPosition();  }
    const auto& getRadius() const { return m_radius; }

    void feed(unsigned short number_of_apples);

    void speed_up() { m_speed *= 3; };
    void slow_down() { m_speed /= 3; };
    [[nodiscard]] inline bool move_on_angle(float seconds_elapsed, sf::Angle angle);

private:
    float m_radius{ 16 };
    sf::Vector2f m_origin{ m_radius /2.f,m_radius /2.f};
    float m_speed{ 100 };
    std::list<sf::CircleShape> m_body;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        for(const auto& part:m_body)
            target.draw(part, states);
    }
};
class Apple : public sf::Drawable
{
public:
    Apple(const std::string& texture_name);

    auto getPosition() const {  return m_sprite.getPosition();  }
    const auto& getRadius() const {  return m_radius;  }

    void set_random_position();
private:
    float m_radius;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(m_sprite, states);
    }
};
class Move_Handler
{
public:
    Move_Handler(Snake& snake);
    inline void operator()(sf::Event::MouseButtonPressed&);
    inline void operator()(sf::Event::MouseButtonReleased&);
    [[nodiscard]] inline bool operator()(sf::RenderWindow& window);
private:
    sf::Clock m_clock;
    Snake& m_snake;
};
//--------------------------------------------------------------------------------------------
class Timer : public sf::Drawable
{
public:
    Timer();
    void start() { clock.start(); }
    void set_text_formatted(const std::string& message);
    void stop() { clock.stop(); }
private:
    sf::Clock clock;
    sf::Text text;
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(text, states);
    }
};
//--------------------------------------------------------------------------------------------
class Window :public sf::RenderWindow
{
public:
    Window();
    void show();
private:
    Snake m_snake;
    Apple m_apple;
    Move_Handler m_move_handler;
    Timer m_timer;
    sf::Text m_apples_count;
};
//--------------------------------------------------------------------------------------------
