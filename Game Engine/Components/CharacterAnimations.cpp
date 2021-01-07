#include "CharacterAnimations.h"
#include "Component.h"
#include "Sprite.h"
#include "..\GameObject.h"
#include "Transform.h"
#include "..\GameObject.h"
#include "..\Frame Rate Controller.h"
#include "..\\EventManager.h"


#include "..\\ResourceManager.h"

extern std::string resourcesPathSprites;
extern ResourceManager* gpResourceManager;

extern Frame_Rate_Controller* gFrameRateController;

AnimationFrameReachedEvent::AnimationFrameReachedEvent() : Event(EventType::ANIMATION_FRAME_REACHED)
{
	frameReached = IdleDown1;
}

AnimationFrameReachedEvent::~AnimationFrameReachedEvent()
{

}


CharacterAnimations::CharacterAnimations() : Component(L"CharacterAnimations")
{
	currentFrame = IdleDown1;
	timeInCurrentFrame = 0;
}

CharacterAnimations::~CharacterAnimations()
{

}


void CharacterAnimations::Update(void)
{
	timeInCurrentFrame += gFrameRateController->GetFrameTime();

	if (timeInCurrentFrame > frameList[currentFrame].duration)
	{
		SetFrame(frameList[currentFrame].nextBehaviorNumber);
	}

}

void CharacterAnimations::SetAction(action newAction)//Animation using the current facing information
{
	int directionOffset = 0;
	if (newAction == Move || newAction == Attack || newAction == Struck)
	{
		directionOffset = frameList[currentFrame].facing * animationLength_MoveAttackStruck;
	}
	if (newAction == Still)
	{
		directionOffset = frameList[currentFrame].facing * animationLength_Still;
	}

	currentFrame = newAction + directionOffset;
	OnFrameEffects(currentFrame);
	timeInCurrentFrame = 0;
	Sprite* sprite = static_cast<Sprite*>(mpOwner->GetComponent(L"Sprite"));
	if (nullptr != sprite)
	{
		sprite->Update(frameList[currentFrame].textureRow, frameList[currentFrame].textureColumn, frameList[currentFrame].facing == Left);
	}

}

void CharacterAnimations::SetAnimation(animation newAnimation)
{
	currentFrame = newAnimation;
	OnFrameEffects(currentFrame);
	timeInCurrentFrame = 0;
	Sprite* sprite = static_cast<Sprite*>(mpOwner->GetComponent(L"Sprite"));
	if (nullptr != sprite)
	{
		sprite->Update(frameList[currentFrame].textureRow, frameList[currentFrame].textureColumn, frameList[currentFrame].facing == Left);
	}
}


void CharacterAnimations::SetFrame(frame newFrame)
{
	currentFrame = newFrame;
	OnFrameEffects(newFrame);
	timeInCurrentFrame = 0;
	Sprite* sprite = static_cast<Sprite*>(mpOwner->GetComponent(L"Sprite"));
	if (nullptr != sprite)
	{
		sprite->Update(frameList[currentFrame].textureRow, frameList[currentFrame].textureColumn, frameList[currentFrame].facing == Left);
	}
}


void CharacterAnimations::OnFrameEffects(int newFrame)
{
	if (newFrame == AttackUp2 || newFrame == AttackRight2 || newFrame == AttackDown2 || newFrame == AttackLeft2 || newFrame == SpecialDown2)
	{
		AnimationFrameReachedEvent* frameReachedEvent = new AnimationFrameReachedEvent();
		frameReachedEvent->frameReached = newFrame;
		this->mpOwner->HandleEvent(frameReachedEvent);
	}
}


void CharacterAnimations::Serialize(std::wfstream& InputStream)
{


}