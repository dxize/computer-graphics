#pragma once
#include <SFML/Graphics.hpp>

struct Palette 
{
    //Background
    inline static const sf::Color Sky = sf::Color(170, 220, 255);
    inline static const sf::Color Grass = sf::Color(90, 180, 90);
    inline static const sf::Color Sun = sf::Color(255, 220, 80);

    //Fence
    inline static const sf::Color PicketFill = sf::Color(210, 190, 140);
    inline static const sf::Color PicketOutline = sf::Color(120, 95, 60);
    inline static const sf::Color RailFill = sf::Color(190, 170, 120);

    //House
    inline static const sf::Color HouseBaseFill = sf::Color(230, 200, 160);
    inline static const sf::Color HouseBaseOutline = sf::Color(120, 90, 60);

    inline static const sf::Color RoofFill = sf::Color(160, 60, 50);
    inline static const sf::Color RoofOutline = sf::Color(90, 30, 25);

    inline static const sf::Color ChimneyFill = sf::Color(150, 80, 70);
    inline static const sf::Color ChimneyOutline = sf::Color(80, 40, 35);
    inline static const sf::Color ChimneyTopFill = sf::Color(120, 60, 55);

    inline static const sf::Color DoorFill = sf::Color(120, 70, 35);
    inline static const sf::Color DoorOutline = sf::Color(60, 35, 18);
    inline static const sf::Color KnobFill = sf::Color(240, 220, 120);

    inline static const sf::Color WindowFill = sf::Color(210, 235, 255);
    inline static const sf::Color WindowOutline = sf::Color(80, 80, 80);

    //Path
    inline static const sf::Color PathFill = sf::Color(170, 150, 120);
};