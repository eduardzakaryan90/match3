#pragma once

#include "SFML/System/Vector2.hpp"

#include <memory>
#include <vector>
#include <list>

namespace sf {
	class RenderWindow;
	class Sprite;
	class RectangleShape;
	class Text;
	class Drawable;
}

namespace match3
{
	enum BoardColorFigureType {
		RedFigureType = 0,
		GreenFigureType = 1,
		BlueFigureType = 2,
		OrangeFigureType = 3,
		VioletFigureType = 4
	};

	enum MouseMoveDirection {
		Up = 0,
		Down = 1,
		Right = 2,
		Left = 3
	};

	enum GameState {
		Active = 0,
		Won = 1,
		Lost = 2
	};

	struct Objective
	{
		BoardColorFigureType figureType;
		std::shared_ptr<sf::Sprite> figureSprite;
		int target;
		std::shared_ptr<sf::Text> targetText;
		sf::Vector2f centerPos;
	};

	class Game
	{
	private:
		const float TILE_SIZE = 40.0f;
		const float OBJECTIVE_TILE_SIZE = 60.0f;
		const float MOVES_COUNT_RECT_WIDTH = 80.0f;
		const float BOARD_HORIZONTAL_DELTA = 40.0f;
		const float BOARD_VERTICAL_DELTA = 40.0f;
		const float HEADER_HEIGHT = 80.0f;

		const unsigned int FRAME_RATE = 10;

		const std::string RED_COLOR_NAME = "red";
		const std::string GREEEN_COLOR_NAME = "green";
		const std::string BLUE_COLOR_NAME = "blue";
		const std::string ORANGE_COLOR_NAME = "orange";
		const std::string VIOLET_COLOR_NAME = "violet";

		const std::string WON_TEXT = "You Win!!!";
		const std::string LOST_TEXT= "You Lose :(";

	public:
		explicit Game(int columns, int rows, int movesCount, std::list<std::pair<std::string, int>> figuresConfig, bool enableBlockFigures);

		std::shared_ptr<sf::RenderWindow> getApp();
		void draw();
		void mouseMoveEvent(int X, int Y, MouseMoveDirection direction);
		void mouseClickEvent(int X, int Y);

	private:
		void drawHeader();
		void drawGameBoard();

		BoardColorFigureType getBoardColorFigureTypesFromColorName(std::string colorName);
		std::shared_ptr<sf::Sprite> createSpriteFromColorFigureTpe(BoardColorFigureType figureTypes);

		void createHeaderDrawables();
		void createObjectiveDrawables(std::list<std::pair<std::string, int>> figuresConfig);
		void updateMovesCount(bool decrement = true);
		void updateObjectiveTarget(BoardColorFigureType type, int decrementValue = 0);

		void createGameBoardBackgroundTiles(int columns, int rows);

	private:
		std::shared_ptr<sf::RenderWindow> m_app;
		GameState m_gameState;

		std::shared_ptr<sf::Text> m_endGameText;

		std::shared_ptr<sf::RectangleShape> m_headerBackgroundRectangle;
		std::list<Objective> m_objectives;
		std::shared_ptr<sf::RectangleShape> m_movesCountRectangle;
		std::shared_ptr<sf::Text> m_movesCountText;
		int m_movesCount;
		sf::Vector2f m_movesCountCenterPos;

		std::list<BoardColorFigureType> m_colorFigureTypesInGame;
		std::vector<std::vector<std::shared_ptr<sf::Sprite>>> m_gameBoardBackgroundTiles;

		unsigned int m_windowWidth;
		unsigned int m_windowHeight;
		bool m_enableBlockFigures;
	};
}