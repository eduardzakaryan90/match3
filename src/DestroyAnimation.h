#pragma once

#include "AnimationBase.h"

#include <list>
#include <memory>

namespace match3
{
	class FigureBase;
	class DestroyAnimation : public AnimationBase
	{
	public:
		DestroyAnimation(std::list<std::shared_ptr<FigureBase>> figures);
		virtual ~DestroyAnimation();

		virtual bool animate() override;

		std::list<std::shared_ptr<FigureBase>> getTargets();

	private:
		std::list<std::shared_ptr<FigureBase>> m_targets;
	};
}