#include "BlockedMoveAnimation.h"

#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	BlockedMoveAnimation::BlockedMoveAnimation(std::shared_ptr<FigureBase> target, SwipeDirection direction)
		: AnimationBase(AnimationType::Swipe)
		, m_figure(target)
		, m_direction(direction)
		, m_reversive(true)
	{
		m_originPos = m_figure->sprite()->getPosition();
		m_targetPos = m_originPos;
		switch (direction)
		{
		case match3::Up:
			m_targetPos.y -= BLOCKED_MOVE_DELTA;
			break;
		case match3::Down:
			m_targetPos.y += BLOCKED_MOVE_DELTA;
			break;
		case match3::Right:
			m_targetPos.x += BLOCKED_MOVE_DELTA;
			break;
		case match3::Left:
			m_targetPos.x -= BLOCKED_MOVE_DELTA;
			break;
		}
	}

	BlockedMoveAnimation::~BlockedMoveAnimation()
	{

	}

	bool BlockedMoveAnimation::animate()
	{
		bool isFinished = false;

		auto sprite = m_figure->sprite();

		float x = sprite->getPosition().x;
		float y = sprite->getPosition().y;

		switch (m_direction)
		{
		case SwipeDirection::Up:
			y -= MOVE_ANIMATION_SPEED;
			if (y < m_targetPos.y) {
				y = m_targetPos.y;
				isFinished = true;
			}
			break;
		case SwipeDirection::Down:
			y += MOVE_ANIMATION_SPEED;
			if (y > m_targetPos.y) {
				y = m_targetPos.y;
				isFinished = true;
			}
			break;
		case SwipeDirection::Right:
			x += MOVE_ANIMATION_SPEED;
			if (x > m_targetPos.x) {
				x = m_targetPos.x;
				isFinished = true;
			}
			break;
		case SwipeDirection::Left:
			x -= MOVE_ANIMATION_SPEED;
			if (x < m_targetPos.x) {
				x = m_targetPos.x;
				isFinished = true;
			}
			break;
		}

		if (isFinished && m_reversive) {
			m_reversive = false;
			isFinished = false;

			m_targetPos = m_originPos;

			switch (m_direction)
			{
			case SwipeDirection::Up:
				m_direction = SwipeDirection::Down;
				break;
			case SwipeDirection::Down:
				m_direction = SwipeDirection::Up;
				break;
			case SwipeDirection::Right:
				m_direction = SwipeDirection::Left;
				break;
			case SwipeDirection::Left:
				m_direction = SwipeDirection::Right;
				break;
			}
		}

		sprite->setPosition(x, y);

		return isFinished;
	}
}