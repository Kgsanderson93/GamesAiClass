
#include "util.h"
#include<vector>
#include <SFML/Graphics.hpp>
#include <math.h>


    float length(sf::Vector2f vec){
        float x=pow(vec.x, 2.0);
        float y=pow(vec.y, 2.0);
        float total=x+y;
        return sqrtf( total);
    }
    bool inRadiusDecel(float distance){
        return (distance<=1);
    }

    bool inRadiusSat(float distance){
        return (distance<=.1);

    }
    sf::Vector2f normalize(sf::Vector2f vec){
        sf::Vector2f normed;
        float l=length(vec);
        normed.x= vec.x/l;
        normed.y= vec.y/l;
        return normed;
    }

    float mapToRange(float rotation){
        if(rotation> (M_PI)){
            rotation=rotation-(M_PI);
        }
        if(rotation < (-M_PI)){
            rotation=rotation+(M_PI);
        }
        return rotation;
    }

 // namespautil

