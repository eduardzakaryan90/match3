#pragma once

#include "SFML/Graphics/Texture.hpp"

#include <memory>

namespace sf {
	class Sprite;
}

namespace match3
{
	enum FigureType {
		RedFigureType = 0,
		GreenFigureType = 1,
		BlueFigureType = 2,
		OrangeFigureType = 3,
		VioletFigureType = 4,
		BombFigureType = 5
	};

	enum SwipeDirection {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3
	};

	class FigureBase
	{
	public:
		FigureBase(std::shared_ptr<sf::Sprite> sprite, FigureType type, bool canClick, bool canSwipe);
		virtual ~FigureBase();

		FigureType type();

		bool canClick();
		bool canSwipe();

		// setters

		void setCoordX(size_t x);
		void setCoordY(size_t y);

		//getters

		std::shared_ptr<sf::Sprite> sprite();

		size_t getCoordX();
		size_t getCoordY();

	private:
		std::shared_ptr<sf::Sprite> m_sprite;
		FigureType m_type;

		bool m_canClick;
		bool m_canSwipe;

		size_t m_coordX;
		size_t m_coordY;
	};
}