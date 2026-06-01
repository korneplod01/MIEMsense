#include "math.h"

Vector2 worldToScreen(Vector3 targetPosition, CS_View playerView,
                      float fovScale1, float fovScale2, int screenWidth,
                      int screenHeight) {
  Vector3 playerCameraPosition = playerView.vOrigin;
  playerCameraPosition.z += 8.0f;
  Vector3 targetRelativePosition = {targetPosition.x - playerCameraPosition.x,
                                    targetPosition.y - playerCameraPosition.y,
                                    targetPosition.z - playerCameraPosition.z};
  float relativeDepth = (targetRelativePosition.x * playerView.vForward.x) +
                        (targetRelativePosition.y * playerView.vForward.y) +
                        (targetRelativePosition.z * playerView.vForward.z);
  float relativeWidth = (targetRelativePosition.x * playerView.vRight.x) +
                        (targetRelativePosition.y * playerView.vRight.y) +
                        (targetRelativePosition.z * playerView.vRight.z);
  float relativeHeight = (targetRelativePosition.x * playerView.vUpward.x) +
                         (targetRelativePosition.y * playerView.vUpward.y) +
                         (targetRelativePosition.z * playerView.vUpward.z);

  if (relativeDepth < 0.01f) return Vector2{0, 0};

  float screenX = (screenWidth / 2.f) + (screenWidth / 2.f) / relativeDepth *
                                            fovScale1 * relativeWidth;
  float screenY = (screenHeight / 2.f) - (screenHeight / 2.f) / relativeDepth *
                                             fovScale2 * relativeHeight;

  return Vector2{screenX, screenY};
}

Team getTeamFromModel(const std::string& modelName) {
  if (t_models.count(modelName)) return Team::T;
  if (ct_models.count(modelName)) return Team::CT;
  return Team::UNKNOWN;
}
