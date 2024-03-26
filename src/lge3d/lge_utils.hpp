#pragma once

#include <functional>
#include <glm/glm.hpp>
#include <vector>

#include "lge_model.hpp"

namespace lge {

// from: https://stackoverflow.com/a/57595105
    template<typename T, typename... Rest>
    void hashCombine(std::size_t &seed, const T &v, const Rest &... rest) {
      std::hash<T> hasher;
      seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      (hashCombine(seed, rest), ...);
    }

} // namespace lge
