#pragma once

#include "AnimationBase.h"
#include "FigureBase.h"

#include <memory>

namespace match3
{
	class SwipeAnimation : public AnimationBase
	{
	public:
		SwipeAnimation(std::shared_ptr<FigureBase> first,
			std::shared_ptr<FigureBase> second, SwipeDirection direction);
		virtual ~SwipeAnimation();

		virtual bool animate() override;

		std::shared_ptr<FigureBase> getFigure1();
		std::shared_ptr<FigureBase> getFigure2();

	private:
		std::shared_ptr<FigureBase> m_figure1;
		sf::Vector2f m_targetPos1;

		std::shared_ptr<FigureBase> m_figure2;
		sf::Vector2f m_targetPos2;

		SwipeDirection m_direction;
	};
}