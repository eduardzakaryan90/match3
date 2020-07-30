#include "SwipeAnimation.h"

#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	SwipeAnimation::SwipeAnimation(std::shared_ptr<FigureBase> first,
		std::shared_ptr<FigureBase> second, SwipeDirection direction, bool isReverseAnim)
		: AnimationBase(AnimationType::Swipe)
		, m_figure1(first)
		, m_figure2(second)
		, m_direction(direction)
		, m_isReverseAnim(isReverseAnim)
	{
		auto sprite1 = m_figure1->sprite();
		auto sprite2 = m_figure2->sprite();

		auto bounds1 = sprite1->getGlobalBounds();
		auto bounds2 = sprite2->getGlobalBounds();

		m_targetPos2.x = sprite1->getPosition().x + bounds1.width / 2 - bounds2.width / 2;
		m_targetPos1.x = sprite2->getPosition().x + bounds2.width / 2 - bounds1.width / 2;
		m_targetPos2.y = sprite1->getPosition().y + bounds1.height / 2 - bounds2.height / 2;
		m_targetPos1.y = sprite2->getPosition().y + bounds2.height / 2 - bounds1.height / 2;
	}

	SwipeAnimation::~SwipeAnimation()
	{

	}

	bool SwipeAnimation::animate()
	{
		bool isFinished = false;

		auto sprite1 = m_figure1->sprite();
		auto sprite2 = m_figure2->sprite();

		float x1 = sprite1->getPosition().x;
		float y1 = sprite1->getPosition().y;
		float x2 = sprite2->getPosition().x;
		float y2 = sprite2->getPosition().y;

		switch (m_direction)
		{
		case SwipeDirection::Up:
			y1 -= MOVE_ANIMATION_SPEED;
			y2 += MOVE_ANIMATION_SPEED;
			if (y1 < m_targetPos1.y) {
				y1 = m_targetPos1.y;
				y2 = m_targetPos2.y;
				isFinished = true;
			}
			break;
		case SwipeDirection::Down:
			y1 += MOVE_ANIMATION_SPEED;
			y2 -= MOVE_ANIMATION_SPEED;
			if (y1 > m_targetPos1.y) {
				y1 = m_targetPos1.y;
				y2 = m_targetPos2.y;
				isFinished = true;
			}
			break;
		case SwipeDirection::Right:
			x1 += MOVE_ANIMATION_SPEED;
			x2 -= MOVE_ANIMATION_SPEED;
			if (x1 > m_targetPos1.x) {
				x1 = m_targetPos1.x;
				x2 = m_targetPos2.x;
				isFinished = true;
			}
			break;
		case SwipeDirection::Left:
			x1 -= MOVE_ANIMATION_SPEED;
			x2 += MOVE_ANIMATION_SPEED;
			if (x1 < m_targetPos1.x) {
				x1 = m_targetPos1.x;
				x2 = m_targetPos2.x;
				isFinished = true;
			}
			break;
		}

		sprite1->setPosition(x1, y1);
		sprite2->setPosition(x2, y2);

		return isFinished;
	}

	std::shared_ptr<FigureBase> SwipeAnimation::getFigure1()
	{
		return m_figure1;
	}

	std::shared_ptr<FigureBase> SwipeAnimation::getFigure2()
	{
		return m_figure2;
	}

	SwipeDirection SwipeAnimation::getDirection()
	{
		return m_direction;
	}

	bool SwipeAnimation::isReverseAnim()
	{
		return m_isReverseAnim;
	}
}