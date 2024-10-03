#include "fruit.h"

fruit::fruit(int id, gameTile *tile, sf::String label){
    //std::cout<<("in fruit\n");
    this->id=id;
    this->location=tile;
    this->frame= sf::IntRect(0, 0, 100, 100);
    //std::cout<<("in fruit 2 \n");
    this->isSmashed=false;

    this->fruitlabel=label;
    if(sf::String("peach")==label){
        this->texture.loadFromFile("peachSprite.png");
        //std::cout<<("in fruit 3\n");

    }
    else{
        this->texture.loadFromFile("watermelonSprites.png");
    }

    this->sprite=sf::Sprite(this->texture, this->frame);
    //std::cout<<("in fruit 4\n");
    this->sprite.scale(sf::Vector2f(.5f, .5f));
    this->coords=tile->coords;
    this->sprite.setPosition(this->coords);
    //std::cout<<("in fruit 5\n");

} 
void fruit::smash(){
    //printf("is smashing!");
    this->isSmashed=true;
    this->texture.loadFromFile("splat.png");
    this->sprite.setTexture(this->texture);
}