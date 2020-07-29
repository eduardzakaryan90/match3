#include "BombFigureBase.h"

namespace match3
{
	BombFigureBase::BombFigureBase(std::shared_ptr<sf::Sprite> sprite)
		: FigureBase(sprite, FigureType::BombFigureType, true, true)
	{

	}

	BombFigureBase::~BombFigureBase()
	{

	}

	std::list<std::shared_ptr<FigureBase>> BombFigureBase::getTargets()
	{
		return std::list<std::shared_ptr<FigureBase>>();
	}
}