#pragma once
#include <vector>
#include "Component.h"
#include "..\\EventManager.h"

class GameObject;


static enum frame
{ 
	MoveUp1, MoveUp2, MoveUp3, MoveUp4,
	MoveRight1, MoveRight2, MoveRight3, MoveRight4,
	MoveDown1, MoveDown2, MoveDown3, MoveDown4,
	MoveLeft1, MoveLeft2, MoveLeft3, MoveLeft4,

	AttackUp1, AttackUp2, AttackUp3, AttackUp4, 
	AttackRight1, AttackRight2, AttackRight3, AttackRight4,
	AttackDown1, AttackDown2, AttackDown3, AttackDown4,
	AttackLeft1, AttackLeft2, AttackLeft3, AttackLeft4,

	StruckFromUp1, StruckFromUp2, StruckFromUp3, StruckFromUp4,
	StruckFromRight1, StruckFromRight2, StruckFromRight3, StruckFromRight4,
	StruckFromDown1, StruckFromDown2, StruckFromDown3, StruckFromDown4,
	StruckFromLeft1, StruckFromLeft2, StruckFromLeft3, StruckFromLeft4,

	IdleDown1, IdleDown2, IdleDown3, IdleDown4, //Idle facing down.

	SpecialDown1, SpecialDown2, SpecialDown3, SpecialDown4, //generic action facing down.

	StillUp1,
	StillRight1,
	StillDown1, 
	StillLeft1,
};

static enum animation
{
	MoveUp = MoveUp1, MoveRight = MoveRight1, MoveDown = MoveDown1, MoveLeft = MoveLeft1, AttackUp = AttackUp1, AttackRight = AttackRight1, AttackDown = AttackDown1, AttackLeft = AttackLeft1, StruckFromUp = StruckFromUp1, StruckFromRight = StruckFromRight1, StruckFromDown = StruckFromDown1, StruckFromLeft = StruckFromLeft1, IdleDown = IdleDown1, SpecialDown = SpecialDown1, StillUp = StillUp1, StillRight = StillRight1, StillDown = StillDown1, StillLeft = StillLeft1
};

static enum action
{
	Move = MoveUp, Attack = AttackUp, Struck = StruckFromUp, Still = StillUp, Idle = IdleDown, Special = SpecialDown //DEBUG idle and action don't have 4 directions, so don't do any multiplication to get to frames from Specials.
};

static enum direction {Up, Right, Down, Left, Omnidirectional};

static int animationLength_MoveAttackStruck = 4;
static int animationLength_Still = 1;

static struct behavior
{
	action currentAction;
	animation currentAnimation;
	direction facing;
	float duration;
	frame nextBehaviorNumber;
	int textureRow;
	int textureColumn;

};

/* 4x11 spritesheet percentages.
0 ---- - 0.25 ---- - 0.5 ---- - 0.75 ---- - 1.0
|
0.0909090
|
0.1818182
|
0.2727273
|
0.3636364
|
0.4545455
|
0.5454545
|
0.6363636
|
0.7272727
|
0.8181818
|
0.9090909
|
1.0
*/

static behavior frameList[60]{
	{Move, MoveUp, Up,	100, MoveUp2, 5, 1}, {Move, MoveUp, Up,	100, MoveUp3, 5, 2}, {Move, MoveUp, Up, 100, MoveUp4, 5, 3}, {Move, MoveUp, Up, 100, StillUp1, 5, 4},
	{Move, MoveRight, Right, 100, MoveRight2, 4, 1}, {Move, MoveRight, Right, 100, MoveRight3, 4, 2}, {Move, MoveRight, Right, 100, MoveRight4, 4, 3}, {Move, MoveRight, Right, 100, StillRight1, 4, 4},
	{Move, MoveDown, Down,	100, MoveDown2, 3, 1}, {Move, MoveDown, Down,	100, MoveDown3,	3, 2}, {Move, MoveDown, Down, 100, MoveDown4, 3, 3}, {Move, MoveDown, Down, 100, StillDown1, 3, 4},
	{Move, MoveLeft, Left, 100, MoveLeft2, 4, 1}, {Move, MoveLeft, Left, 100, MoveLeft3, 4, 2}, {Move, MoveLeft, Left, 100, MoveLeft4, 4, 3}, {Move, MoveLeft, Left, 100, StillLeft1, 4, 4},

	{Attack, AttackUp, Up,	100, AttackUp2,	8, 1}, {Attack, AttackUp, Up,	100, AttackUp3, 8, 2}, {Attack, AttackUp, Up, 100, AttackUp4, 8, 3}, {Attack, AttackUp, Up, 100, StillUp1, 8, 4},
	{Attack, AttackRight, Right, 100, AttackRight2, 7, 1}, {Attack, AttackRight, Right, 100, AttackRight3, 7, 2}, {Attack, AttackRight, Right, 100, AttackRight4, 7, 3}, {Attack, AttackRight, Right, 100, StillRight1,  7, 4},
	{Attack, AttackDown, Down,	100, AttackDown2, 6, 1}, {Attack, AttackDown, Down,	100, AttackDown3, 6, 2}, {Attack, AttackDown, Down, 100, AttackDown4, 6, 3}, {Attack, AttackDown, Down, 100, StillDown1, 6, 4},
	{Attack, AttackLeft, Left, 100, AttackLeft2, 7, 1}, {Attack, AttackLeft, Left, 100, AttackLeft3, 7, 2}, {Attack, AttackLeft, Left, 100, AttackLeft4, 7, 3}, {Attack, AttackLeft, Left, 100, StillLeft1,  7, 4},

	{Struck, StruckFromUp, Up,	120, StruckFromUp2,	11, 1},		{Struck, StruckFromUp, Up,	80, StruckFromUp3, 11, 2},		{Struck, StruckFromUp, Up, 100, StruckFromUp4, 11, 3},		{Struck, StruckFromUp, Up, 100, StillUp1, 11, 1},
	{Struck, StruckFromRight, Right, 120, StruckFromRight2,  10, 1},	{Struck, StruckFromRight, Right, 80, StruckFromRight3,	10, 2},	{Struck, StruckFromRight, Right, 80, StruckFromRight4, 10, 3}, {Struck, StruckFromRight, Right, 80, StillRight1, 10, 1},
	{Struck, StruckFromDown, Down,	120, StruckFromDown2,	9, 1},	{Struck, StruckFromDown, Down,	80, StruckFromDown3, 9, 2},	{Struck, StruckFromDown, Down, 80, StruckFromDown4,9, 3},	{Struck, StruckFromDown, Down, 80, StillDown1, 9, 1},
	{Struck, StruckFromLeft, Left, 120, StruckFromLeft2,  10, 1},	{Struck, StruckFromLeft, Left, 80, StruckFromLeft3,	10, 2},	{Struck, StruckFromLeft, Left, 80, StruckFromLeft4, 10, 3}, {Struck, StruckFromLeft, Left, 80, StillLeft1, 10, 1},


	{Idle, IdleDown, Down,	640, IdleDown2, 1, 1},	{Idle, IdleDown, Down, 80, IdleDown3, 1, 2}, {Idle, IdleDown, Down, 640, IdleDown4, 1, 3}, {Idle, IdleDown, Down, 80, IdleDown1, 1, 2},
	
	{Special, SpecialDown, Down, 640, SpecialDown2, 2, 1},	{Special, SpecialDown, Down, 80, SpecialDown3,  2, 2}, {Special, SpecialDown, Down, 640, SpecialDown4, 2, 3}, {Special, SpecialDown, Down, 80, IdleDown1, 2, 2},

	{Still, StillUp, Up,	5000, IdleDown1, 11, 4},
	{Still, StillRight, Right, 5000, IdleDown1,	10, 4},
	{Still, StillDown, Down,	5000, IdleDown1, 9, 4},
	{Still, StillLeft, Left,	5000, IdleDown1, 10, 4}
};


class CharacterAnimations : public Component
{
public:
	CharacterAnimations();
	~CharacterAnimations();

	void Update(void);
	void Serialize(std::wfstream& InputStream);

	void SetAction(action newAction);//Animation uses its current facing information
	void SetAnimation(animation newAnimation);
	void SetFrame(frame newFrame);

	void OnFrameEffects(int newFrame);

public:
	int currentFrame;
	float timeInCurrentFrame;

private:

private:

};

class AnimationFrameReachedEvent : public Event
{
public:
	AnimationFrameReachedEvent();
	~AnimationFrameReachedEvent();
public:
	int frameReached;
};