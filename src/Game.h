#pragma once

#include "FigureBase.h"
#include "SFML/System/Vector2.hpp"

#include <memory>
#include <list>
#include <vector>
#include <set>

namespace sf {
	class RenderWindow;
	class Sprite;
	class RectangleShape;
	class Text;
	class Drawable;
}

namespace match3
{
	enum class GameState {
		Active = 0,
		Won = 1,
		Lost = 2,
		Error = 3
	};

	struct Objective
	{
		FigureType figureType;
		std::shared_ptr<sf::Sprite> figureSprite;
		int32_t target;
		std::shared_ptr<sf::Text> targetText;
		sf::Vector2f centerPos;
	};

	class DropAnimation;
	class AnimationBase;
	class PatternManager;
	class Game
	{
	private:
		const float TILE_SIZE = 40.0f;
		const float OBJECTIVE_TILE_SIZE = 60.0f;
		const float MOVES_COUNT_RECT_WIDTH = 80.0f;
		const float BOARD_HORIZONTAL_DELTA = 40.0f;
		const float BOARD_VERTICAL_DELTA = 40.0f;
		const float HEADER_HEIGHT = 80.0f;

		const uint32_t FRAME_RATE = 60;

		const std::string RED_COLOR_NAME = "red";
		const std::string GREEEN_COLOR_NAME = "green";
		const std::string BLUE_COLOR_NAME = "blue";
		const std::string ORANGE_COLOR_NAME = "orange";
		const std::string VIOLET_COLOR_NAME = "violet";

		const std::string WON_TEXT = "You Won!!!";
		const std::string LOST_TEXT= "You Lost :(";

	public:
		explicit Game(int32_t columns, int32_t rows, int32_t movesCount, std::list<std::pair<std::string, int32_t>> figuresConfig);
		explicit Game(std::string errorMassage);

		std::shared_ptr<sf::RenderWindow> openGameWindow();
		void draw();
		void mouseMoveEvent(float x, float y, SwipeDirection direction);
		void createSwipeAnimation(std::shared_ptr<FigureBase> first, std::shared_ptr<FigureBase> second, SwipeDirection direction, bool isReversive = false);
		void mouseClickEvent(float x, float y);

	private:
		void drawHeader();
		void drawGameBoard();
		void handleAnimations();
		void shuffleIfNeeded();
		bool findPossibleCombinations(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures);

		FigureType getBoardColorFigureTypesFromColorName(std::string colorName);

		void createHeaderDrawables();
		void createObjectiveDrawables(std::list<std::pair<std::string, int32_t>> figuresConfig);
		void updateMovesCount(bool decrement = true);
		void updateObjectiveTarget(FigureType type, bool decrement = true);

		void createGameBoardBackgroundTiles();
		void createInitialGameBoardFigures();
		void setGameBoardFigureCoords(int32_t i, int32_t j, std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures);
		std::shared_ptr<FigureBase> getRandomColorFigure();
		std::shared_ptr<FigureBase> findFigureUnderXY(float x, float y);

		bool checkListForMatching(std::list<sf::Vector2i> checkList);

		void handleDestoryAnimation();
		void handleSwipeAnimation();
		void handleDropAnimatiom();
		void handleSpawnAnimation();

	private:
		std::shared_ptr<sf::RenderWindow> m_app;
		GameState m_gameState;
		std::shared_ptr<PatternManager> m_patternManager;

		bool m_canDecrementMovesCount;
		bool m_shuffling;

		std::shared_ptr<sf::Text> m_gameMessageText;

		std::shared_ptr<sf::RectangleShape> m_headerBackgroundRectangle;
		std::list<Objective> m_objectives;
		std::shared_ptr<sf::RectangleShape> m_movesCountRectangle;
		std::shared_ptr<sf::Text> m_movesCountText;
		int32_t m_movesCount;
		sf::Vector2f m_movesCountCenterPos;

		std::shared_ptr<sf::RectangleShape> m_gameBoardRectangle;
		std::vector<FigureType> m_colorFigureTypesInGame;
		std::list<std::shared_ptr<sf::Sprite>> m_gameBoardBackgroundTiles;
		std::vector<std::vector<std::shared_ptr<FigureBase>>> m_gameBoardFigures;
		std::vector<std::vector<std::shared_ptr<FigureBase>>> m_shuffledGameBoardFigures;
		std::set<std::shared_ptr<FigureBase>> m_spawnList;

		std::shared_ptr<AnimationBase> m_activeAnimation;
		std::shared_ptr<DropAnimation> m_droppingAnimation;
		std::list<sf::Vector2i> m_checkCoords;

		uint32_t m_windowWidth;
		uint32_t m_windowHeight;
		int32_t m_columnsSize;
		int32_t m_rowsSize;
	};
}