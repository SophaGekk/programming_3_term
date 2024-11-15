#pragma once
#include "Ball.hpp"
#include "Dust.hpp"
#include "Velocity.h"
#include "Physics.h"
#include <vector>

class Painter;

class World {
  public:
    World(const std::string& worldFilePath);
    void show(Painter& painter) const;
    void update(double time);

  private:
    // Границы мира заданы углами прямоугольника
    Point topLeft;
    Point bottomRight;
    // Объект физического движка
    Physics physics;
    // Контейнер с шарами
    std::vector<Ball> balls;
    // Контейнер с частицами
    std::vector<Dust> dusts;
    // Длина отрезка времени, который не был
    // учтен при прошлой симуляции. См. реализацию update
    double restTime = 0.;
};
