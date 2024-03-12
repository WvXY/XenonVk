#pragma once

#include <memory>

#include <glm/glm.hpp>

namespace lge {

class ECSManager {
public:
  ECSManager()                             = default;
  ECSManager(const ECSManager&)            = delete;
  ECSManager& operator=(const ECSManager&) = delete;
  ~ECSManager()                            = default;

};

} // namespace lge