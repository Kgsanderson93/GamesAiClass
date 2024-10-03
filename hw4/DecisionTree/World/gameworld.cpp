#include "gameWorld.h"

GameWorld::GameWorld(){
    gridx=32;
    fruitIdinit=0;
    setUpGameWorld();
    
}

void GameWorld::addFruit(sf::String label){
    gameTile *t=randomTile();
    t=getTile(sf::Vector2f(t->coords.x+10, t->coords.y+10));
    
    printf(" tile id for fruit: %d \n",t->getID());
    this->fruitList.emplace_back(new fruit(fruitIdinit++, t, label));
    ////printf("leaving fruit\n");
    
}
gameTile *GameWorld::randomTile(){
    sf::Vector2f loc=randomPosition( 640, 640);
    gameTile *tile=getTile(loc);
    if(!tile->ispassable){
        return randomTile();
    }else{
        return tile;
    }

}
void GameWorld::setUpGameWorld(){
    exitPos=sf::Vector2i(gridx-1, gridx-1);
    setUpTiles();
    for (std::vector<gameTile *> row : this->tiles)
    {
        for (gameTile *tile : row)
        {
            if (tile->ispassable)
            {
                this->tileList.emplace_back(tile);
            }
        }
    }
}
void GameWorld::updategoals(sf::Vector2f pos, float time){
    if(timesincedrop+time>droptime && fruitList.size()<5){
       int i=randomBinomial();
       if(i==1){
        addFruit("peach");
       }else{
        addFruit("watermelon");
       }
        timesincedrop=0.0f;
    }else{
        timesincedrop+=time;
    }
    this->goals= std::priority_queue<piRefFru, std::vector<piRefFru>, std::greater<piRefFru>>();

    for(fruit *f:this->fruitList){
        if(!f->isSmashed){
        float d=distancelength(pos, f->coords);
        goals.push(std::make_pair(d, f));
        }
    }
}
gameTile* GameWorld::getTileStrict(sf::Vector2f vec){
    int x=(int)vec.x/20;
    ////printf("%d x", x);
    int y=(int)vec.y/20;
    ////printf("%d y\n", y);
    if (x>31 || x<0 || y>31 || y<0){
        printf("out of bounds\n");
    }
    return tiles[y][x];
    
}
gameTile* GameWorld::getTileNearby(sf::Vector2f vec){
    sf::Vector2f loc=randomPosition( 90, 90);
    printf("additive:%f %f\n", loc.x, loc.y);
    int m=randomBinomial();
    if(m==0){
        m=1;
    }
    printf("multiplier: %d\n", m);
    
    loc=loc*(float)m;
    printf("multiplier with sign: %f, %f\n", loc.x, loc.y);
    loc=loc+vec;
    printf("combined loc with multiplier: %f, %f\n", loc.x, loc.y);
    if(loc.x<0){
        loc.x=0;
    }if(loc.y<0){
        loc.y=0;
    }
    if(loc.x>31*20){
        loc.x=30*20;
    }
    if(loc.y>31*20){
        loc.y=30*20;
    }

    gameTile *tile=getTile(loc);
    if(!tile->ispassable){
        return randomTile();
    }else{
        return tile;
    }
    
}
gameTile* GameWorld::getTile(sf::Vector2f vec){
    int x=(int)vec.x/20;
    ////printf("%d x", x);
    int y=(int)vec.y/20;
    ////printf("%d y\n", y);


    if (tiles[y][x]->ispassable==false){
        int i=1;
        while(i>0){
        if(y-i>0 &&tiles[y-i][x]->ispassable){
            return tiles[y-i][x];
        }
        else if(y+i<gridx && tiles[y+i][x]->ispassable){
            return tiles[y+i][x];
        }
        else if(x-i>0 && tiles[y][x-i]->ispassable){
            return tiles[y][x-i];
        }
        else if(x+i<gridx && tiles[y][x+i]->ispassable){
            return tiles[y][x+i];
        }
        else if(x+i< gridx && y+i < gridx&&tiles[y+i][x+i]->ispassable){
            return tiles[y+i][x+i];
        }
        else if(x+i< gridx && y-i>0 &&tiles[y-i][x+i]->ispassable){
            return tiles[y-i][x+i];
        }
        else if(x-i> 0 && y-i>0 && tiles[y-i][x-i]->ispassable){
            return tiles[y-i][x-i];
        }
        else if(x-i> 0 && y+i<gridx &&tiles[y+i][x-i]->ispassable){
            return tiles[y+i][x-i];
        }
        i++;

        }
    }{
        return tiles[y][x];
    }

}

void GameWorld::setUpTiles(){
        tiles.clear();
    int tile[32][32]={
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    int id=0;
    //printf("adding nodes\n");
    for(int i=0; i<32; i++){
        
        std::vector<gameTile *>row;

        for(int j =0; j<32; j++){
            
            int k=j*20;
            int l=i*20;
            if(tile[i][j]==1){
                row.emplace_back(new gameTile("wall.png", k, l, false, false, j, i, 0));
                //std::cout<<"wall name: "<<row.back()->getName()<<" id: "<<row.back()->id<<"\n";
                //tileList.emplace_back(new gameTile("wall.png", k, l, false, false, k, l));
            }
             else{
                row.emplace_back(new gameTile("floor.png", k, l, true, false, j,i, id++));
                //std::cout<<"floor name: "<<row.back()->getName()<<" id: "<<row.back()->id<<"\n";


                //tileList.emplace_back(new gameTile("floor.png", k, l,rue, false, k,l));

            }
        }
            tiles.emplace_back(row);
            row.clear();
    
    }
    //printf("Finished adding nodes\n");
     bool left=false;
    bool right=false;
    bool up=false;
    bool down=false;
    edges.clear();
    for(int i=0; i<32; i++){

        for(int j =0; j<32; j++){
            if(tiles[j][i]->ispassable){
            left=false;
            right=false;
            down=false;
            up=false;
            //i-1;
            if(i-1>0&&tiles[j][i-1]->ispassable){
                up=true;
                edges.emplace_back(new Edge("vertical", tiles[j][i], tiles[j][i-1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j][i-1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j][i-1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j][i-1]->coords);
                //std::cout<<"new Edge should have "<<tiles[j][i]->id<<"<-from id to id-> "<<tiles[j][i-1]->id<<" but has ids:" <<map[tiles[j][i]->id].back()->idFrom<<" "<<map[tiles[j][i]->id].back()->idTo<<"\n";

            }
            //i+1;
            if(i+1<32 &&tiles[j][i+1]->ispassable){
                down=true;
                edges.emplace_back(new Edge("vertical", tiles[j][i], tiles[j][i+1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j][i+1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j][i+1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j][i+1]->coords);
                
            }

            //j-1;
            if(j-1>0&& tiles[j-1][i]->ispassable){
                right=true;
                edges.emplace_back(new Edge("horizontal", tiles[j][i], tiles[j-1][i]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j-1][i]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i]->coords);
            }
            //j+1;
            if(j+1<32&& tiles[j+1][i]->ispassable){
                left=true;
                edges.emplace_back(new Edge("horizontal", tiles[j][i], tiles[j+1][i]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j+1][i]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i]->coords);
            }
            //i-1, j-1;
            if(up && right && tiles[j-1][i-1]->ispassable){
                edges.emplace_back(new Edge("diagonal", tiles[j][i], tiles[j-1][i-1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i-1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j-1][i-1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i-1]->coords);
            }
            //i-1, j+1;
             if(up && left && tiles[j+1][i-1]->ispassable){
                edges.emplace_back(new Edge("diagonal", tiles[j][i], tiles[j+1][i-1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i-1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j+1][i-1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i-1]->coords);
            }
        
            //i+1, j-1;
            if(down && right && tiles[j-1][i+1]->ispassable){
                edges.emplace_back(new Edge("diagonal", tiles[j][i], tiles[j-1][i+1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i+1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j-1][i+1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j-1][i+1]->coords);

            }

            //i+1, j+1;

            if(down && left && tiles[j+1][i+1]->ispassable){
                edges.emplace_back(new Edge("diagonal", tiles[j][i], tiles[j+1][i+1]));
                edges.back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i+1]->coords);
                map[tiles[j][i]->id].emplace_back(new Edge("vertical", tiles[j][i], tiles[j+1][i+1]));
                map[tiles[j][i]->id].back()->distance=distancelength(tiles[j][i]->coords, tiles[j+1][i+1]->coords);

            }
            }

        }
    }

}


