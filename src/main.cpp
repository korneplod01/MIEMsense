#include <windows.h>
#include <tlhelp32.h>

#include <iostream>

#include "SDK.h"
#include "math.h"
#include "structs.h"

float prevAnimFrames[32] = {};

int main() {
  try {
    HWND csWindow = FindWindowA(NULL, "Counter-Strike");
    if (csWindow == NULL) {
      throw std::runtime_error(
          "'Counter-Strike' window not found. Please launch the game first.");
    }

    DWORD csPID;
    GetWindowThreadProcessId(csWindow, &csPID);

    std::cout << "CS1.6 PID: " << csPID << std::endl;

    HANDLE csHandle = OpenProcess(PROCESS_VM_READ, FALSE, csPID);

    if (csHandle == NULL) {
      throw std::runtime_error(
          "Failed to obtain CS1.6 handle. Make sure the game is active and "
          "running.");
    }

    HANDLE csSnapshot = CreateToolhelp32Snapshot(
        TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, csPID);

    DWORD_PTR hwBase = getModuleBaseAddress(csSnapshot, "hw.dll");
    DWORD_PTR clientBase = getModuleBaseAddress(csSnapshot, "client.dll");
    CloseHandle(csSnapshot);

    std::cout << "hwBase: 0x" << std::hex << hwBase << std::endl;
    std::cout << "clientBase: 0x" << std::hex << clientBase << std::endl;

    while (TRUE) {
      RECT clientRect;
      GetClientRect(csWindow, &clientRect);
      int screenWidth = clientRect.right;
      int screenHeight = clientRect.bottom;
      HDC deviceContext = GetDC(csWindow);

      InvalidateRect(csWindow, NULL, FALSE);

      CS_View localView;
      if (!ReadProcessMemory(csHandle, (LPCVOID)(clientBase + 0x12EAF0),
                             &localView, sizeof(localView), NULL)) {
        ReleaseDC(csWindow, deviceContext);
        throw std::runtime_error("Lost connection to the game process.");
      }

      DWORD_PTR entityListAddress = hwBase + 0x12043C8;

      float fovScale1 = 0.0f;
      float fovScale2 = 0.0f;
      ReadProcessMemory(csHandle, (LPCVOID)(hwBase + 0xEC9E20), &fovScale1,
                        sizeof(fovScale1), NULL);
      ReadProcessMemory(csHandle, (LPCVOID)(hwBase + 0xEC9E34), &fovScale2,
                        sizeof(fovScale2), NULL);

      for (int i = 0; i < 32; i++) {
        DWORD_PTR currentPlayerAddress =
            entityListAddress + (i * sizeof(PlayerInfo));
        PlayerInfo currentPlayer;
        ReadProcessMemory(csHandle, (LPCVOID)(currentPlayerAddress),
                          &currentPlayer, sizeof(PlayerInfo), NULL);

        if (currentPlayer.anim_frame == prevAnimFrames[i]) continue;
        prevAnimFrames[i] = currentPlayer.anim_frame;

        if (currentPlayer.name[0] == '\0') {
          continue;
        }

        Vector3 feetPosition = {currentPlayer.position.x,
                                currentPlayer.position.y,
                                currentPlayer.position.z - 36.f};
        Vector3 headPosition = {currentPlayer.position.x,
                                currentPlayer.position.y,
                                currentPlayer.position.z + 36.f};
        Vector2 feetScreenCoords =
            worldToScreen(feetPosition, localView, fovScale1,
                          fovScale2, screenWidth, screenHeight);
        Vector2 headScreenCoords =
            worldToScreen(headPosition, localView, fovScale1,
                          fovScale2, screenWidth, screenHeight);

        if ((feetScreenCoords.x == 0 && feetScreenCoords.y == 0) ||
            (headScreenCoords.x == 0 && headScreenCoords.y == 0)) {
          continue;
        }

        float boxHeight = feetScreenCoords.y - headScreenCoords.y;
        float boxWidth = boxHeight / 2.0f;
        Vector2 startPoint = {headScreenCoords.x - (boxWidth / 2.0f),
                              headScreenCoords.y};

        ESPbox playerBox = {boxHeight, boxWidth, startPoint};

        if (getTeamFromModel(currentPlayer.model) == Team::T) {
          drawBox(playerBox, deviceContext, {255, 0, 0});
        } else if (getTeamFromModel(currentPlayer.model) == Team::CT) {
          drawBox(playerBox, deviceContext, {0, 0, 255});
        } else {
          drawBox(playerBox, deviceContext, {0, 255, 0});
        }
      }
      ReleaseDC(csWindow, deviceContext);
      Sleep(15);
    }
  } catch (const std::exception& error) {
    std::cerr << "Error: " << error.what() << std::endl;
    return 1;
  }

  return 0;
}