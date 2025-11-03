// particle.h
#pragma once
#include <SFML/Graphics.hpp>

struct Particle {
    sf::Vector2f position;
    sf::Vector2f previous_position;
    sf::Vector2f acceleration = {0.f, 0.f};
    bool isPinned = false;
    bool active = true;

    Particle() = default;
    explicit Particle(const sf::Vector2f &pos)
        : position(pos), previous_position(pos) {}

    void applyForce(const sf::Vector2f &f) {
        acceleration += f;
    }

    void update(float time_step) {
        if (isPinned) return;
        // Verlet integration
        sf::Vector2f velocity = position - previous_position;
        previous_position = position;
        position += velocity + acceleration * (time_step * time_step);
        // reset acceleration
        acceleration = {0.f, 0.f};
    }

    void constrainToBounds(float width, float height) {
        if (position.x < 0.f) position.x = 0.f;
        if (position.x > width) position.x = width;
        if (position.y < 0.f) position.y = 0.f;
        if (position.y > height) position.y = height;
    }

    void moveTo(const sf::Vector2f &newPos) {
        position = newPos;
        previous_position = newPos;
    }
};

