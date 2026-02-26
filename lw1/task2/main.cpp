#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <vector>

// Удобная функция: тонкая "линия" как прямоугольник (если понадобится)
sf::RectangleShape line(sf::Vector2f a, sf::Vector2f b, float thickness, sf::Color color)
{
    sf::Vector2f d = b - a;
    float length = std::sqrt(d.x * d.x + d.y * d.y);

    sf::RectangleShape l({ length, thickness });
    l.setFillColor(color);
    l.setOrigin({ 0.f, thickness / 2.f });
    l.setPosition(a);

    sf::Angle angle = sf::radians(std::atan2(d.y, d.x));
    l.setRotation(angle);
    return l;
}

// ---------- Сцена / объекты ----------
struct Fence
{
    std::vector<sf::RectangleShape> pickets;
    sf::RectangleShape rail1;
    sf::RectangleShape rail2;

    void build(float fenceY, float fenceX0, float fenceX1, float picketW, float picketH, float gap)
    {
        pickets.clear();

        for (float x = fenceX0; x <= fenceX1; x += (picketW + gap))
        {
            sf::RectangleShape p({ picketW, picketH });
            p.setPosition({ x, fenceY - picketH });
            p.setFillColor(sf::Color(210, 190, 140));
            p.setOutlineThickness(2.f);
            p.setOutlineColor(sf::Color(120, 95, 60));
            pickets.push_back(p);
        }

        // Рейки ровно по первой и последней штакетине (без "хвоста")
        const float railLeft = pickets.front().getPosition().x;
        const float railRight = pickets.back().getPosition().x + picketW;
        const float railW = railRight - railLeft;

        rail1 = sf::RectangleShape({ railW, 10.f });
        rail1.setPosition({ railLeft, fenceY - 60.f });
        rail1.setFillColor(sf::Color(190, 170, 120));

        rail2 = sf::RectangleShape({ railW, 10.f });
        rail2.setPosition({ railLeft, fenceY - 30.f });
        rail2.setFillColor(sf::Color(190, 170, 120));
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(rail1);
        target.draw(rail2);
        for (const auto& p : pickets) target.draw(p);
    }
};

void buildSky(sf::RectangleShape& sky)
{
    sky = sf::RectangleShape({ 900.f, 600.f });
    sky.setFillColor(sf::Color(170, 220, 255));
}

void buildGrass(sf::RectangleShape& grass)
{
    grass = sf::RectangleShape({ 900.f, 180.f });
    grass.setPosition({ 0.f, 420.f });
    grass.setFillColor(sf::Color(90, 180, 90));
}

void buildSun(sf::CircleShape& sun)
{
    sun = sf::CircleShape(40.f);
    sun.setPosition({ 60.f, 50.f });
    sun.setFillColor(sf::Color(255, 220, 80));
}

void buildHouseBase(sf::RectangleShape& houseBase)
{
    houseBase = sf::RectangleShape({ 320.f, 220.f });
    houseBase.setPosition({ 290.f, 250.f });
    houseBase.setFillColor(sf::Color(230, 200, 160));
    houseBase.setOutlineThickness(3.f);
    houseBase.setOutlineColor(sf::Color(120, 90, 60));
}

void buildRoof(sf::ConvexShape& roof)
{
    roof = sf::ConvexShape();
    roof.setPointCount(3);
    roof.setPoint(0, { 270.f, 250.f });
    roof.setPoint(1, { 450.f, 140.f });
    roof.setPoint(2, { 630.f, 250.f });
    roof.setFillColor(sf::Color(160, 60, 50));
    roof.setOutlineThickness(3.f);
    roof.setOutlineColor(sf::Color(90, 30, 25));
}

void buildChimney(sf::RectangleShape& chimney, sf::RectangleShape& chimneyTop)
{
    chimney = sf::RectangleShape({ 45.f, 95.f });
    chimney.setPosition({ 520.f, 155.f });
    chimney.setFillColor(sf::Color(150, 80, 70));
    chimney.setOutlineThickness(3.f);
    chimney.setOutlineColor(sf::Color(80, 40, 35));

    chimneyTop = sf::RectangleShape({ 55.f, 18.f });
    chimneyTop.setPosition({ 515.f, 145.f });
    chimneyTop.setFillColor(sf::Color(120, 60, 55));
}

void buildDoor(sf::RectangleShape& door, sf::CircleShape& doorKnob)
{
    door = sf::RectangleShape({ 70.f, 120.f });
    door.setPosition({ 430.f, 350.f });
    door.setFillColor(sf::Color(120, 70, 35));
    door.setOutlineThickness(3.f);
    door.setOutlineColor(sf::Color(60, 35, 18));

    doorKnob = sf::CircleShape(5.f);
    doorKnob.setFillColor(sf::Color(240, 220, 120));
    doorKnob.setPosition({ 485.f, 410.f });
}

void buildWindow(sf::RectangleShape& windowFrame, sf::RectangleShape& wVert, sf::RectangleShape& wHor)
{
    windowFrame = sf::RectangleShape({ 90.f, 90.f });
    windowFrame.setPosition({ 330.f, 315.f });
    windowFrame.setFillColor(sf::Color(210, 235, 255));
    windowFrame.setOutlineThickness(3.f);
    windowFrame.setOutlineColor(sf::Color(80, 80, 80));

    wVert = sf::RectangleShape({ 4.f, 90.f });
    wVert.setPosition({ 330.f + 43.f, 315.f });
    wVert.setFillColor(sf::Color(80, 80, 80));

    wHor = sf::RectangleShape({ 90.f, 4.f });
    wHor.setPosition({ 330.f, 315.f + 43.f });
    wHor.setFillColor(sf::Color(80, 80, 80));
}

void buildPath(sf::ConvexShape& path)
{
    path = sf::ConvexShape();
    path.setPointCount(4);
    path.setPoint(0, { 455.f, 470.f });
    path.setPoint(1, { 475.f, 470.f });
    path.setPoint(2, { 520.f, 600.f });
    path.setPoint(3, { 410.f, 600.f });
    path.setFillColor(sf::Color(170, 150, 120));
}

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(
        sf::VideoMode({ 900u, 600u }),
        "Domik v derevne",
        sf::Style::Titlebar | sf::Style::Close,
        sf::State::Windowed,
        settings
    );
    window.setFramerateLimit(60);

    // --- Объекты сцены ---
    sf::RectangleShape sky, grass;
    sf::CircleShape sun;

    sf::RectangleShape houseBase;
    sf::ConvexShape roof;

    sf::RectangleShape chimney, chimneyTop;

    sf::RectangleShape door;
    sf::CircleShape doorKnob;

    sf::RectangleShape windowFrame, wVert, wHor;

    Fence fence;
    sf::ConvexShape path;

    // --- Построение ---
    buildSky(sky);
    buildGrass(grass);
    buildSun(sun);

    buildHouseBase(houseBase);
    buildRoof(roof);
    buildChimney(chimney, chimneyTop);

    buildDoor(door, doorKnob);
    buildWindow(windowFrame, wVert, wHor);

    fence.build(
        /*fenceY*/ 420.f,
        /*fenceX0*/ 70.f,
        /*fenceX1*/ 830.f,
        /*picketW*/ 18.f,
        /*picketH*/ 85.f,
        /*gap*/ 6.f
    );

    buildPath(path);

    // --- Цикл ---
    while (window.isOpen())
    {
        while (const std::optional<sf::Event> e = window.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = e->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        window.clear();

        window.draw(sky);
        window.draw(sun);
        window.draw(grass);

        window.draw(path);

        // Забор позади дома
        fence.draw(window);

        // Дом
        window.draw(houseBase);
        window.draw(chimney);
        window.draw(chimneyTop);
        window.draw(roof);

        window.draw(windowFrame);
        window.draw(wVert);
        window.draw(wHor);

        window.draw(door);
        window.draw(doorKnob);

        window.display();
    }

    return 0;
}