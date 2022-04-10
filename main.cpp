#include<iostream>
#include<SFML/Graphics.hpp>
#include "ShapeWithTex.h"
#include<vector>
#include<fstream>
#include<sstream>
#include<string>
#include "CreateSound.h"

sf::RenderWindow window(sf::VideoMode(1200, 1000), "Flappy Bird", sf::Style::Close);


float deltaTime = 0.f;
const float GroundHeight = 920.f;
const std::string FontPath = "Fonts/Font1.ttf";

sf::Font font;

int CurrentPipe = 0;

namespace map
{
    const std::string BackgroundPath = "Textures/Background.png";
    const std::string PipePath = "Textures/Pipe.png";
    const std::string PipeTopPath = "Textures/Pipe.png";
    const float MoveValue = -2.f;

    const float PipeWidth = 80.f;
    const float DistanceBetweenPipes = 300.f;

    const float PipeSpawnTimerMax = 2.f;
    float PipeSpawnTimer = PipeSpawnTimerMax;
    int MaxPipes = 5;

    ShapeWithTex Background[] = { ShapeWithTex(BackgroundPath, {float(window.getSize().x), float(window.getSize().y)}),
                                  ShapeWithTex(BackgroundPath, {float(window.getSize().x), float(window.getSize().y)}, float(window.getSize().x), 0.f) } ;


    sf::Texture PipeTex;
    std::vector<sf::RectangleShape*> Pipes;
    ShapeWithTex PipeTop(PipeTopPath, { 100.f, 30.f });

    void init()
    {
        PipeTex.loadFromFile(PipePath);
        font.loadFromFile(FontPath);
    }

    void MoveMap()
    {
        Background[0].Shape.move(MoveValue, 0.f);
        Background[1].Shape.move(MoveValue, 0.f);


        for (size_t i = 0; i < 2; i++)
            if (Background[i].Shape.getPosition().x + Background[i].Shape.getGlobalBounds().width <= 0)
                Background[i].Shape.setPosition(float(window.getSize().x), 0);
    }

    void update()
    {


        
        if (PipeSpawnTimer >= PipeSpawnTimerMax)
        {
            if (Pipes.size() / 2 < MaxPipes)
            {
                float PipeSize[2] = { 0,0 };
                int r = rand() % 2;

                PipeSize[r] = float(rand() % int(window.getSize().y - DistanceBetweenPipes)) + 1;
                if (r == 1)
                    PipeSize[0] = window.getSize().y - PipeSize[1] - DistanceBetweenPipes;
                else
                    PipeSize[1] = window.getSize().y - PipeSize[0] - DistanceBetweenPipes;


                Pipes.push_back(new sf::RectangleShape({ PipeWidth, PipeSize[0]}));
                Pipes[Pipes.size() - 1]->setPosition(window.getSize().x + PipeWidth, 0.f);
                Pipes[Pipes.size() - 1]->setTexture(&PipeTex);

                Pipes.push_back(new sf::RectangleShape({ PipeWidth, PipeSize[1]}));
                Pipes[Pipes.size() - 1]->setPosition(window.getSize().x + PipeWidth,
                    (window.getSize().y -  Pipes[Pipes.size() - 1]->getGlobalBounds().height) - (window.getSize().y - GroundHeight));
                Pipes[Pipes.size() - 1]->setTexture(&PipeTex);

            }
            PipeSpawnTimer = 0.f;
        }

        else
            PipeSpawnTimer += deltaTime;

   
        if(!Pipes.empty())
        {
            for (size_t i = 0; i < Pipes.size(); i++)
            {
               
                Pipes[i]->move(MoveValue, 0.f);

                if (Pipes[i]->getPosition().x + PipeWidth < 0.f)
                {
                    delete Pipes[0];
                    Pipes.erase(Pipes.begin());
                    Pipes.erase(Pipes.begin());
                    CurrentPipe--;
                    i -= 2;
                }
            }
        }
    }

    void draw()
    {
        window.draw(Background[0].Shape);
        window.draw(Background[1].Shape);

        if (!Pipes.empty())
            for (size_t i = 0; i < Pipes.size(); i++)
            {
                window.draw(*Pipes[i]);
                if (i % 2 == 0)
                    PipeTop.Shape.setPosition(Pipes[i]->getPosition().x - 10.f, Pipes[i]->getPosition().y + Pipes[i]->getGlobalBounds().height - PipeTop.Shape.getGlobalBounds().height);
                else
                    PipeTop.Shape.setPosition(Pipes[i]->getPosition().x - 10.f, Pipes[i]->getPosition().y);

                window.draw(PipeTop.Shape);
            }
    }

}

namespace Bird
{
    enum BirdStatusE{ FLY = 0, FALL = 1};

    int BirdStatus = FALL;

    bool Dead = false;
    bool Start = true;
    bool canSpaceBePressed = true;
    bool FirstTime = true;
    bool canHitSoundPlay = true;

    int Score =  -1;
    int HighScore;
    sf::Text ScoreText;
    sf::Text HighScoreText;
    sf::Text StartText;

    const std::string BirdFallingPath = "Textures/BirdFalling.png";
    const std::string BirdFlyingPath = "Textures/BirdFlying.png";
    const std::string BirdDeadPath = "Textures/BirdDead.png";
    const std::string SaveFilePath = "save files/Highscore.txt";
    const sf::Vector2f BirdSize = { 55, 40 };
    const std::string BirdFlySoundPath = "Sounds/BirdFlySound.wav";
    const std::string BirdLoseHitPath = "Sounds/BirdLoseHit.wav";

    CreateSound BirdFlySound(BirdFlySoundPath);
    CreateSound BirdLoseHitSound(BirdLoseHitPath);

    ShapeWithTex BirdShape(BirdFallingPath, BirdSize, 200.f, window.getSize().y / 2.f);

    float YBirdVelocity = 0.f;
    float Gravity = 0.1f;
    float FlyValue = -5.f;

    
    void init()
    {
        BirdFlySound.sound.setVolume(20);
        BirdLoseHitSound.sound.setVolume(20);

        ScoreText.setFont(font);
        ScoreText.setString("0");
        ScoreText.setOutlineColor(sf::Color::Black);
        ScoreText.setOutlineThickness(2.f);
        ScoreText.setCharacterSize(60);
        ScoreText.setPosition(float(window.getSize().x / 2), 50.f);

        StartText.setFont(font);
        StartText.setCharacterSize(60);
        StartText.setString("Flappy Bird made by sheraah.-#1064");
        StartText.setFillColor(sf::Color::Red);
        StartText.setOutlineColor(sf::Color::Black);
        StartText.setOutlineThickness(5.f);
        StartText.setPosition(window.getSize().x / 2 - StartText.getGlobalBounds().width / 2, 400);

        std::ifstream in(SaveFilePath);

        if (in.is_open())
        {
            std::string StrHighScore;

            std::getline(in, StrHighScore);
            std::stringstream ss;
            ss << StrHighScore;
            ss >> HighScore;
        }

        in.close();

        HighScoreText.setFont(font);
        HighScoreText.setFillColor(sf::Color::Color(255, 75, 75));
        HighScoreText.setOutlineColor(sf::Color::Black);
        HighScoreText.setOutlineThickness(1.f);
        HighScoreText.setString("HIGHSCORE: " + std::to_string(HighScore));
        HighScoreText.setPosition(float(window.getSize().x / 2 - HighScoreText.getGlobalBounds().width / 2), 10);
    }

    void update()
    {
        if (!Start)
        {
            YBirdVelocity += Gravity;

            if (!Dead && !Start)
            {
                if (BirdShape.Shape.getPosition().y + BirdSize.y > GroundHeight)
                {
                    BirdLoseHitSound.sound.play();
                    BirdShape.ShapeTex.loadFromFile(BirdDeadPath);
                    Dead = true;
                    return;
                }

                if (BirdShape.Shape.getPosition().y < 0.f)
                {
                    BirdShape.Shape.setPosition(BirdShape.Shape.getPosition().x, 0.f);
                    YBirdVelocity = 0.f;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canSpaceBePressed)
                {
                   
                    BirdFlySound.sound.play();

                    canSpaceBePressed = false;
                    YBirdVelocity = FlyValue;
                    BirdShape.ShapeTex.loadFromFile(BirdFlyingPath);
                    BirdStatus = FLY;
                }

                if (YBirdVelocity > 0.f && BirdStatus != FALL)
                {
                    BirdShape.ShapeTex.loadFromFile(BirdFallingPath);
                    BirdStatus = FALL;
                }

                BirdShape.Shape.move(0.f, YBirdVelocity);

                for (size_t i = 0; i < map::Pipes.size(); i++)
                {
                    if (BirdShape.Shape.getGlobalBounds().intersects(map::Pipes[i]->getGlobalBounds()))
                    {
                        BirdLoseHitSound.sound.play();
                        Dead = true;
                    }
                }

                if (!map::Pipes.empty())
                {
                    if (map::Pipes[CurrentPipe]->getPosition().x + map::Pipes[CurrentPipe]->getGlobalBounds().width / 2 < BirdShape.Shape.getPosition().x + BirdSize.x)
                    {
                        Score++;

                        ScoreText.setString(std::to_string(Score));
                        CurrentPipe++;;
                    }
                }
            }
            else
            {
                if (BirdShape.Shape.getPosition().y + BirdSize.y < GroundHeight)
                {
                    BirdShape.Shape.move(0.f, YBirdVelocity);

                    if (canHitSoundPlay)
                    {
                        BirdLoseHitSound.sound.play();
                        canHitSoundPlay = false;
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                {
                    if (FirstTime && HighScore == 0 && Score > 0)
                    {
                        HighScore = Score;
                        FirstTime = false;

                        std::ofstream out(SaveFilePath);
                        out << HighScore;
                        out.close();
                        HighScoreText.setString("HIGHSCORE: " + std::to_string(HighScore));
                    }
                    else
                    {
                        if (Score > HighScore)
                        {
                            HighScore = Score;
                            std::ofstream out(SaveFilePath);
                            out << HighScore;
                            out.close();
                            HighScoreText.setString("HIGHSCORE: " + std::to_string(HighScore));
                        }
                    }
                    
                    Start = true;
                    Dead = false;
                    BirdShape.Shape.setPosition(200.f, window.getSize().y / 2.f);
                    
                    for (auto& P : map::Pipes)
                        delete P;
                    map::Pipes.clear();
                    YBirdVelocity = 0.f;
                    Score = -1;
                    CurrentPipe = 0;
                    ScoreText.setString("0");
                }

            }
        }
        else
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                Start = false;
        }

    }

    void draw()
    {
        window.draw(BirdShape.Shape);
        window.draw(ScoreText);
        window.draw(HighScoreText);

        if (Start)
            window.draw(StartText);
    }
}




int main()
{
    srand(static_cast<unsigned>(time(0)));

    window.setFramerateLimit(144);

    sf::Clock clock;

    map::init();
    Bird::init();

    while (window.isOpen())
    {

        deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();
        sf::Event evnt;
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::KeyReleased:
                if (evnt.key.code == sf::Keyboard::Space)
                    Bird::canSpaceBePressed = true;
                break;
            }
        }

        //update
        Bird::update();

        if(!Bird::Dead)
        map::MoveMap();

        if (!Bird::Dead && !Bird::Start)
        {
            map::update();
        }

        
        //draw

        window.clear();

        map::draw();
        Bird::draw();


        window.display();
    }

    return 0;

}
