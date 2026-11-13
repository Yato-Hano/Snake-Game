#include <snake.h>

template<typename A, typename B>
bool are_collide(const A& a, const B& b)
{
    return (a.getPosition() - b.getPosition()).length() <= a.getRadius() + b.getRadius();
}
//--------------------------------------------
Move_Handler::Move_Handler(Snake& snake)
    :m_snake{ snake }
{
    m_clock.start();
}
void Move_Handler::operator()(sf::Event::MouseButtonPressed&)
{
    m_snake.speed_up();
}
void Move_Handler::operator()(sf::Event::MouseButtonReleased&)
{
    m_snake.slow_down();
}
bool Move_Handler::operator()(sf::RenderWindow& window)
{
    sf::Vector2f from = m_snake.getPosition();
    sf::Vector2f to = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

    sf::Angle angle = (to - from).angle();
    float seconds = m_clock.restart().asSeconds();

    return m_snake.move_on_angle(seconds, angle);
}
//--------------------------------------------
Apple::Apple(const std::string& texture_name)
    :m_radius{ 11 },
    m_texture{ texture_name },
    m_sprite{ m_texture }
{
    m_sprite.setPosition({ 50,50 });
    m_sprite.setOrigin({ 30,30 });
}
void Apple::set_random_position()
{
    m_sprite.setPosition({ static_cast<float>(rand_int(0,800)), static_cast<float>(rand_int(0,800)) });
}
//--------------------------------------------
Snake::Snake()
{
    m_body.emplace_back(m_radius);
    m_body.back().setFillColor(sf::Color::White);
    m_body.back().setOrigin(m_origin);
    m_body.back().setPosition({ 100, 100 });
}
void Snake::feed(unsigned short number_of_apples)
{
    for (unsigned short i = 0; i < number_of_apples; i++)
    {
        auto pos = m_body.back().getPosition();
        m_body.emplace_back(m_radius);
        m_body.back().setFillColor(sf::Color::White);
        m_body.back().setOrigin(m_origin);
        m_body.back().setPosition(pos);
    }
}
bool Snake::move_on_angle(float seconds_elapsed, sf::Angle angle)
{
    //move------------
    if (5.f <= (m_body.front().getPosition() - std::next(m_body.begin())->getPosition()).length())
    {
        m_body.back().setPosition(m_body.front().getPosition());
        m_body.splice(m_body.begin(), m_body, std::prev(m_body.end()));
    }
    m_body.front().move({ seconds_elapsed * m_speed,angle });

    //check collision------------
    if (m_body.size() < 15)return true;
    auto front = m_body.front();
    for (auto part = std::next(m_body.begin(),15); part != m_body.end(); ++part)
    {
        if (are_collide(front, *part))
        {
            for (auto& circle : m_body)
                circle.setFillColor(sf::Color::Red);

            return false;
        }
    }
    return true;
}
//--------------------------------------------
Timer::Timer() :
    text{ font,"timer-here" }
{
    text.setPosition({ 0, 0 });
    text.setFillColor(sf::Color::Yellow);
}
void Timer::set_text_formatted(const std::string& message)
{
    text.setString(std::format("{:.3}s {}", clock.getElapsedTime().asSeconds(), message));
}
//--------------------------------------------
Window::Window()
    :sf::RenderWindow{ sf::VideoMode{ { 800, 800 } }, "Snake Noodles",sf::Style::Close ,sf::State::Windowed,sf::ContextSettings{.antiAliasingLevel = 8 } },
    m_move_handler{m_snake},
    m_apple{"sprite_0.png"},
    m_apples_count{ font,std::format("0 apples") }
{
    m_apples_count.setPosition({ 300, 0 });
    m_apples_count.setFillColor(sf::Color::Red);
}
void Window::show()
{
    short apples_count{ 0 };
    bool not_over{ true };
    m_timer.start();
    while (isOpen())
    {
        handleEvents([this](const sf::Event::Closed) { close(); }, m_move_handler);
        if( not_over){
            not_over = m_move_handler(*this);
            m_timer.set_text_formatted("");
            if (are_collide(m_snake, m_apple))
            {
                ++apples_count;
                m_apples_count.setString(std::format("{} apples", apples_count) );
                m_snake.feed(5);
                m_apple.set_random_position();
            }
        }
        clear(sf::Color{ 18, 20, 32 });
        draw(m_apples_count);
        draw(m_timer);
        draw(m_apple);
        draw(m_snake);
        display();
    }
}
//--------------------------------------------

int main()
try
{
   Window w;
   w.show();
}
catch(const std::exception& e)
{
    std::ofstream ofs{"error_log.txt",std::ios::app};
    ofs << "Error: " << e.what() <<'.' << std::endl;
    std::println("Error: {}.", e.what());
    std::cin.get();
}
catch(...)
{
    std::ofstream ofs{ "error_log.txt",std::ios::app };
    ofs << "Some error." << std::endl;
    std::println("Some error.");
    std::cin.get();
}

