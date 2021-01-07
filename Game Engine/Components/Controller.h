#pragma once
#include "..\EventManager.h"
#include "Component.h"
#include "CharacterAnimations.h"
#include <vector>

class GameObject;



class Controller : public Component
{
public:
	Controller();
	~Controller();



	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:
private:
private:
};