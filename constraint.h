// constraint.h
#pragma once
#include "particle.h"
#include <cmath>

struct Constraint {
    Particle *p1;
    Particle *p2;
    float restLength;
    bool active = true;

    Constraint(Particle *a, Particle *b)
        : p1(a), p2(b), restLength(length(a->position - b->position)) {}

    static float length(const sf::Vector2f &v) {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    void satisfy() {
        if (!active) return;
        sf::Vector2f delta = p2->position - p1->position;
        float current = length(delta);
        if (current == 0.f) return;
        float diff = (current - restLength) / current;
        // Push each particle unless pinned
        if (!p1->isPinned && !p2->isPinned) {
            p1->position += delta * 0.5f * diff;
            p2->position -= delta * 0.5f * diff;
        } else if (!p1->isPinned) {
            p1->position += delta * diff;
        } else if (!p2->isPinned) {
            p2->position -= delta * diff;
        }
    }
};

