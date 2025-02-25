#pragma once
#include "SerializationAPI.hpp"

namespace Snowglobe::Core::Serialization
{

class Serializer : public SerializationAPI
{
public:
    void Serialize(auto* valRef) { Property(*valRef); }
    void Serialize(auto& valRef) { Property(valRef); }
};

} // namespace Snowglobe::Core::Serialization