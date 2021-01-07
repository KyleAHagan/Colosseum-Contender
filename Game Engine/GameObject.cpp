#include "GameObject.h"

#include "Components/Component.h"
#include "Components/LeftRight.h"
#include "Components/Sprite.h"
#include "Components/Transform.h"
#include "Components/UIBar.h"
#include "Components/Controller.h"
#include "Components/UpDown.h"
#include "Components/Body.h"
#include "Components/Stats.h"
#include "Components/PlayerBehavior.h"
#include "Components/BatBehavior.h"
#include "Components/GoblinMeleeBehavior.h"
#include "Components/GoblinRangedBehavior.h"
#include "Components/OrcBehavior.h"
#include "Components/RatBehavior.h"
#include "Components/SlimeBehavior.h"
#include "Components/SpiderBehavior.h"
#include "Components/TrollBehavior.h"
#include "Components/AttackZoneBehavior.h"
#include "Components/ArrowBehavior.h"
#include "Components/MenuBehavior.h"

#include <iostream>


GameObject::GameObject()
{

}

GameObject::~GameObject()
{
	for (auto iterator = components.begin(); iterator != components.end(); iterator++)
	{
		delete iterator->second;
	}
	components.clear();
}

void GameObject::Update()
{
	for (auto iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if (iterator->second == nullptr)
		{
			//std::wcout << this->objectType << L" " << iterator->first << L" Doesn't exist!"; //Debug
		}
		else
			iterator->second->Update();
	}
}

Component* GameObject::AddComponent(std::wstring type)
{
	Component* pNewComponent = nullptr;
	pNewComponent = GetComponent(type);
	if (nullptr == pNewComponent)
	{
		//If component doesn't already exist, create it.
		if (type == L"Controller")
			pNewComponent = new Controller();
		else if (type == L"Sprite")
			pNewComponent = new Sprite();
		else if (type == L"CharacterAnimations")
			pNewComponent = new CharacterAnimations();
		else if (type == L"Transform")
			pNewComponent = new Transform();
		else if (type == L"UIBar")
			pNewComponent = new UIBar();
		else if (type == L"MenuBehavior")
			pNewComponent = new MenuBehavior();
		else if (type == L"UpDown")
			pNewComponent = new UpDown();
		else if (type == L"Body")
			pNewComponent = new Body();
		else if (type == L"Stats")
			pNewComponent = new Stats();
		else if (type == L"ArrowBehavior")
			pNewComponent = new ArrowBehavior();
		else if (type == L"PlayerBehavior")
			pNewComponent = new PlayerBehavior();
		else if (type == L"AttackZoneBehavior")
			pNewComponent = new AttackZoneBehavior();
		else if (type == L"BatBehavior")
			pNewComponent = new BatBehavior();
		else if (type == L"GoblinMeleeBehavior")
			pNewComponent = new GoblinMeleeBehavior();
		else if (type == L"GoblinRangedBehavior")
			pNewComponent = new GoblinRangedBehavior();
		else if (type == L"OrcBehavior")
			pNewComponent = new OrcBehavior();
		else if (type == L"RatBehavior")
			pNewComponent = new RatBehavior();
		else if (type == L"SlimeBehavior")
			pNewComponent = new SlimeBehavior();
		else if (type == L"SpiderBehavior")
			pNewComponent = new SpiderBehavior();
		else if (type == L"TrollBehavior")
			pNewComponent = new TrollBehavior();
		//else
			//std::cout << "No component named: " << pNewComponent << std::endl; //debug
	}
	if (nullptr != pNewComponent)
	{
		components[type] = pNewComponent;
		pNewComponent->mpOwner = this;
	}

	return pNewComponent;
}

Component* GameObject::GetComponent(std::wstring Type)
{
	if (components.find(Type) == components.end())
		return nullptr;
	else
		return components[Type];
}


void GameObject::HandleEvent(Event* pEvent)
{
	for (auto iterator = components.begin(); iterator != components.end(); iterator++)
	{
		if (iterator->second == nullptr)
			std::wcout << this->objectType << L" " << iterator->first << L" Doesn't exist!";
		else
			iterator->second->HandleEvent(pEvent);
	}
}
