#pragma once
#include <windows.h>

#include "structs.h"
/**
 * @brief Функция для получения базового адреса модуля программы
 * @param appSnapshot Снапшот программы, к которой подключен модуль
 * @param moduleName Название модуля с расширением, например, "hw.dll"
 * @return Базовый адрес указанного модуля; если модуль не найден, возвращается 0
 */
DWORD_PTR getModuleBaseAddress(HANDLE appSnapshot, const char* moduleName);

/**
 * @brief Функция для отрисовки ESP-бокса вокруг игрока
 * @param boxToDraw ESPbox-структура с параметрами самого бокса
 * @param HDcontext Дескриптор HDC экрана для вывода
 * @param color RGBcolor-структура цвета бокса
 */
void drawBox(ESPbox boxToDraw, HDC HDcontext, RGBcolor color);