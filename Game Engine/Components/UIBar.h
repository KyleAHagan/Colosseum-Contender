#pragma once
#include "..\EventManager.h"
#include "Component.h"
#include "Stats.h"
#include "Transform.h"
#include "Sprite.h"
#include <vector>
#include "..\GraphicsManager.h"

class GameObject;

extern Transform* playerTransform;


class UIBar : public Component
{
public:
	UIBar();
	~UIBar();

	void Initialize();

	void Update();

	virtual void Serialize(std::wfstream& InputStream);

	void HandleEvent(Event* pEvent);

public:

	GameObject* player;
	Stats* playerStats;
	int statToTrack = 1;
	

private:
private:
};