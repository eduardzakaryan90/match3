#pragma once

#include "FigureBase.h"

namespace match3
{
	class ColorFigure : public FigureBase
	{
	public:
		ColorFigure(FigureType figureTypes);
		virtual ~ColorFigure();

		static std::shared_ptr<sf::Sprite> createSpriteFromColorFigureTpe(FigureType figureTypes);
	};
}