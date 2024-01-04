#pragma once

#include "lge_model.hpp"

// std
#include <memory>

namespace lge {
    struct Transform2dComponent{
        glm::vec2 translation{};
        glm::vec2 scale{1.0f, 1.0f};
        float rotation{};

        glm::mat2 mat2() {
          const float c = glm::cos(rotation);
          const float s = glm::sin(rotation);
          glm::mat2 rotMat{{c, s}, {-s, c}};
          glm::mat2 scaleMat{scale.x, 0.0f, 0.0f, scale.y};
          return rotMat * scaleMat;
        }
    };

    class LgeGameObject {
    public:
        using id_t = unsigned int;

        static LgeGameObject createGameObject() {
            static id_t currentId = 0;
            return LgeGameObject{currentId++};
        }

        LgeGameObject(const LgeGameObject &) = delete;
        LgeGameObject &operator=(const LgeGameObject &) = delete;
        LgeGameObject(LgeGameObject &&) = default;
        LgeGameObject &operator=(LgeGameObject &&) = default;

//        id_t getId() const { return id; }

        const id_t id{};
        std::shared_ptr<LgeModel> model{};
        glm::vec3 color{};
        Transform2dComponent transform2d{};

    private:
        explicit LgeGameObject(id_t objId) : id{objId} {}

    };
}