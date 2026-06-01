#pragma once
#include "structs.h"

/**
 * @brief Функция для проецирования трехмерных координат мира на экран
 * @details Переводит 3D-вектор позиции цели в относительную систему координат
 * камеры игрока, проверяет глубину видимости и масштабирует проекцию по углам
 * обзора
 * @param targetPosition Трехмерные координаты центра модели игрока-цели
 * @param playerView CS_View-структура камеры локального игрока
 * @param fovScale1 Коэффициент горизонтального масштабирования угла обзора
 * @param fovScale2 Коэффициент вертикального масштабирования угла обзора
 * @param screenWidth Ширина окна игры в пикселях
 * @param screenHeight Высота окна игры в пикселях
 * @return Двумерный вектор экранных координат игрока-цели; если цель за спиной возвращается { 0, 0 }
 */
Vector2 worldToScreen(Vector3 targetPosition, CS_View playerView,
                      float fovScale1, float fovScale2, int screenWidth,
                      int screenHeight);

enum class Team { T, CT, UNKNOWN };

/**
 * @brief Определяет команду игрока по названию его 3D-модели
 * @param modelName Имя модели персонажа
 * @return Team Идентификатор команды (террористы, спецназ или неизвестно)
 */
Team getTeamFromModel(const std::string& modelName);