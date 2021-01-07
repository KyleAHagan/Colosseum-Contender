#pragma once
#include <vector>
#include <string>
#include <map>

class Component;
class Event;

class GameObject
{
public:
	GameObject();
	~GameObject();
	void Update();
	Component* AddComponent(std::wstring Type);
	Component* GetComponent(std::wstring Type);
	void HandleEvent(Event* pEvent);
public:
	std::wstring objectType;
	std::wstring objectGroup;
	std::map<std::wstring, Component*> components;
private:
private:

};