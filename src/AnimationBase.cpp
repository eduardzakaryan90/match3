#include "AnimationBase.h"

namespace match3
{
	AnimationBase::AnimationBase(AnimationType type)
		: m_type(type)
	{

	}

	AnimationBase::~AnimationBase()
	{

	}

	AnimationType AnimationBase::type()
	{
		return m_type;
	}
}