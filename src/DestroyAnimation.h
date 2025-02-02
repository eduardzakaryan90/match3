#pragma once

#include "AnimationBase.h"

#include <set>
#include <memory>

namespace match3
{
	class FigureBase;
	class DestroyAnimation : public AnimationBase
	{
	public:
		DestroyAnimation(std::set<std::shared_ptr<FigureBase>> figures);
		virtual ~DestroyAnimation();

		virtual bool animate() override;

		std::set<std::shared_ptr<FigureBase>> getTargets();

	private:
		std::set<std::shared_ptr<FigureBase>> m_targets;
	};
}