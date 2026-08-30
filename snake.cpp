#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
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

    // Clock

    sf::Clock clock;
        
    // Lines

    sf::VertexArray linesY(sf::PrimitiveType::Lines, 32);

    int lineCount = 0;

    for (int i = 0; i < 800; i += 50)
    {
        linesY[lineCount].position = sf::Vector2f(0.f, i);
        linesY[++lineCount].position = sf::Vector2f(800.f,i);
        lineCount++;
    }
    
    sf::VertexArray linesX(sf::PrimitiveType::Lines, 32);

    lineCount = 0;

    for (int i = 0; i < 800; i += 50)
    {
        linesX[lineCount].position = sf::Vector2f(i, 0.f);
        linesX[++lineCount].position = sf::Vector2f(i, 600.f);
        lineCount++;
    }
    

    //RANDOM

    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> dist(0, 15);
    uniform_int_distribution<int> distY(0, 11);

    //Snake

    vector<sf::RectangleShape> snake;
    
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

    snake.push_back(snakeBody);

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

        if (clock.getElapsedTime().asSeconds() >= 0.2)
        {
            for (int i = snake.size() - 1; i > 0; i--)
            {
                snake[i].setPosition(snake[i - 1].getPosition());
            }
            
            if (direction == UP)
            {
                snake[0].move({0.f, -50.f});
            }
            if (direction == DOWN)
            {
                snake[0].move({0.f, 50.f});
            }
            if (direction == LEFT)
            {
                snake[0].move({-50.f, 0.f});
            }
            if (direction == RIGHT)
            {
                snake[0].move({50.f, 0.f});
            }
            clock.restart();
        }

        //Bonduary

        sf::Vector2f boundaries = {750.f, 550.f};

        if (snake[0].getPosition().x < 0)
        {
            snake[0].setPosition({
                0.f,
                snake[0].getPosition().y
            });
        }
        if (snake[0].getPosition().x > boundaries.x)
        {
            snake[0].setPosition({
                boundaries.x,
                snake[0].getPosition().y
            });
        }
        if (snake[0].getPosition().y < 0)
        {
            snake[0].setPosition({
                snake[0].getPosition().x,
                0.f
            });
        }
        if (snake[0].getPosition().y > boundaries.y)
        {
            snake[0].setPosition({
                snake[0].getPosition().x,
                boundaries.y
            });
        }
        
        //Collision
        
        auto intersection = snake[0].getGlobalBounds().findIntersection(apple.getGlobalBounds());
        if (intersection)
        {
            apple.setPosition({
                static_cast<float>(dist(gen) * 50),
                static_cast<float>(distY(gen) * 50)
            });
            
            sf::RectangleShape newSegment({50.f, 50.f});
            sf::Vector2f lastPosition = snake.back().getPosition();
            newSegment.setTexture(&snakeTexture);

            if (direction == RIGHT)
            {
                newSegment.setPosition({
                    lastPosition.x - 50.f,
                    lastPosition.y
                });

                cout << "Head: "<< lastPosition.x << "Y: " << lastPosition.y << endl;
            }
            
            if (direction == LEFT)
            {
                newSegment.setPosition({
                    lastPosition.x + 50.f,
                    lastPosition.y
                });
            }

            if (direction == UP)
            {
                newSegment.setPosition({
                    lastPosition.x,
                    lastPosition.y - 50.f
                });
            }

            if (direction == DOWN)
            {
                newSegment.setPosition({
                    lastPosition.x,
                    lastPosition.y + 50.f
                });
            }
            
            

            snake.push_back(newSegment);
            
        }
        
        mainWindow.clear(sf::Color::Black);

        mainWindow.draw(linesY);
        mainWindow.draw(linesX);

        for (sf::RectangleShape snakes : snake)
        {
            mainWindow.draw(snakes);
        }
        
        mainWindow.draw(apple);

        mainWindow.display();
    }
    
}