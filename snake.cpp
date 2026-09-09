#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main()
{
    sf::RenderWindow mainWindow(sf::VideoMode({800,600}), "Snake");

    mainWindow.setFramerateLimit(60);

    enum Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        STILL
    };

    Direction direction = STILL;

    sf::Clock clock;

    sf::VertexArray linesY(sf::PrimitiveType::Lines, 32);

    int lineCount = 0;

    for (int i = 0; i < 800; i += 50)
    {
        linesY[lineCount].position = sf::Vector2f(0.f, i);
        linesY[lineCount].color = sf::Color({30, 60, 30});

        linesY[++lineCount].position = sf::Vector2f(800.f, i);
        linesY[lineCount].color = sf::Color({30, 60, 30});

        lineCount++;
    }

    sf::VertexArray linesX(sf::PrimitiveType::Lines, 32);
    lineCount = 0;

    for (int i = 0; i < 800; i += 50)
    {
        linesX[lineCount].position = sf::Vector2f(i, 0.f);
        linesX[lineCount].color = sf::Color({30, 60, 30});
        linesX[++lineCount].position = sf::Vector2f(i, 600.f);
        linesX[lineCount].color = sf::Color({30, 60, 30});
        lineCount++;
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 15);
    uniform_int_distribution<int> distY(0, 11);

    //SNAKE

    vector<sf::RectangleShape> snake;

    sf::RectangleShape snakeBody({50.f, 50.f});

    snakeBody.setFillColor(sf::Color::Green);

    sf::Texture snakeTexture;

    if (!snakeTexture.loadFromFile("snakeSkin.jpg"))
    {
        cout << "Can't load a file";
    }

    snakeBody.setTexture(&snakeTexture);
    snake.push_back(snakeBody);

    sf::CircleShape apple(20);

    sf::Texture appleTexture;

    if (!appleTexture.loadFromFile("appleSkin.png"))
    {
        cout << "Can't load a file";
    }

    apple.setTexture(&appleTexture);
    apple.setPosition({150.f, 150.f});

    bool gameOver = false;

    int score = 0;

    sf::Font font;

    if (!font.openFromFile("ARIAL.TTF"))
    {
        cout << "Cant load font!";
    }

    sf::Text gameOverText(font, "GAME OVER", 60);
    gameOverText.setPosition({250.f, 200.f});

    sf::Text restartText(font, "RESTART", 30);
    restartText.setPosition({270.f, 300.f});

    sf::Text scoreText(font, "SCORE 0", 30);
    scoreText.setPosition({10.f, 10.f});

    while (mainWindow.isOpen())
    {
        while (const std::optional event = mainWindow.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                mainWindow.close();
            }

            if (gameOver && event->is<sf::Event::KeyPressed>())
            {
                if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R)
                {
                    snake.clear();

                    snakeBody.setPosition({0.f, 0.f});
                    snake.push_back(snakeBody);

                    gameOver = false;
                    direction = STILL;
                    clock.restart();

                    score = 0;
                    scoreText.setString("SCORE " + to_string(score));
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            if (direction != DOWN)
            {
                direction = UP;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            if (direction != UP)
            {
                direction = DOWN;
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            if (direction != RIGHT)
            {
                direction = LEFT;
            }
            
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            if (direction != LEFT)
            {
                direction = RIGHT;   
            }
        }

        if (!gameOver && clock.getElapsedTime().asSeconds() >= 0.2)
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

            for (int i = 1; i < snake.size(); i++)
            {
                auto collision = snake[0].getGlobalBounds().findIntersection(snake[i].getGlobalBounds());

                if (collision)
                {
                    gameOver = true;
                }
            }

            clock.restart();
        }

        sf::Vector2f boundaries = {750.f, 550.f};

        if (snake[0].getPosition().x < 0)
        {
            gameOver = true;
        }

        if (snake[0].getPosition().x > boundaries.x)
        {
            gameOver = true;
        }

        if (snake[0].getPosition().y < 0)
        {
            gameOver = true;
        }

        if (snake[0].getPosition().y > boundaries.y)
        {
            gameOver = true;
        }

        auto intersection = snake[0].getGlobalBounds().findIntersection(apple.getGlobalBounds());

        if (intersection)
        {
            bool appleOnSnake = true;

            while (appleOnSnake)
            {
                appleOnSnake = false;

                apple.setPosition({
                    static_cast<float>(dist(gen) * 50),
                    static_cast<float>(distY(gen) * 50)
                });

                for (int i = 0; i < snake.size(); i++)
                {
                    if (snake[i].getGlobalBounds().findIntersection(apple.getGlobalBounds()))
                    {
                        appleOnSnake = true;
                    }
                }   
            }

            score++;

            scoreText.setString("SCORE " + to_string(score));

            sf::RectangleShape newSegment({50.f, 50.f});
            sf::Vector2f lastPosition = snake.back().getPosition();

            newSegment.setTexture(&snakeTexture);

            if (direction == RIGHT)
            {
                newSegment.setPosition({
                    lastPosition.x - 50.f,
                    lastPosition.y
                });
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
        mainWindow.draw(scoreText);

        if (gameOver)
        {
            mainWindow.draw(gameOverText);
            mainWindow.draw(restartText);
        }

        mainWindow.display();
    }
}