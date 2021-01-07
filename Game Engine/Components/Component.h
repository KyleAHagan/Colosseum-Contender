#pragma once
#include <fstream>
#include <string>

class GameObject;
class Event;

class Component
{
public:
	Component(std::wstring Type);
	virtual ~Component() { }
	virtual void Update() = 0;

	std::wstring GetType() { return mType; }

	virtual void Serialize(std::wfstream& InputStream) = 0;
	virtual void HandleEvent(Event* pEvent) { }

public:
	GameObject* mpOwner;
private:

private:
	std::wstring mType;
};