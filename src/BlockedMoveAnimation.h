#pragma once

#include "AnimationBase.h"
#include "FigureBase.h"

#include <memory>

namespace match3
{
	class BlockedMoveAnimation : public AnimationBase
	{
	private:
		const float BLOCKED_MOVE_DELTA = 10.0f;

	public:
		BlockedMoveAnimation(std::shared_ptr<FigureBase> target, SwipeDirection direction);
		virtual ~BlockedMoveAnimation();

		virtual bool animate() override;

	private:
		std::shared_ptr<FigureBase> m_figure;
		sf::Vector2f m_originPos;
		sf::Vector2f m_targetPos;

		SwipeDirection m_direction;
		bool m_reversive;
	};
}