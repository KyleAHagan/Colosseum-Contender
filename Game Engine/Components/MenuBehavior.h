#pragma once
#include <vector>
#include "Component.h"
#include "..\EventManager.h"

class GameObject;

class MenuBehavior : public Component
{
public:
	MenuBehavior();
	~MenuBehavior();

	void Update(void);
	void Serialize(std::wfstream& InputStream);

	void MenuBehavior::HandleEvent(Event* pEvent);

public:

private:

private:
};