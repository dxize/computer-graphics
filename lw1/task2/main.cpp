#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class Background final : public sf::Drawable
{
public:
    Background()
    {
        build();
    }

private:
    sf::RectangleShape sky_;
    sf::RectangleShape grass_;
    sf::CircleShape sun_;

private:
    static sf::RectangleShape makeRect(sf::Vector2f size, sf::Vector2f pos, sf::Color fill)
    {
        sf::RectangleShape r(size);
        r.setPosition(pos);
        r.setFillColor(fill);
        return r;
    }

    static sf::CircleShape makeCircle(float radius, sf::Vector2f pos, sf::Color fill)
    {
        sf::CircleShape c(radius);
        c.setPosition(pos);
        c.setFillColor(fill);
        return c;
    }

    void build()
    {
        sky_ = makeRect({ 900.f, 600.f }, { 0.f, 0.f }, sf::Color(170, 220, 255));
        grass_ = makeRect({ 900.f, 180.f }, { 0.f, 420.f }, sf::Color(90, 180, 90));
        sun_ = makeCircle(40.f, { 60.f, 50.f }, sf::Color(255, 220, 80));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(sky_, states);
        target.draw(sun_, states);
        target.draw(grass_, states);
    }
};

class Fence final : public sf::Drawable, public sf::Transformable
{
public:
    Fence(sf::Vector2f origin, float length,
        float picketW = 18.f, float picketH = 85.f, float gap = 6.f)
        : picketW_(picketW), picketH_(picketH), gap_(gap)
    {
        setPosition(origin);   // origin = (x0, fenceY)
        rebuild(length);
    }

private:
    std::vector<sf::RectangleShape> pickets_;
    sf::RectangleShape rail1_;
    sf::RectangleShape rail2_;

    float picketW_;
    float picketH_;
    float gap_;

private:
    static sf::RectangleShape makeRect(sf::Vector2f size, sf::Vector2f pos, sf::Color fill)
    {
        sf::RectangleShape r(size);
        r.setPosition(pos);
        r.setFillColor(fill);
        return r;
    }

    sf::RectangleShape makePicket(float x) const
    {
        sf::RectangleShape p({ picketW_, picketH_ });
        p.setPosition({ x, -picketH_ }); // локально: вверх = отрицательный y
        p.setFillColor(sf::Color(210, 190, 140));
        p.setOutlineThickness(2.f);
        p.setOutlineColor(sf::Color(120, 95, 60));
        return p;
    }

    sf::RectangleShape makeRail(float width, float yOffset) const
    {
        // локально: yOffset отрицательный = выше линии fenceY
        return makeRect({ width, 10.f }, { 0.f, yOffset }, sf::Color(190, 170, 120));
    }

    void rebuild(float length)
    {
        pickets_.clear();

        for (float x = 0.f; x <= length; x += (picketW_ + gap_))
            pickets_.push_back(makePicket(x));

        // Рейки строго по штакетинам (без хвоста)
        const float railLeft = pickets_.front().getPosition().x;           // 0
        const float railRight = pickets_.back().getPosition().x + picketW_;
        const float railW = railRight - railLeft;

        rail1_ = makeRail(railW, -60.f);
        rail2_ = makeRail(railW, -30.f);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(rail1_, states);
        target.draw(rail2_, states);
        for (const auto& p : pickets_) target.draw(p, states);
    }
};

class House final : public sf::Drawable, public sf::Transformable
{
public:
    explicit House(sf::Vector2f pos)
    {
        setPosition(pos); // pos = левый верх основания
        build();
    }

private:
    sf::RectangleShape base_;
    sf::ConvexShape roof_;

    sf::RectangleShape chimney_;
    sf::RectangleShape chimneyTop_;

    sf::RectangleShape door_;
    sf::CircleShape knob_;

    sf::RectangleShape windowFrame_;
    sf::RectangleShape wVert_;
    sf::RectangleShape wHor_;

private:
    static sf::RectangleShape makeRect(sf::Vector2f size, sf::Vector2f pos, sf::Color fill)
    {
        sf::RectangleShape r(size);
        r.setPosition(pos);
        r.setFillColor(fill);
        return r;
    }

    static sf::RectangleShape makeOutlinedRect(sf::Vector2f size, sf::Vector2f pos,
        sf::Color fill, float outlineTh, sf::Color outline)
    {
        sf::RectangleShape r(size);
        r.setPosition(pos);
        r.setFillColor(fill);
        r.setOutlineThickness(outlineTh);
        r.setOutlineColor(outline);
        return r;
    }

    static sf::CircleShape makeCircle(float radius, sf::Vector2f pos, sf::Color fill)
    {
        sf::CircleShape c(radius);
        c.setPosition(pos);
        c.setFillColor(fill);
        return c;
    }

    void buildBase()
    {
        base_ = makeOutlinedRect({ 320.f, 220.f }, { 0.f, 0.f },
            sf::Color(230, 200, 160),
            3.f, sf::Color(120, 90, 60));
    }

    void buildRoof()
    {
        roof_ = sf::ConvexShape();
        roof_.setPointCount(3);
        roof_.setPoint(0, { -20.f, 0.f });
        roof_.setPoint(1, { 160.f, -110.f });
        roof_.setPoint(2, { 340.f, 0.f });
        roof_.setFillColor(sf::Color(160, 60, 50));
        roof_.setOutlineThickness(3.f);
        roof_.setOutlineColor(sf::Color(90, 30, 25));
    }

    void buildChimney()
    {
        chimney_ = makeOutlinedRect({ 45.f, 95.f }, { 230.f, -95.f },
            sf::Color(150, 80, 70),
            3.f, sf::Color(80, 40, 35));

        chimneyTop_ = makeRect({ 55.f, 18.f }, { 225.f, -105.f }, sf::Color(120, 60, 55));
    }

    void buildDoor()
    {
        door_ = makeOutlinedRect({ 70.f, 120.f }, { 140.f, 100.f },
            sf::Color(120, 70, 35),
            3.f, sf::Color(60, 35, 18));

        knob_ = makeCircle(5.f, { 195.f, 160.f }, sf::Color(240, 220, 120));
    }

    void buildWindow()
    {
        windowFrame_ = makeOutlinedRect({ 90.f, 90.f }, { 40.f, 65.f },
            sf::Color(210, 235, 255),
            3.f, sf::Color(80, 80, 80));

        wVert_ = makeRect({ 4.f, 90.f }, { 83.f, 65.f }, sf::Color(80, 80, 80));
        wHor_ = makeRect({ 90.f, 4.f }, { 40.f, 108.f }, sf::Color(80, 80, 80));
    }

    void build()
    {
        buildBase();
        buildRoof();
        buildChimney();
        buildDoor();
        buildWindow();
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();

        target.draw(base_, states);
        target.draw(chimney_, states);
        target.draw(chimneyTop_, states);
        target.draw(roof_, states);

        target.draw(windowFrame_, states);
        target.draw(wVert_, states);
        target.draw(wHor_, states);

        target.draw(door_, states);
        target.draw(knob_, states);
    }
};

class Path final : public sf::Drawable
{
public:
    Path()
    {
        build();
    }

private:
    sf::ConvexShape path_;

private:
    void build()
    {
        path_ = sf::ConvexShape();
        path_.setPointCount(4);
        path_.setPoint(0, { 455.f, 470.f });
        path_.setPoint(1, { 475.f, 470.f });
        path_.setPoint(2, { 520.f, 600.f });
        path_.setPoint(3, { 410.f, 600.f });
        path_.setFillColor(sf::Color(170, 150, 120));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        target.draw(path_, states);
    }
};

class App
{
public:
    App()
        : window_(createWindow())
        , bg_()
        , fence_({ 70.f, 420.f }, 830.f - 70.f) // origin(x0,fenceY), length
        , house_({ 290.f, 250.f })
        , path_()
    {
        window_.setFramerateLimit(60);
    }

    int run()
    {
        while (window_.isOpen())
        {
            handleEvents();
            render();
        }
        return 0;
    }

private:
    sf::RenderWindow window_;

    Background bg_;
    Fence fence_;
    House house_;
    Path path_;

private:
    static sf::RenderWindow createWindow()
    {
        sf::ContextSettings settings;
        settings.antiAliasingLevel = 8;

        return sf::RenderWindow(
            sf::VideoMode({ 900u, 600u }),
            "Domik v derevne (SFML 3)",
            sf::Style::Titlebar | sf::Style::Close,
            sf::State::Windowed,
            settings
        );
    }

    void handleEvents()
    {
        while (const std::optional<sf::Event> e = window_.pollEvent())
        {
            if (e->is<sf::Event::Closed>())
                window_.close();

            if (const auto* key = e->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Escape)
                    window_.close();
            }
        }
    }

    void render()
    {
        window_.clear();

        window_.draw(bg_);
        window_.draw(fence_); 
        window_.draw(path_);
        window_.draw(house_);

        window_.display();
    }
};

int main()
{
    App app;
    return app.run();
}