#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <memory>

struct WindowRunner
{
    WindowRunner()
    : window_(std::make_unique<sf::RenderWindow>(sf::VideoMode(800u, 600u), "cellular machines"))
    {
        window_->setFramerateLimit(30.f);
    }

    template<typename F>
    void run(F&& f)
    {
        for (; window_->isOpen(); window_->display())
        {
            window_->clear();
            f(*window_);
        }   
    }

private:
    std::unique_ptr<sf::RenderWindow> window_;
};
