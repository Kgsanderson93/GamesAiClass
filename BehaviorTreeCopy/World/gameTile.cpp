#include "gameTile.h"
gameTile::gameTile(std::string texturename, float x, float y, bool passable, bool exit, int Row, int Col, int idnum){
    pos= sf::Vector2f(x, y);
    sprite.setPosition(pos);
    ispassable=passable;
    isExit=exit;
    if(!setUpSprite(texturename)){
        return;
    }
    idRow=Row;
    idCol=Col;
    id=idnum;
    name=std::to_string(idnum);
    coords=pos;
    hasfruit=false;

}
bool gameTile::setUpSprite(std::string textureName){
    if(!texture.loadFromFile(textureName)){
        return false;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
    return true;
}