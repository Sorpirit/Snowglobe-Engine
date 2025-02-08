#include "ComponentEditorSystem.hpp"
#include "Engine.hpp"

#include <string>

namespace Snowglobe::Engine
{
	void ComponentEditorSystem::DrawSystemUI()
	{
		bool isActive = true;

		std::string buttonText = _allSystemsPuased ? "Unpause" : "Pause";
		if (_uiSystem->Button(buttonText))
		{
			PuaseAllSystems(!_allSystemsPuased);
		}

		for (auto& systemPair : _engine->GetAllSystems())
		{
			//skip "class Snowglobe::"
			const int skipChars = 17;
			if (_uiSystem->BeginTreeNode((systemPair.first.name() + skipChars)))
			{
				if (!systemPair.second->IsPermanent())
				{
					isActive = systemPair.second->IsActive();
					if (_uiSystem->Checkbox("IsActive", &isActive))
					{
						systemPair.second->SetActive(isActive);
					}
				}

				systemPair.second->DrawDebugUI();

				_uiSystem->EndTreeNode();
			}
		}
	}

	void ComponentEditorSystem::DrawSceneUI()
	{
		if (_uiSystem->BeginTreeNode("All Entites"))
		{
			for (auto entity : _entityManager->GetAllEntities())
			{
				std::string delLable = "Del##" + std::to_string(entity->GetId());
				if (_uiSystem->Button(delLable))
				{
					entity->Destroy();
				}
				_uiSystem->SameLine();
				_uiSystem->Text(entity->GetName());
				_uiSystem->SameLine();
				_uiSystem->Text(entity->GetTag().GetName());
			}
			_uiSystem->EndTreeNode();
		}

		if (_uiSystem->BeginTreeNode("Taged Entites"))
		{
			for (auto& tagPair : Core::ECS::TagManager::GetInstance().GetAllTags())
			{
				if (!_uiSystem->BeginTreeNode(tagPair.second))
					continue;

				for (auto entity : _entityManager->GetEntitiesWithTag(Core::ECS::Tag(tagPair.first)))
				{
					std::string delLable = "Del##" + entity->GetId();
					if (_uiSystem->Button(delLable))
					{
						entity->Destroy();
					}
					_uiSystem->SameLine();
					_uiSystem->Text(entity->GetName());
				}
				_uiSystem->EndTreeNode();
			}
			_uiSystem->EndTreeNode();
		}
	}

	void ComponentEditorSystem::PuaseAllSystems(bool pause)
	{
		_allSystemsPuased = pause;
		for (auto& systemPair : _engine->GetAllSystems())
		{
			if (systemPair.second->IsPermanent())
				continue;

			systemPair.second->SetActive(!pause);
		}
	}
}