#pragma once

#include <cmath>
#include <glm/glm.hpp>

namespace Snowglobe::Render
{
    struct RenderID
    {
        const uint32_t id;

        RenderID operator=(const RenderID& otherId) {
            return RenderID{ otherId.id };
        }

        bool operator==(const RenderID& otherId) const
        {
            if (this->id == otherId.id) return true;
            else return false;
        }

        struct HashFunction
        {
            size_t operator()(const RenderID& id) const
            {
                return std::hash<uint32_t>()(id.id);
            }
        };
    };

    static const RenderID InvalidID{ 0 };

    class RenderEntity
    {
        public:
            RenderEntity(const RenderID& id) : _id(id) {}

            const RenderID& GetID() const { return _id; }

        private:
            RenderID _id;

        public:
            bool operator==(const RenderEntity& otherEntity) const
            {
                if (this->_id == otherEntity._id) return true;
                else return false;
            }
    };
    
    class RenderProxy
    {
    public:
        RenderProxy(const RenderID& id) : _id(id) {}

        const RenderID& GetID() const { return _id; }

        virtual void SetPosition(const glm::vec3& position) = 0;
        virtual void SetRotation(const glm::vec3& rotation) = 0;
        virtual void SetScale(const glm::vec3& scale) = 0;
        
    private:
        RenderID _id;

    };
} // namespace Snowglobe::Render

template<> struct std::hash<Snowglobe::Render::RenderID> {
    std::size_t operator()(Snowglobe::Render::RenderID const& id) const noexcept {
        return std::hash<uint32_t>{}(id.id);
    }
};

template<> struct std::hash<Snowglobe::Render::RenderEntity> {
    std::size_t operator()(Snowglobe::Render::RenderEntity const& entity) const noexcept {
        return std::hash<Snowglobe::Render::RenderID>{}(entity.GetID());
    }
};