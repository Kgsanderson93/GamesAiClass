#pragma once
#include<vector>
#include <SFML/Graphics.hpp>

    float length(sf::Vector2f vec);
    bool inRadiusDecel(float distance);
    bool inRadiusSat(float distance);
    sf::Vector2f normalize(sf::Vector2f vec);
    float mapToRange(float roation);
    float sqrMag(sf::Vector2f vec);
       // Utility functions for degree-radian conversion
    float degreesToRadians(float degrees);

    float radiansToDegrees(float radians);
  

 // namespautil