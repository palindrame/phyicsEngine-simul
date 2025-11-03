#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "particle.h"

class InputHandler {
public:
    InputHandler(sf::RenderWindow& win,
                 std::vector<std::unique_ptr<Particle>>& ps)
        : window(win), particles(ps) {}

    void handleEvent(const sf::Event&) {
        bool leftDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        sf::Vector2i pixel = sf::Mouse::getPosition(window);
        sf::Vector2f world = window.mapPixelToCoords(pixel);

        if (leftDown && !prevDown) startDrag(world);
        else if (!leftDown && prevDown) stopDrag();
        else if (leftDown && dragging && dragged)
            dragged->moveTo(world);

        prevDown = leftDown;
    }

    void update(float) {}

private:
    sf::RenderWindow& window;
    std::vector<std::unique_ptr<Particle>>& particles;
    Particle* dragged = nullptr;
    bool dragging = false, prevDown = false;

    void startDrag(const sf::Vector2f& pos) {
        const float R = 20.f;
        float best = R*R; Particle* near = nullptr;
        for (auto& p : particles) {
            float dx = p->position.x - pos.x, dy = p->position.y - pos.y;
            float d2 = dx*dx + dy*dy;
            if (d2 < best) { best = d2; near = p.get(); }
        }
        if (near) {
            dragged = near; dragging = true;
            dragged->isPinned = true;
            dragged->moveTo(pos);
        }
    }

    void stopDrag() {
        if (dragged) dragged->isPinned = false;
        dragging = false; dragged = nullptr;
    }
};

