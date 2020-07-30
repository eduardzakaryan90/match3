#pragma once

#include "BombFigureBase.h"

namespace match3
{
	class VBombFigure : public BombFigureBase
	{
	public:
		VBombFigure();
		virtual ~VBombFigure();

		virtual std::set<std::shared_ptr<FigureBase>> blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
			, std::set<std::shared_ptr<FigureBase>> affectedFigures = {}) override;
	};
}