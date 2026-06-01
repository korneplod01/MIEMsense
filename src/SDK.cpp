#include <windows.h>
#include <tlhelp32.h>

#include "structs.h"

DWORD_PTR getModuleBaseAddress(HANDLE appSnapshot, const char* moduleName) {
  if (appSnapshot != INVALID_HANDLE_VALUE) {
    MODULEENTRY32 moduleEntry;
    moduleEntry.dwSize = sizeof(moduleEntry);
    DWORD_PTR modBaseAddress;

    if (Module32First(appSnapshot, &moduleEntry)) {
      do {
        if (_stricmp(moduleEntry.szModule, moduleName) == 0) {
          modBaseAddress = (DWORD_PTR)moduleEntry.modBaseAddr;

          return modBaseAddress;
        }
      } while (Module32Next(appSnapshot, &moduleEntry));
    }
  }
  return 0;
}

void drawBox(ESPbox boxToDraw, HDC HDcontext, RGBcolor color) {
  HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));

  int x = (int)boxToDraw.startPoint.x;
  int y = (int)boxToDraw.startPoint.y;
  int w = (int)boxToDraw.width;
  int h = (int)boxToDraw.height;

  RECT top = {x, y, x + w, y + 3};
  RECT left = {x, y, x + 3, y + h};
  RECT right = {x + w - 3, y, x + w, y + h};
  RECT bottom = {x, y + h - 3, x + w, y + h};

  FillRect(HDcontext, &top, brush);
  FillRect(HDcontext, &left, brush);
  FillRect(HDcontext, &right, brush);
  FillRect(HDcontext, &bottom, brush);

  DeleteObject(brush);
}