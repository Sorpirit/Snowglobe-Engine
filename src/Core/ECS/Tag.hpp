#pragma once

#include <assert.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Snowglobe::Core::ECS
{

struct Tag
{
public:
    Tag(const uint32_t id) : _id(id) {}
    
    uint32_t GetId() const { return _id; }
    std::string GetName() const;
    
    bool operator==(const Tag& other) const { return _id == other._id; }
    bool operator!=(const Tag& other) const { return _id != other._id; }

private:
    uint32_t _id;
};
    
class TagManager
{
public:
    static TagManager& GetInstance()
    {
        static TagManager instance;
        return instance;
    }
    
    Tag CreateTag(const std::string& name)
    {
        assert(!_tagsSet.contains(name) && "Tag already exists");
        const uint32_t id = _tagsSet.size();
        _tagsSet.insert(name);
        _tags[id] = name;
        return {id};
    }

    std::string GetName(Tag tag) const
    {
        auto it = _tags.find(tag.GetId());
        assert(it != _tags.end() && "Tag does not exist");
        return it->second;
    }

    std::unordered_map<uint32_t, std::string>& GetAllTags() { return _tags; }

private:
    TagManager() = default;
    
    std::unordered_set<std::string> _tagsSet;
    std::unordered_map<uint32_t, std::string> _tags;
};

inline std::string Tag::GetName() const
{
    return TagManager::GetInstance().GetName(*this);
}

}

template <>
struct std::hash<Snowglobe::Core::ECS::Tag>
{
    std::size_t operator()(const Snowglobe::Core::ECS::Tag& tag) const noexcept { return std::hash<uint32_t>()(tag.GetId()); }
};

#define REGISTER_TAG(name) namespace Snowglobe::Tags { inline Core::ECS::Tag name() { static Core::ECS::Tag name = Core::ECS::TagManager::GetInstance().CreateTag(#name); return name; }}
REGISTER_TAG(Default)

