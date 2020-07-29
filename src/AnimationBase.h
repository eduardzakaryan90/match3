#pragma once

namespace match3
{
	enum AnimationType {
		Destroy = 0,
		Swipe = 1,
		BlockedMove = 2,
		Drop = 3
	};

	class AnimationBase
	{
	protected:
		const float MOVE_ANIMATION_SPEED = 4.0f;
		const float SCALE_ANIMATION_SPEED = 0.05f;

	public:
		AnimationBase(AnimationType type);
		virtual ~AnimationBase();

		AnimationType type();
		virtual bool animate();

	private:
		AnimationType m_type;
	};
}