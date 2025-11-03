#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "particle.h"
#include "constraint.h"
#include "input_handler.h"

constexpr unsigned WIDTH  = 1080;
constexpr unsigned HEIGHT = 640;
constexpr float    GRAVITY = 980.f;
constexpr float    DT = 1.f / 60.f;
constexpr int      ROWS = 20;
constexpr int      COLS = 30;

int main() {
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Cloth Simulation (SFML 3.0.2)");
    window.setVerticalSyncEnabled(true);

    std::vector<std::unique_ptr<Particle>> particles;
    particles.reserve(ROWS * COLS);

    const float startX = 100.f, startY = 50.f, spacing = 12.f;
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            particles.emplace_back(std::make_unique<Particle>(
                sf::Vector2f(startX + c * spacing, startY + r * spacing)));

    for (int c = 0; c < COLS; ++c) particles[c]->isPinned = true;

    std::vector<Constraint> constraints;
    auto id = [&](int r, int c){ return r * COLS + c; };
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c) {
            if (c + 1 < COLS) constraints.emplace_back(particles[id(r,c)].get(), particles[id(r,c+1)].get());
            if (r + 1 < ROWS) constraints.emplace_back(particles[id(r,c)].get(), particles[id(r+1,c)].get());
            if (r + 1 < ROWS && c + 1 < COLS) constraints.emplace_back(particles[id(r,c)].get(), particles[id(r+1,c+1)].get());
            if (r + 1 < ROWS && c     > 0)   constraints.emplace_back(particles[id(r,c)].get(), particles[id(r+1,c-1)].get());
        }

    InputHandler input(window, particles);
    sf::CircleShape shape(4.f);
    shape.setOrigin({4.f,4.f});

    sf::Clock clock; float acc = 0.f;
    while (window.isOpen()) {
        while (const auto e = window.pollEvent()) {
            if (e->is<sf::Event::Closed>()) window.close();
            else input.handleEvent(*e);
        }

        acc += clock.restart().asSeconds();
        while (acc >= DT) {
            for (auto& p : particles) if (!p->isPinned) p->applyForce({0.f, GRAVITY});
            for (auto& p : particles) p->update(DT);
            for (int i = 0; i < 5; ++i) {
                for (auto& c : constraints) c.satisfy();
                for (auto& p : particles) p->constrainToBounds(WIDTH, HEIGHT);
            }
            input.update(DT);
            acc -= DT;
        }

        window.clear(sf::Color(30,30,30));
        std::vector<sf::Vertex> verts;
        verts.reserve(constraints.size()*2);
        for (auto& c : constraints) {
            sf::Vertex v1, v2;
            v1.position = c.p1->position; v2.position = c.p2->position;
            v1.color = v2.color = sf::Color(200,200,200);
            verts.push_back(v1); verts.push_back(v2);
        }
        if (!verts.empty()) window.draw(verts.data(), verts.size(), sf::PrimitiveType::Lines);

        for (auto& p : particles) {
            shape.setPosition(p->position);
            shape.setFillColor(p->isPinned ? sf::Color(200,80,80) : sf::Color::White);
            window.draw(shape);
        }
        window.display();
    }
}

