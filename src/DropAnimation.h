#pragma once

#include "AnimationBase.h"

#include <list>
#include <memory>

namespace match3
{
	class FigureBase;
	class DropAnimation : public AnimationBase
	{
	public:
		DropAnimation(std::list<std::shared_ptr<FigureBase>> figures, float moveDelta);
		virtual ~DropAnimation();

		virtual bool animate() override;

		std::list<std::shared_ptr<FigureBase>> getTargets();

	private:
		std::list<std::shared_ptr<FigureBase>> m_targets;
		float m_targetPosY;
		float m_moveDelta;
	};
}