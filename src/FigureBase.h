#pragma once

#include "SFML/Graphics/Texture.hpp"

#include <memory>

namespace sf {
	class Sprite;
}

namespace match3
{
	enum class FigureType {
		RedFigureType = 0,
		GreenFigureType = 1,
		BlueFigureType = 2,
		OrangeFigureType = 3,
		VioletFigureType = 4,
		BombFigureType = 5
	};

	enum class SwipeDirection {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3
	};

	class FigureBase
	{
	private:
		const float DEFAULT_SCALE = 0.5f;

	public:
		FigureBase(std::shared_ptr<sf::Sprite> sprite, FigureType type, bool canClick, bool canSwipe);
		virtual ~FigureBase();

		FigureType type();

		bool canClick();
		bool canSwipe();

		// setters
		void setCoords(size_t x, size_t y);
		void setCoords(sf::Vector2i coordXY);

		//getters
		std::shared_ptr<sf::Sprite> sprite();

		sf::Vector2i getCoords();

		void setNeedToSpawn(bool needToSpawn);
		bool isNeedToSpawn();

	private:
		std::shared_ptr<sf::Sprite> m_sprite;
		FigureType m_type;

		bool m_canClick;
		bool m_canSwipe;

		sf::Vector2i m_coords;

		bool m_needToSpawn;
	};
}