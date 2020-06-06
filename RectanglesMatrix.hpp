#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

struct RectanglesMatrix : sf::Drawable, sf::Transformable
{
    RectanglesMatrix(sf::Vector2u resolution, sf::Vector2f size)
    : resolution_(resolution),
      size_(size),
      points_(sf::PrimitiveType::Quads, 4u * resolution_.x * resolution_.y)
    {
        const auto rectSize = sf::Vector2f(size_.x / resolution_.x, size_.y / resolution_.y);
        for (auto row = 0u; row < resolution_.y; ++row)
        {
            for (auto col = 0u; col < resolution_.x; ++col)
            {
                const auto offset = 4u * (row * resolution_.x + col);
                points_[offset + 0u] = {{col * rectSize.x + 0.f,        row * rectSize.y + 0.f},        sf::Color::Black};
                points_[offset + 1u] = {{col * rectSize.x + rectSize.x, row * rectSize.y + 0.f},        sf::Color::Black};
                points_[offset + 2u] = {{col * rectSize.x + rectSize.x, row * rectSize.y + rectSize.y}, sf::Color::Black};
                points_[offset + 3u] = {{col * rectSize.x + 0.f,        row * rectSize.y + rectSize.y}, sf::Color::Black};
            }
        }
    }

    sf::Vector2u resolution() const
    {
        return resolution_;
    }

    sf::Vector2f size() const
    {
        return size_;
    }

    void setColor(sf::Vector2u index, sf::Color color)
    {
        const auto offset = 4u * (index.y * resolution().x + index.x);
        points_[offset + 0u].color = color;
        points_[offset + 1u].color = color;
        points_[offset + 2u].color = color;
        points_[offset + 3u].color = color;
    }

    // sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        target.draw(points_, states);
    }

private:
    sf::Vector2u resolution_;
    sf::Vector2f size_;
    sf::VertexArray points_;
};