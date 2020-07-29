#include "DropAnimation.h"

#include "FigureBase.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	DropAnimation::DropAnimation(std::list<std::shared_ptr<FigureBase>> figures, float moveDelta)
		: AnimationBase(AnimationType::Drop)
		, m_targets(figures)
		, m_moveDelta(moveDelta)
	{
		auto beginIt = m_targets.begin();
		m_targetPosY = (*beginIt)->sprite()->getPosition().y;
	}

	DropAnimation::~DropAnimation()
	{

	}

	bool DropAnimation::animate()
	{
		bool isFinished = false;

		for (auto target : m_targets) {
			auto sprite = target->sprite();

			float x = sprite->getPosition().x;
			float y = sprite->getPosition().y + MOVE_ANIMATION_SPEED;

			if (y > m_targetPosY) {
				y = m_targetPosY;
				isFinished = true;
			}

			sprite->setPosition(x, y);
		}

		return isFinished;
	}

	std::list<std::shared_ptr<FigureBase>> DropAnimation::getTargets()
	{
		return m_targets;
	}
}