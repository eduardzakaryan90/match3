#include "DestroyAnimation.h"

#include "FigureBase.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	DestroyAnimation::DestroyAnimation(std::list<std::shared_ptr<FigureBase>> figures)
		: AnimationBase(AnimationType::Destroy)
		, m_targets(figures)
	{

	}

	DestroyAnimation::~DestroyAnimation()
	{

	}

	bool DestroyAnimation::animate()
	{
		int32_t finishedTargets = 0;

		for (auto target : m_targets) {
			std::shared_ptr<sf::Sprite> sprite(target->sprite());

			sf::FloatRect originBoundRect = sprite->getGlobalBounds();
			sf::Vector2f originScale = sprite->getScale();
			sf::Vector2f originPos = sprite->getPosition();

			float newScaleX = originScale.x - SCALE_ANIMATION_SPEED;
			float newScaleY = originScale.y - SCALE_ANIMATION_SPEED;

			if (newScaleX < 0) {
				newScaleX = 0;
				++finishedTargets;
			}
			else if (newScaleY <= 0) {
				newScaleY = 0;
				++finishedTargets;
			}

			sprite->setScale(newScaleX, newScaleY);
			sf::FloatRect newBoundRect = sprite->getGlobalBounds();

			float deltaX = (originBoundRect.width - newBoundRect.width) / 2;
			float deltaY = (originBoundRect.height - newBoundRect.height) / 2;
			sprite->setPosition(originPos.x + deltaX, originPos.y + deltaY);
		}

		return finishedTargets == m_targets.size();
	}
	std::list<std::shared_ptr<FigureBase>> DestroyAnimation::getTargets()
	{
		return m_targets;
	}
}