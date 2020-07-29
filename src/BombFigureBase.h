#pragma once

#include "FigureBase.h"

#include<list>

namespace match3
{
	class BombFigureBase : public FigureBase
	{
	public:
		BombFigureBase(std::shared_ptr<sf::Sprite> sprite);
		virtual ~BombFigureBase();

		std::list<std::shared_ptr<FigureBase>> getTargets();
	};
}