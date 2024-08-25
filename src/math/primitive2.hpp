#pragma once

#include <limits>
#include <utility>
#include <vector>

#include "global_alias.hpp"
#include "xev_math.hpp"

namespace xev {

class Primitive2{
public:
  virtual ~Primitive2() = default;
  virtual void Draw() = 0;
};

class Ray {
public:
  vec2 dir{1.f, 0.f};
  vec2 ori{0.f};

  Ray(const vec2 dir, const vec2 ori) : dir(dir), ori(ori) {}
  Ray() = default;
};

class Box {
public:
  Box(const vec2 center, const float width, const float height)
      : min(center - vec2(width / 2, height / 2)),
        max(center + vec2(width / 2, height / 2)) {}
  Box(vec2 min, vec2 max) : min(min), max(max) {}
  Box() : min(0.f), max(1.f) {}

  std::vector<vec2> getVertices() const {
    return {
        vec2{min.x, min.y}, vec2{max.x, min.y}, vec2{max.x, max.y}, vec2{min.x, max.y}};
  }

private:
  vec2 min{};
  vec2 max{};
};

class Circle {
public:
  Circle(const vec2 center, const float radius) : center(center), radius(radius) {}
  Circle() : center(0.f), radius(0.f) {}
  ~Circle() = default;

private:
  vec2 center{};
  float radius{};
};

class Line { // In 3d this is a plane
public:
  Line() = default;
  Line(const vec2 point, const vec2 normal) : point{point}, normal(normal) {}
  Line(const vec2 point, const vec2 normal, const float_t length)
      : point(point), normal(normal), length(length) {}

  void setNormal(vec2 nrm) { normal = normalize(nrm); }
  void setLength(float_t len) { length = len; }
  vec2 getNormal() { return normal; }
  void move(auto offset) { point += offset; }
  void rotate(float_t theta) { normal = XeMath::rotaion2(theta) * normal; }

  std::pair<vec2, vec2> getEndpoints() {
    vec2 tan_len_half = vec2(normal.y, -normal.x) * length * 0.5f;
    return {point - tan_len_half, point + tan_len_half};
  }

private:
  vec2 point{0.f};
  vec2 normal{0.f, 1.f};
  float_t length{
      std::numeric_limits<float_t>::max()}; // sign don't matter, inf line by default
};

class Polygon {
  std::vector<vec2> vertices;
  std::vector<uint32_t> indices; // optional
};

} // namespace xev
