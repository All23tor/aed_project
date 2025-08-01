#include "Utils.h"



namespace Utils{
    void ClearInputBuffer() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    float GetDistance(raylib::Vector2 p1, raylib::Vector2 p2) {
        return p1.Distance(p2);
    }
    bool CheckLineRectangleCollision(raylib::Vector2 p1, raylib::Vector2 p2, raylib::Rectangle rec) {
        
        if (CheckCollisionPointRec(p1, rec) || CheckCollisionPointRec(p2, rec)) {
            return true;
        }

        
        if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x + rec.width, rec.y}, nullptr)) return true; // Lado superior
        if (CheckCollisionLines(p1, p2, {rec.x, rec.y + rec.height}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true; // Lado inferior
        if (CheckCollisionLines(p1, p2, {rec.x, rec.y}, {rec.x, rec.y + rec.height}, nullptr)) return true; // Lado izquierdo
        if (CheckCollisionLines(p1, p2, {rec.x + rec.width, rec.y}, {rec.x + rec.width, rec.y + rec.height}, nullptr)) return true; // Lado derecho

        return false;
    }
}