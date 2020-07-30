#include "SpawnAnimation.h"

#include "FigureBase.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	SpawnAnimation::SpawnAnimation(std::set<std::shared_ptr<FigureBase>> figures)
		: AnimationBase(AnimationType::Spawn)
		, m_targets(figures)
	{
		for (auto& target : m_targets) {
			target->setNeedToSpawn(false);
		}
	}

	SpawnAnimation::~SpawnAnimation()
	{

	}

	bool SpawnAnimation::animate()
	{
		int32_t finishedTargets = 0;

		for (auto& target : m_targets) {
			auto& sprite = target->sprite();

			sf::FloatRect& originBoundRect = sprite->getGlobalBounds();
			sf::Vector2f originScale = sprite->getScale();
			sf::Vector2f originPos = sprite->getPosition();

			float newScaleX = originScale.x + SCALE_ANIMATION_SPEED;
			float newScaleY = originScale.y + SCALE_ANIMATION_SPEED;

			if (newScaleX > TARGET_SCALE.x) {
				newScaleX = TARGET_SCALE.x;
				newScaleY = TARGET_SCALE.y;
				++finishedTargets;
			}

			sprite->setScale(newScaleX, newScaleY);
			sf::FloatRect& newBoundRect = sprite->getGlobalBounds();

			float deltaX = (originBoundRect.width - newBoundRect.width) / 2;
			float deltaY = (originBoundRect.height - newBoundRect.height) / 2;
			sprite->setPosition(originPos.x + deltaX, originPos.y + deltaY);
		}

		return finishedTargets == m_targets.size();
	}
	std::set<std::shared_ptr<FigureBase>> SpawnAnimation::getTargets()
	{
		return m_targets;
	}
}