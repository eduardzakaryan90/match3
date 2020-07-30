#include "DropAnimation.h"

#include "FigureBase.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	DropAnimation::DropAnimation(std::list<std::pair<std::shared_ptr<FigureBase>, int32_t>> dropingFigures, float moveDelta)
		: AnimationBase(AnimationType::Drop)
		, m_dropingFigures(dropingFigures)
		, m_moveDelta(moveDelta)
	{

	}

	void DropAnimation::setTargetPositions()
	{
		m_targetPositions.resize(m_dropingFigures.size());

		size_t i = 0;
		for (auto& droppigFigure : m_dropingFigures) {
			m_targetPositions[i] = droppigFigure.second * m_moveDelta + droppigFigure.first->sprite()->getPosition().y;
			++i;
		}
	}

	DropAnimation::~DropAnimation()
	{

	}

	bool DropAnimation::animate()
	{
		int32_t finishedAnims = 0;

		size_t i = 0;
		for (auto& droppigFigure : m_dropingFigures) {
			auto& sprite = droppigFigure.first->sprite();

			float x = sprite->getPosition().x;
			float y = sprite->getPosition().y + MOVE_ANIMATION_SPEED;

			if (y > m_targetPositions[i]) {
				y = m_targetPositions[i];
				++finishedAnims;
			}

			sprite->setPosition(x, y);

			++i;
		}

		return finishedAnims == m_dropingFigures.size();
	}

	std::set<std::shared_ptr<FigureBase>> DropAnimation::getTargets()
	{
		std::set<std::shared_ptr<FigureBase>> targets;

		for (auto& pair : m_dropingFigures) {
			targets.insert(pair.first);
		}

		return targets;
	}
}