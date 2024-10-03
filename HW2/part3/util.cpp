
#include "util.h"
#include<vector>
#include <SFML/Graphics.hpp>
#include <math.h>


    float length(sf::Vector2f vec){
        float x=(vec.x* vec.x);
        float y=(vec.y * vec.y);
        float total=x+y;
        float sqrt=sqrtf( total);
        return sqrt;
    }
    float sqrMag(sf::Vector2f vec){
        float x=(vec.x* vec.x);
        float y=(vec.y * vec.y);
        float total=x+y;
        return total;
    }
    bool inRadiusDecel(float distance){
        return (distance<=100);
    }

    bool inRadiusSat(float distance){
        return (distance<=5);

    }
    sf::Vector2f normalize(sf::Vector2f vec){
        sf::Vector2f normed=sf::Vector2f(0,0);
        float l=length(vec);
        if(l>0){
        normed.x= vec.x/l;
        normed.y= vec.y/l;
        }
        return normed;
    }

    float mapToRange(float rotation){
        //printf("IN MAP %f\n", rotation);
        rotation=fmodf(rotation, 360.f);
        if(rotation>=180.f){
            rotation-=360.f;
        }
        if(rotation<=-180.f){
            rotation+=360.f;
        }
        //printf("OUTMAP %f\n", rotation);
       return rotation;

    }

  // Utility functions for degree-radian conversion
  float degreesToRadians(float degrees){
    return degrees * M_PI / 180.0f;
  }

  float radiansToDegrees(float radians){
    return radians * 180.0f / M_PI;
  }
 // namespautil

float randomBinomial(){
    float rand=fmod(random(), 3);
    rand=rand-1;
     return rand;
}
float randomDirection(){
    float rand=fmod(random(), 360);
    rand = mapToRange(rand);
    return rand;
}

sf::Vector2f getOrientationAsVector(float orientation){
    return sf::Vector2f(sin(orientation), cos(orientation));
}
