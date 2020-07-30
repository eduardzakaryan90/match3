#pragma once

#include "BombFigureBase.h"

#include "ResourceManager.h"

namespace match3
{
	class RBombFigure : public BombFigureBase
	{
	public:
		RBombFigure();
		virtual ~RBombFigure();

		virtual std::set<std::shared_ptr<FigureBase>> blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
			, std::set<std::shared_ptr<FigureBase>> affectedFigures = {}) override;
	};
}