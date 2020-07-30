#pragma once

#include "FigureBase.h"

#include<set>

namespace match3
{
	class BombFigureBase : public FigureBase
	{
	public:
		BombFigureBase(std::shared_ptr<sf::Sprite> sprite);
		virtual ~BombFigureBase();

		virtual std::set<std::shared_ptr<FigureBase>> blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
			, std::set<std::shared_ptr<FigureBase>> affectedFigures = {}) = 0;

		void addToAffectedList(std::set<std::shared_ptr<FigureBase>>& affectedList, std::shared_ptr<FigureBase> figure
			, std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures);

	protected:
		std::set<std::shared_ptr<FigureBase>> affectedFigures;
		std::set<std::shared_ptr<BombFigureBase>> m_chainBlowBombs;
	};
}