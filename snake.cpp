#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
using namespace std;

int main()
{
    sf::RenderWindow mainWindow (sf::VideoMode({800,600}), "Snake");

    mainWindow.setFramerateLimit(60);

    enum Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STILL
    };

    Direction direction = STILL;

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> dist(0, 15);
    uniform_int_distribution<int> distY(0, 11);

    //Snake
    
    sf::RectangleShape snakeBody({50.f, 50.f});

    //Snake color

    snakeBody.setFillColor(sf::Color::Green);

    //Snake Texture

    sf::Texture snakeTexture;
    if (!snakeTexture.loadFromFile("snakeSkin.jpg"))
    {
        cout << "Can't load a file";
    }

    snakeBody.setTexture(&snakeTexture);

    //Apple

    sf::CircleShape apple(20);

    //Apple Texture

    sf::Texture appleTexture;
    if (!appleTexture.loadFromFile("appleSkin.png"))
    {
        cout << "Can't load a file";
    }
    
    apple.setTexture(&appleTexture);
    apple.setPosition({150.f, 150.f});
    

    while (mainWindow.isOpen())
    {
        while (const std::optional event = mainWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                mainWindow.close();
            }
            
        }

        //movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
           direction = UP;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            direction = DOWN;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            direction = LEFT;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            direction = RIGHT;
        }

        if (direction == UP)
        {
            snakeBody.move({0.f, -5.f});
        }
        if (direction == DOWN)
        {
            snakeBody.move({0.f, 5.f});
        }
        if (direction == LEFT)
        {
            snakeBody.move({-5.f, 0.f});
        }
        if (direction == RIGHT)
        {
            snakeBody.move({5.f, 0.f});
        }

        //Collision
        
        auto intersection = snakeBody.getGlobalBounds().findIntersection(apple.getGlobalBounds());
        if (intersection)
        {
            apple.setPosition({
                static_cast<float>(dist(gen) * 50),
                static_cast<float>(distY(gen) * 50)
            });
        }
        
        mainWindow.clear(sf::Color::Black);

        mainWindow.draw(snakeBody);
        mainWindow.draw(apple);

        mainWindow.display();
    }
    
}