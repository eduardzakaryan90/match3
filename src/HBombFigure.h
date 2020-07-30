#pragma once

#include "BombFigureBase.h"

namespace match3
{
	class HBombFigure : public BombFigureBase
	{
	public:
		HBombFigure();
		virtual ~HBombFigure();

		virtual std::set<std::shared_ptr<FigureBase>> blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
			, std::set<std::shared_ptr<FigureBase>> affectedFigures = {}) override;
	};
}