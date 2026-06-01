#include <windows.h>
#include <tlhelp32.h>

#include "SDK.h"
#include "doctest.h"
#include "math.h"
#include "structs.h"

TEST_SUITE("worldToScreen") {
  TEST_CASE("Положительный результат: объект перед камерой") {
    CS_View mockView;
    mockView.vForward = {1.0f, 0.0f, 0.0f};
    mockView.vRight = {0.0f, 1.0f, 0.0f};
    mockView.vUpward = {0.0f, 0.0f, 1.0f};
    mockView.vOrigin = {0.0f, 0.0f, 0.0f};

    Vector3 enemyInFront = {100.0f, 0.0f, 0.0f};
    Vector2 result =
        worldToScreen(enemyInFront, mockView, 1.0f, 1.0f, 800, 600);

    CHECK(result.x == 400.0f);
    CHECK(result.y == 324.0f);
  }

  TEST_CASE("Отрицательный результат: объект за спиной игрока") {
    CS_View mockView;
    mockView.vForward = {1.0f, 0.0f, 0.0f};
    mockView.vRight = {0.0f, 1.0f, 0.0f};
    mockView.vUpward = {0.0f, 0.0f, 1.0f};
    mockView.vOrigin = {0.0f, 0.0f, 0.0f};

    Vector3 enemyBehind = {-50.0f, 0.0f, 0.0f};
    Vector2 result = worldToScreen(enemyBehind, mockView, 1.0f, 1.0f, 800, 600);

    CHECK(result.x == 0.0f);
    CHECK(result.y == 0.0f);
  }
}

TEST_SUITE("getTeamFromModel") {
  TEST_CASE("Положительный результат: валидные модели команд") {
    CHECK(getTeamFromModel("terror") == Team::T);
    CHECK(getTeamFromModel("leet") == Team::T);
    CHECK(getTeamFromModel("gign") == Team::CT);
    CHECK(getTeamFromModel("sas") == Team::CT);
  }

  TEST_CASE("Отрицательный результат: неизвестная или пустая модель") {
    CHECK(getTeamFromModel("unknown_model") == Team::UNKNOWN);
    CHECK(getTeamFromModel("") == Team::UNKNOWN);
  }
}

TEST_SUITE("getModuleBaseAddress") {
  TEST_CASE("Отрицательный результат: невалидный хэндл снапшота") {
    DWORD_PTR result = getModuleBaseAddress(INVALID_HANDLE_VALUE, "hw.dll");
    CHECK(result == 0);
  }

  TEST_CASE("Отрицательный результат: модуль отсутствует в системе") {
    HANDLE emptySnapshot =
        CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    if (emptySnapshot != INVALID_HANDLE_VALUE) {
      DWORD_PTR result =
          getModuleBaseAddress(emptySnapshot, "non_existent_module.dll");
      CHECK(result == 0);
      CloseHandle(emptySnapshot);
    }
  }
}