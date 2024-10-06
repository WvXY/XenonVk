#pragma once

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "primitive2.hpp"

#include <typeinfo>

#define EPSILON 1e-3

namespace xev {

// distances functions
class Geometry {
public:
  template <typename T> static float_t distPointToPoint(const T& p, const T& q) {
    return glm::length(p - q);
  }

  template <typename T> static float_t
  distPointToPlane(const T& point, const T& planeNormal, const T& planePoint) {
    return distPointTo_(point, planeNormal, planePoint);
  }

  // static float_t distPointToLine();
  template <typename T>
  static bool isPointOnPlane(const T& point, const T& planeNormal, const T& planePoint) {
    assert(
    typeid(point) == typeid(planeNormal) &&
    typeid(planeNormal) == typeid(planePoint));
    return Geometry::distPointTo_(point, planeNormal, planePoint) < EPSILON;
  }

  // area
  static float_t
  triangleSize3(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
    return glm::length(cross(p1 - p0, p2 - p0)) / 2.f;
  }
  static float_t
  triangleArea2(const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2) {
    glm::vec2 v0 = p1 - p0, v1 = p2 - p0;
    return glm::abs(v0.x * v1.y - v0.y * v1.x) / 2.0f;
  }

  static float_t polygonArea2(const std::vector<glm::vec2>& vertices) {
    float_t area = 0;
    for (size_t i = 0; i < vertices.size(); i++) {
      size_t j = (i + 1) % vertices.size();
      area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
    }
    return glm::abs(area) / 2.0f;
  }

  // normals
  static glm::vec3
  getNormal(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2) {
    return normalize(cross(p1 - p0, p2 - p0));
  }

  // intersect
  static bool isIntersectLines(Line line1, Line line2) {}

protected:
  template <typename T>
  static float_t distPointTo_(const T& x, const T& normal, const T& point) {
    return glm::abs(glm::dot(point - x, glm::normalize(normal)));
  }
};
} // namespace xev
