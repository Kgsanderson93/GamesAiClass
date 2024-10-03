#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main()
{
    int windowx = 640;
    int windowy = 480;
    int moveRate = 5;

    sf::RenderWindow window(sf::VideoMode(windowx, windowy), "Demo Game");
    window.setFramerateLimit(60);
    sf::Texture texture;
    texture.loadFromFile("boid.png");
    sf::Sprite sprite(texture);
    sprite.setScale(.25, .25);
    const sf::Vector2f spriteSize(
    sprite.getTexture()->getSize().x * sprite.getScale().x,
    sprite.getTexture()->getSize().y * sprite.getScale().y);
    sf::Clock timer;
    //sprite.setOrigin(10+spriteSize.x,-(spriteSize.y/2+1));
    // run the program as long as the window is open
    while (window.isOpen())
    {
        sf::Vector2f pos=sprite.getPosition();
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if(timer.getElapsedTime().asMilliseconds() >= 100){
            if(pos.x + moveRate+spriteSize.x >= windowx){
                sprite.setPosition(0,0);
            }else{
            sprite.move(5,0);
            }
            timer.restart();
        }
        window.clear(sf::Color(0, 95, 0));
        window.draw(sprite);
        window.display();
    }

    return 0;
}