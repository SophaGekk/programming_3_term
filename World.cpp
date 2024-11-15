#include "World.h"
#include "Painter.h"
#include <fstream>
#include "algorithm"

// Длительность одного тика симуляции.
// Изменять не следует
static constexpr double timePerTick = 0.001;

Point readPoint(std::istream& stream) {
    double x, y;
    stream >> x >> y;
    return Point(x, y);
}

Color readColor(std::istream& stream) {
    double red, green, blue;
    stream >> red >> green >> blue;
    return Color(red, green, blue);
}

/// @brief Конструирует объект мира для симуляции
/// @param worldFilePath путь к файлу модели мира
World::World(const std::string& worldFilePath) {
    std::ifstream stream(worldFilePath);
    topLeft = readPoint(stream);
    bottomRight = readPoint(stream);
    physics.setWorldBox(topLeft, bottomRight);

    double radius;
    bool isCollidable;
    while (stream.peek(), stream.good()) //для эффективного определения того, есть ли еще данные для чтения из потока
    {
        // Читаем координаты центра шара (x, y) и вектор
        Point center = readPoint(stream);
        Point vector = readPoint(stream);
        Velocity velocity(vector);
        // Читаем три составляющие цвета шара
        Color color = readColor(stream);
        // Читаем радиус шара
        stream >> radius;
        // Читаем свойство шара isCollidable, которое
        // указывает, требуется ли обрабатывать пересечение
        // шаров как столкновение. Если true - требуется.
        // В базовой части задания этот параметр
        stream >> std::boolalpha >> isCollidable;
        Ball ball(velocity, center, radius, color,isCollidable); // Создание объекта Ball
        balls.push_back(ball);
    }
}

/// @brief Отображает состояние мира
void World::show(Painter& painter) const {
    // Рисуем белый прямоугольник, отображающий границу
    // мира
    painter.draw(topLeft, bottomRight, Color(1, 1, 1));
    // Вызываем отрисовку каждого шара
    for (const Ball& ball : balls) {
        ball.draw(painter);
    }
    // Отрисовка частиц
    for (const Dust& dust : dusts) {
        if (dust.getIsAlive()) {
            dust.draw(painter);
        }
    }
}

/// @brief Обновляет состояние мира
void World::update(double time) {
    time += restTime;
    const auto ticks = static_cast<size_t>(std::floor(time / timePerTick));
    restTime = time - double(ticks) * timePerTick;
    // Обновляем физику для шаров и частиц
    physics.update(balls, dusts, ticks); // Передаем частицы в метод update
    // Обновление состояния частиц
    for (auto& dust : dusts) {
        dust.update(timePerTick); // Передаем время для обновления частиц
    }
    // Удаление мертвых частиц
    dusts.erase(std::remove_if(dusts.begin(), dusts.end(), [](const Dust& dust) {
        return !dust.getIsAlive();
    }), dusts.end());
}
