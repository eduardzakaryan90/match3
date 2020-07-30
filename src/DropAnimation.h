#pragma once

#include "AnimationBase.h"

#include <list>
#include <vector>
#include <set>
#include <memory>

namespace match3
{
	class FigureBase;
	class DropAnimation : public AnimationBase
	{
	public:
		DropAnimation(std::list<std::pair<std::shared_ptr<FigureBase>, int32_t>> dropingFigures, float moveDelta);
		virtual ~DropAnimation();

		virtual bool animate() override;

		void setTargetPositions();

		std::set<std::shared_ptr<FigureBase>> getTargets();

	private:
		std::list<std::pair<std::shared_ptr<FigureBase>, int32_t>> m_dropingFigures;
		std::vector<float> m_targetPositions;
		float m_moveDelta;
	};
}