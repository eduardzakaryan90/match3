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

	void BombFigureBase::addToAffectedList(std::set<std::shared_ptr<FigureBase>>& affectedList, std::shared_ptr<FigureBase> figure
		, std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures)
	{
		if (affectedList.find(figure) != affectedList.end()) {
			return;
		}
		if (figure->type() == FigureType::BombFigureType && figure.get() != this) {
			std::shared_ptr<BombFigureBase> bomb = std::dynamic_pointer_cast<BombFigureBase>(figure);
			m_chainBlowBombs.insert(bomb);
		}
		else {
			affectedList.insert(figure);
		}
	}
}