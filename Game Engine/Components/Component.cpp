#include "Component.h"

Component::Component(std::wstring Type)
{
	mpOwner = nullptr;
	mType = Type;
}