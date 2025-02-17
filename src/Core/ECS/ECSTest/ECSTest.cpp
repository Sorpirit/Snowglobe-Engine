#include "ECSTest.hpp"

#include "ECS/LifetimeSystem.hpp"
#include "ECS/SystemManager.hpp"
#include "SerializationAPI.hpp"
#include "JsonSerialization.hpp"

using namespace Snowglobe::Core::ECS::ECSTest;
using namespace Snowglobe::Core;

struct BasicData {
    int A;
    float B;
    std::string C;
};

struct ComplexData {
    uint32_t D;
    BasicData Data;
};

struct DataStructures {
    std::vector<int> UVec;
    std::vector<ComplexData> ComplexData;
};

template<>
inline void SerializationAPI::Property<BasicData>(BasicData& varRef) {
    Property("A", varRef.A);
    Property("B", varRef.B);
    Property("C", varRef.C);
}

template<>
inline void SerializationAPI::Property<ComplexData>(ComplexData& varRef) {
    Property("D", varRef.D);
    Property("Data", varRef.Data);
}

template<>
inline void SerializationAPI::Property<DataStructures>(DataStructures& varRef) {
    Property("UVec", varRef.UVec);
    Property("ComplexData", varRef.ComplexData);
}

void TestSerializer()
{
    std::string input = R"(
      {
        "UVec": [ 24, 42, 12, 11, 22, 33],
        "ComplexData": [
            {
                "D": 10,
                "Data": {"A": 110, "B": 1110.1 }
            }
        ]
      }
    )";

    nlohmann::json result;
    SAXHandler sax(result);

    bool parse_result = nlohmann::json::sax_parse(input, &sax);
    if (!parse_result)
    {
        std::cerr << "parsing unsuccessful!" << std::endl;
        return;
    }

    DataStructures d;
    JsonReader jreader(result);
    JsonWriter jwriter;
    jreader.Deserialize(d);
    jwriter.Serialize(d);
    std::cout << jwriter.GetResult().dump(4) << std::endl;
}

void TestEntityManager()
{
    auto nativeEntityManager = std::make_shared<ECSTestEntityManager>();
    std::shared_ptr<Snowglobe::Core::ECS::EntityManagerBase> entityManager = nativeEntityManager;
    const Snowglobe::Core::ECS::Lifetime testLifetime(42);

    auto ent1 = entityManager->CreateEntity();
    auto ent2 = entityManager->CreateEntity(Snowglobe::Tags::TestTag());
    auto ent3 = entityManager->CreateEntity(testLifetime, Snowglobe::Tags::TestTag());
    auto ent4 = entityManager->CreateEntity(testLifetime);

    entityManager->Update();

    assert(entityManager->GetAllEntities().size() == 4);
    assert(entityManager->GetEntitiesWithTag(Snowglobe::Tags::TestTag()).size() == 2);
    assert(entityManager->GetEntitiesWithLifetime(testLifetime).size() == 2);
    assert(entityManager->GetEntitiesWithLifetimeAndTag(testLifetime, Snowglobe::Tags::TestTag()).size() == 1);

    const Snowglobe::Core::ECS::Lifetime sceneLifetime(43);
    Snowglobe::Core::ECS::SceneEntityManager sceneManager(entityManager, sceneLifetime);
    auto scnEnt1 = sceneManager.CreateEntity();
    auto scnEnt2 = sceneManager.CreateEntity(Snowglobe::Tags::TestTag());

    entityManager->Update();

    assert(entityManager->GetAllEntities().size() == 6);
    assert(entityManager->GetEntitiesWithTag(Snowglobe::Tags::TestTag()).size() == 3);
    assert(sceneManager.GetAllEntities().size() == 2);
    assert(sceneManager.GetEntitiesWithTag(Snowglobe::Tags::TestTag()).size() == 1);
}

void TestSystemManager()
{
    auto nativeEntityManager = std::make_shared<ECSTestEntityManager>();
    std::shared_ptr<Snowglobe::Core::ECS::EntityManagerBase> entityManager = nativeEntityManager;
    auto systemManager = std::make_shared<Snowglobe::Core::ECS::SystemManager>(entityManager);
    const Snowglobe::Core::ECS::Lifetime testLifetime(42);

    uint32_t counter = 0;

    bool addTestA = systemManager->TryAddSystem<TestSystemA>(Snowglobe::Core::ECS::DefaultLifetime, 0, &counter);
    assert(addTestA && "Unable to add TestSystemA");

    TestSystemA* testAPtr = nullptr;
    bool queryTestA = systemManager->QuerySystem(testAPtr);

    assert(queryTestA && testAPtr != nullptr && "Unable to query TestSystemA");

    TestSystemB* testBPtr = nullptr;
    bool queryTestB = systemManager->QuerySystem(testBPtr);

    assert(!queryTestB && testBPtr == nullptr &&
           "Was able to get TestSystemB that was not added yet");

    bool testBSystem = false;
    testBSystem = systemManager->TryAddSystem<TestSystemB>(testLifetime, 1, &counter);
    assert(testBSystem && "Unable to add TestSystemB");
    testBSystem = systemManager->QuerySystem(testBPtr);
    assert(testBSystem && "Was not able to get TestSystemB");
    testBSystem = systemManager->TryAddSystem<TestSystemB>(testLifetime, 1, &counter);
    assert(!testBSystem && "Was able to add TestSystemB two times");

    systemManager->TryAddSystem([&](auto& entityManager) {
        assert(counter == 2 && "TestSystem Update Order is incorrect");
        counter++;
    }, 2, testLifetime, "TestAnonymousSystem");

    {
        auto systemsWithTestLifetime = systemManager->GetSystemsWithLifetime(testLifetime);
        assert(systemsWithTestLifetime.size() == 2 && (systemsWithTestLifetime[0].get() == testBPtr) &&
               "Unable to get physics system with target lifetime");
    }

    {
        auto orderedSystems = systemManager->GetSortedSystems();
        assert(orderedSystems.size() == 3 && "Unable to get physics system with target lifetime");
    }

    {
        auto typedSystems = systemManager->GetTypedSystems();
        assert(typedSystems.size() == 2 && typedSystems[typeid(TestSystemA)].get() == testAPtr && "Typed system map is wrong");
    }

    systemManager->Update();
    assert(counter == 3 && "Update is not full");

    counter = 0;
    testAPtr->UnregisterSystem();
    assert(testAPtr->NeedsUnregisterSystem());
    systemManager->Update();

    uint32_t systemCount = 0;
    for (auto vec : systemManager->GetSortedSystems())
    {
        systemCount += vec.size();
    }

    assert(systemCount == 2 && "System has not been removed properly");
    assert(systemManager->GetTypedSystems().size() == 1 && "System has not been removed properly");

    TestSystemC* testCPtr = nullptr;
    systemManager->TryAddSystem<TestSystemC>(Snowglobe::Core::ECS::DefaultLifetime);
    systemManager->QuerySystem(testCPtr);

    assert(testBPtr->IsPermanent());
    assert(testCPtr->IsActive());
    testBPtr->SetActive(false);
    testCPtr->SetActive(false);
    assert(testBPtr->IsActive());
    assert(!testCPtr->IsActive());
}

int main()
{
    TestSerializer();
    TestEntityManager();
    TestSystemManager();
}