#pragma once

#include "AnimationBase.h"

#include "SFML/System/Vector2.hpp"

#include <set>
#include <memory>

namespace match3
{
	class FigureBase;
	class SpawnAnimation : public AnimationBase
	{
	private:
		const sf::Vector2f TARGET_SCALE = sf::Vector2f(0.5f, 0.5f);

	public:
		SpawnAnimation(std::set<std::shared_ptr<FigureBase>> figures);
		virtual ~SpawnAnimation();

		virtual bool animate() override;

		std::set<std::shared_ptr<FigureBase>> getTargets();

	private:
		std::set<std::shared_ptr<FigureBase>> m_targets;
	};
}