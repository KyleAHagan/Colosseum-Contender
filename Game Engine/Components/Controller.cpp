#include "Controller.h"
#include "..\Input Manager.h"
#include "..\EventManager.h"
#include "..\PhysicsManager.h"
#include "..\..\SDL2.0 Lib\include\SDL_scancode.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "Body.h"
#include "Stats.h"

#include <set>

#include <iostream>
#include <fstream>
#include <streambuf>


extern Input_Manager* gInputManager;
extern EventManager* gEventManager;




Controller::Controller() : Component(L"Controller")
{

}

Controller::~Controller()
{

}

void Controller::Serialize(std::wfstream& InputStream)
{
	gEventManager->Subscribe(EventType::MOUSE_CLICK, this->mpOwner);
}





void Controller::Update()
{

	
	Body* pBody = static_cast<Body*>(mpOwner->GetComponent(L"Body"));
	Stats* pStats = static_cast<Stats*>(mpOwner->GetComponent(L"Stats"));
	Transform* pTransform = static_cast<Transform*>(mpOwner->GetComponent(L"Transform"));
	CharacterAnimations* pCharacterAnimations = static_cast<CharacterAnimations*>(mpOwner->GetComponent(L"CharacterAnimations"));

	if (nullptr != pTransform && nullptr != pCharacterAnimations)
	{
		action currentAction = frameList[pCharacterAnimations->currentFrame].currentAction;
		animation currentAnimation = frameList[pCharacterAnimations->currentFrame].currentAnimation;


		//I've cut down the amount of time that the player is animation locked to the bare minimum . They can't even be stunned.
		if (std::set<int>{ AttackUp1, AttackUp2, AttackRight1, AttackRight2, AttackDown1, AttackDown2, AttackLeft1, AttackLeft2 }.count(pCharacterAnimations->currentFrame))
			return;

		if (gInputManager->IsKeyPressed(SDL_SCANCODE_SPACE))
		{
			pCharacterAnimations->SetAction(Attack);
		}
		else if (gInputManager->IsKeyPressed(SDL_SCANCODE_W))
		{
			pTransform->positionY += pStats->movementSpeed;
			if (currentAnimation != MoveUp)
			{
				pCharacterAnimations->SetAnimation(MoveUp);
			}
		}
		else if (gInputManager->IsKeyPressed(SDL_SCANCODE_D))
		{
			pTransform->positionX += pStats->movementSpeed;
			if (currentAnimation != MoveRight)
			{
				pCharacterAnimations->SetAnimation(MoveRight);
			}
		}
		else if (gInputManager->IsKeyPressed(SDL_SCANCODE_S))
		{
			pTransform->positionY -= pStats->movementSpeed;
			if (currentAnimation != MoveDown)
			{
				pCharacterAnimations->SetAnimation(MoveDown);
			}
		}
		else if (gInputManager->IsKeyPressed(SDL_SCANCODE_A))
		{
			pTransform->positionX -= pStats->movementSpeed;
			if (currentAnimation != MoveLeft)
			{
				pCharacterAnimations->SetAnimation(MoveLeft);
			}
		}
	}
}


void Controller::HandleEvent(Event* pEvent)
{


}