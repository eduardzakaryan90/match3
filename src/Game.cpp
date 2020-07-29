#include "Game.h"

#include "DestroyAnimation.h"
#include "SwipeAnimation.h"
#include "BlockedMoveAnimation.h"
#include "ColorFigure.h"
#include "BombFigureBase.h"
#include "ResourceManager.h"
#include "SFML/Graphics.hpp"

namespace match3
{
	Game::Game(int32_t columns, int32_t rows, int32_t movesCount, std::list<std::pair<std::string, int32_t>> figuresConfig)
		: m_columnsSize(columns)
		, m_rowsSize(rows)
		, m_movesCount(movesCount)
		, m_gameState(GameState::Active)
	{
		std::srand(static_cast<uint32_t>(std::time(nullptr)));

		m_windowWidth = static_cast<uint32_t>(m_columnsSize * TILE_SIZE + 2 * BOARD_HORIZONTAL_DELTA);
		m_windowHeight = static_cast<uint32_t>(m_rowsSize * TILE_SIZE + 2 * BOARD_VERTICAL_DELTA + HEADER_HEIGHT);

		createObjectiveDrawables(figuresConfig);

		createHeaderDrawables();
		createGameBoardBackgroundTiles();
		createInitialGameBoardFigures();
		
		m_gameMessageText.reset(new sf::Text("", ResourceManager::getFont(Font::MainFont), 50));
		m_gameMessageText->setStyle(sf::Text::Bold);
		m_gameMessageText->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_gameMessageText->setOutlineThickness(7.0f);
		m_gameMessageText->setLetterSpacing(1.5f);
	}

	Game::Game(std::string errorMassage)
		: m_gameState(GameState::Error)
	{
		m_gameMessageText.reset(new sf::Text(errorMassage, ResourceManager::getFont(Font::MainFont), 24));
		m_gameMessageText->setStyle(sf::Text::Regular);
		m_gameMessageText->setFillColor(sf::Color::Magenta);
		m_gameMessageText->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_gameMessageText->setOutlineThickness(1.0f);
		m_gameMessageText->setLetterSpacing(1.5f);
		m_gameMessageText->setPosition(TILE_SIZE, TILE_SIZE);

		sf::FloatRect boundRect = m_gameMessageText->getGlobalBounds();
		m_windowWidth = static_cast<uint32_t>(TILE_SIZE * 2 + boundRect.width);
		m_windowHeight = static_cast<uint32_t>(TILE_SIZE * 2 + boundRect.height);
	}

	std::shared_ptr<sf::RenderWindow> Game::openGameWindow()
	{
		m_app.reset(new sf::RenderWindow(sf::VideoMode(m_windowWidth, m_windowHeight), "Match 3", sf::Style::Close));
		m_app->setFramerateLimit(FRAME_RATE);

		return m_app;
	}

	void Game::draw()
	{
		if (m_gameState == GameState::Active) {
			drawGameBoard();
			drawHeader();
		}
		else {
			m_app->draw(*m_gameMessageText);
		}
	}

	void Game::mouseMoveEvent(float x, float y, SwipeDirection direction)
	{
		if (m_gameState == GameState::Active && m_activeAnimation.get() == nullptr) {
			std::shared_ptr<FigureBase> figure = findFigureUnderXY(x, y);
			if (figure.get() != nullptr && figure->canSwipe()) {
				std::shared_ptr<FigureBase> secondFigure;
				switch (direction)
				{
				case match3::Up:
					secondFigure = findFigureUnderXY(x, y - TILE_SIZE);
					break;
				case match3::Down:
					secondFigure = findFigureUnderXY(x, y + TILE_SIZE);
					break;
				case match3::Right:
					secondFigure = findFigureUnderXY(x + TILE_SIZE, y);
					break;
				case match3::Left:
					secondFigure = findFigureUnderXY(x - TILE_SIZE, y);
					break;
				}
				if (secondFigure.get() == nullptr) {
					m_activeAnimation.reset(new BlockedMoveAnimation(figure, direction));
				}
				else {
					CreateSwipeAnimation(figure, secondFigure, direction);
				}
			}
		}
	}

	void Game::CreateSwipeAnimation(std::shared_ptr<FigureBase> first, std::shared_ptr<FigureBase> second, SwipeDirection direction)
	{
		m_activeAnimation.reset(new SwipeAnimation(first, second, direction));
		auto tempFigure = first;
		m_gameBoardFigures[first->getCoordX()][first->getCoordY()] = second;
		m_gameBoardFigures[second->getCoordX()][second->getCoordY()] = tempFigure;

		size_t tempX = first->getCoordX();
		size_t tempY = first->getCoordY();
		first->setCoordX(second->getCoordX());
		first->setCoordY(second->getCoordY());
		second->setCoordX(tempX);
		second->setCoordY(tempY);
	}

	void Game::mouseClickEvent(float x, float y)
	{
		if (m_gameState == GameState::Active && m_activeAnimation.get() == nullptr) {
			std::shared_ptr<FigureBase> figure = findFigureUnderXY(x, y);
			if (figure.get() != nullptr && figure->canClick()) {
				switch (figure->type())
				{
				case FigureType::BombFigureType:
					auto bomb = std::dynamic_pointer_cast<BombFigureBase>(figure);
					m_activeAnimation.reset(new DestroyAnimation(bomb->getTargets()));

					updateMovesCount();
					break;
				}
			}
		}
	}

#pragma warning( push )
#pragma warning( disable : 4715)
	FigureType Game::getBoardColorFigureTypesFromColorName(std::string colorName)
	{
		if (colorName == RED_COLOR_NAME) {
			return FigureType::RedFigureType;
		}
		else if (colorName == GREEEN_COLOR_NAME) {
			return FigureType::GreenFigureType;
		}
		else if (colorName == BLUE_COLOR_NAME) {
			return FigureType::BlueFigureType;
		}
		else if (colorName == ORANGE_COLOR_NAME) {
			return FigureType::OrangeFigureType;
		}
		else if (colorName == VIOLET_COLOR_NAME) {
			return FigureType::VioletFigureType;
		}
	}
#pragma warning( pop )

	void Game::createObjectiveDrawables(std::list<std::pair<std::string, int32_t>> figuresConfig)
	{
		size_t i = 0;
		for (auto config : figuresConfig) {
			FigureType figureType = getBoardColorFigureTypesFromColorName(config.first);
			int32_t objectiveTarget = config.second;

			if (objectiveTarget > 0) {
				Objective objective;
				objective.figureType = figureType;
				objective.target = objectiveTarget;

				// create objective sprite
				std::shared_ptr<sf::Sprite> sprite = ColorFigure::createSpriteFromColorFigureTpe(figureType);

				sf::FloatRect spriteBoundRect = sprite->getGlobalBounds();
				float spriteXDelta = spriteBoundRect.width / 2;
				float spriteYDelta = spriteBoundRect.height / 2;

				float x = (i + 0.5f) * OBJECTIVE_TILE_SIZE - spriteXDelta;
				float y = OBJECTIVE_TILE_SIZE / 2 - spriteYDelta;

				sprite->setPosition(x, y);
				objective.figureSprite = sprite;

				// create objective target text
				std::shared_ptr<sf::Text> text(new sf::Text("", ResourceManager::getFont(Font::MainFont), 20));
				text->setStyle(sf::Text::Regular);
				text->setFillColor(sf::Color::White);
				text->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
				text->setOutlineThickness(2.0f);
				text->setLetterSpacing(1.5f);
				objective.targetText = text;
				objective.centerPos.x = (i + 0.5f) * OBJECTIVE_TILE_SIZE;
				objective.centerPos.y = (HEADER_HEIGHT + OBJECTIVE_TILE_SIZE) / 2.0f;

				m_objectives.push_back(objective);

				updateObjectiveTarget(figureType, 0);
				++i;
			}

			m_colorFigureTypesInGame.push_back(figureType);
		}
	}

	void Game::createHeaderDrawables()
	{
		// draw objectives rectangle
		m_headerBackgroundRectangle.reset(new sf::RectangleShape());
		m_headerBackgroundRectangle->setSize(sf::Vector2f(m_windowWidth * 1.0f, HEADER_HEIGHT));
		m_headerBackgroundRectangle->setFillColor(ResourceManager::getColor(Color::Title1Color));
		m_headerBackgroundRectangle->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_headerBackgroundRectangle->setOutlineThickness(3.0f);
		m_headerBackgroundRectangle->setPosition(0.0f, 0.0f);

		// draw moves count rectangle
		float movesCountRectX = OBJECTIVE_TILE_SIZE * 3;
		m_movesCountRectangle.reset(new sf::RectangleShape());
		m_movesCountRectangle->setSize(sf::Vector2f(MOVES_COUNT_RECT_WIDTH, HEADER_HEIGHT));
		m_movesCountRectangle->setFillColor(ResourceManager::getColor(Color::Title2Color));
		m_movesCountRectangle->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_movesCountRectangle->setOutlineThickness(3.0f);
		m_movesCountRectangle->setPosition(movesCountRectX, 0.0f);

		// draw moves count text
		m_movesCountText.reset(new sf::Text("", ResourceManager::getFont(Font::MainFont), 30));
		m_movesCountText->setStyle(sf::Text::Bold);
		m_movesCountText->setFillColor(sf::Color::White);
		m_movesCountText->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_movesCountText->setOutlineThickness(5.0f);
		m_movesCountText->setLetterSpacing(1.5f);
		m_movesCountCenterPos.x = movesCountRectX + MOVES_COUNT_RECT_WIDTH * 0.5f;
		m_movesCountCenterPos.y = HEADER_HEIGHT / 2.0f;
		updateMovesCount(false);
	}

	void Game::updateMovesCount(bool decrement)
	{
		if (decrement) {
			--m_movesCount;
			if (m_movesCount == 0) {
				m_gameMessageText->setString(LOST_TEXT);
				m_gameMessageText->setFillColor(sf::Color::Red);
				sf::FloatRect boundRect = m_gameMessageText->getGlobalBounds();
				float xDelta = boundRect.width / 2;
				float yDelta = boundRect.height / 2;
				m_gameMessageText->setPosition(m_windowWidth / 2 - xDelta, m_windowHeight / 2 - yDelta);
				m_gameState = GameState::Lost;
			}
		}

		m_movesCountText->setString(std::to_string(m_movesCount));
		sf::FloatRect boundRect = m_movesCountText->getGlobalBounds();
		float xDelta = boundRect.width / 2;
		float yDelta = boundRect.height / 2;
		m_movesCountText->setPosition(m_movesCountCenterPos.x - xDelta, m_movesCountCenterPos.y - yDelta);
	}

	void Game::updateObjectiveTarget(FigureType type, int32_t decrementValue)
	{
		int32_t targetAchieved = 0;
		for (auto& objective : m_objectives) {
			if (objective.target == 0) {
				++targetAchieved;
				continue;
			}
			if (objective.figureType == type) {
				if (objective.target > 0) {
					objective.target = objective.target - decrementValue;
					if (objective.target == 0) {
						++targetAchieved;
					}
				}

				objective.targetText->setString(std::to_string(objective.target));
				sf::FloatRect boundRect = objective.targetText->getGlobalBounds();
				float xDelta = boundRect.width / 2;
				float yDelta = boundRect.height / 2;
				objective.targetText->setPosition(objective.centerPos.x - xDelta, objective.centerPos.y - 2 * yDelta);
				break;
			}
		}

		if (targetAchieved == m_objectives.size()) {
			m_gameMessageText->setString(WON_TEXT);
			m_gameMessageText->setFillColor(sf::Color::Green);
			sf::FloatRect boundRect = m_gameMessageText->getGlobalBounds();
			float xDelta = boundRect.width / 2;
			float yDelta = boundRect.height / 2;
			m_gameMessageText->setPosition(m_windowWidth / 2 - xDelta, m_windowHeight / 2 - yDelta);
			m_gameState = GameState::Won;
		}
	}

	void Game::createGameBoardBackgroundTiles()
	{
		m_gameBoardRectangle.reset(new sf::RectangleShape());
		m_gameBoardRectangle->setSize(sf::Vector2f(TILE_SIZE * m_columnsSize, TILE_SIZE * m_rowsSize));
		m_gameBoardRectangle->setFillColor(ResourceManager::getColor(Color::Title2Color));
		m_gameBoardRectangle->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_gameBoardRectangle->setOutlineThickness(3.0f);
		m_gameBoardRectangle->setPosition(BOARD_HORIZONTAL_DELTA, HEADER_HEIGHT + BOARD_VERTICAL_DELTA);
		
		for (size_t i = 0; i < m_columnsSize; ++i) {
			for (size_t j = 0; j < m_rowsSize; ++j) {
				if ((i + j) % 2 == 0) {
					continue;
				}
				std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(ResourceManager::getTexture(Texture::Title1Texture)));

				sprite->setPosition(BOARD_HORIZONTAL_DELTA + i * TILE_SIZE, HEADER_HEIGHT + BOARD_VERTICAL_DELTA + j * TILE_SIZE);

				m_gameBoardBackgroundTiles.push_back(sprite);
			}
		}
	}

	void Game::createInitialGameBoardFigures()
	{
		m_gameBoardFigures.resize(m_columnsSize);

		for (size_t i = 0; i < m_columnsSize; ++i) {
			m_gameBoardFigures[i].resize(m_rowsSize);

			for (size_t j = 0; j < m_rowsSize; ++j) {
				m_gameBoardFigures[i][j] = getRandomColorFigure();

				sf::FloatRect boundRect = m_gameBoardFigures[i][j]->sprite()->getGlobalBounds();

				float x = BOARD_HORIZONTAL_DELTA + (i + 0.5f) * TILE_SIZE - boundRect.width / 2;
				float y = HEADER_HEIGHT + BOARD_VERTICAL_DELTA + (j + 0.5f) * TILE_SIZE - boundRect.height / 2;

				m_gameBoardFigures[i][j]->sprite()->setPosition(x, y);

				m_gameBoardFigures[i][j]->setCoordX(i);
				m_gameBoardFigures[i][j]->setCoordY(j);
			}
		}
	}

	std::shared_ptr<FigureBase> Game::getRandomColorFigure()
	{
		int32_t blockPossibiltyCheck = (std::rand() * 4) / RAND_MAX;
		return std::shared_ptr<FigureBase>(new ColorFigure(static_cast<FigureType>(blockPossibiltyCheck)));
	}

	std::shared_ptr<FigureBase> Game::findFigureUnderXY(float x, float y)
	{
		float gameBoardLocalX = x - BOARD_HORIZONTAL_DELTA;
		float gameBoardLocalY = y - HEADER_HEIGHT - BOARD_VERTICAL_DELTA;
		if (gameBoardLocalX < 0 || gameBoardLocalY < 0) {
			return nullptr;
		}

		int32_t i = static_cast<int32_t>(gameBoardLocalX / TILE_SIZE);
		int32_t j = static_cast<int32_t>(gameBoardLocalY / TILE_SIZE);

		if (i >= m_columnsSize || j >= m_rowsSize) {
			return nullptr;
		}

		return m_gameBoardFigures[i][j];
	}

	void Game::drawHeader()
	{
		// draw objectives area			
		m_app->draw(*m_headerBackgroundRectangle);
		for (auto& objective : m_objectives) {
			m_app->draw(*(objective.figureSprite));
			m_app->draw(*(objective.targetText));
		}

		// draw moves count area
		m_app->draw(*m_movesCountRectangle);
		m_app->draw(*m_movesCountText);
	}

	void Game::drawGameBoard()
	{
		// draw background tiles
		m_app->draw(*m_gameBoardRectangle);
		for (auto tile : m_gameBoardBackgroundTiles) {
			m_app->draw(*tile);
		}

		// animate
		if (m_activeAnimation.get() != nullptr && m_activeAnimation->animate()) {
			m_activeAnimation.reset();
		}

		// draw board figures
		for (auto& row : m_gameBoardFigures) {
			for (auto& figure : row) {
				m_app->draw(*(figure->sprite()));
			}
		}
	}
}