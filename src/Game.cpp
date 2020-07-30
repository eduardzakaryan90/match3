#include "Game.h"

#include "ResourceManager.h"
#include "PatternManager.h"
#include "DestroyAnimation.h"
#include "SwipeAnimation.h"
#include "DropAnimation.h"
#include "SpawnAnimation.h"
#include "BlockedMoveAnimation.h"
#include "ColorFigure.h"
#include "BombFigureBase.h"
#include "HBombFigure.h"
#include "VBombFigure.h"
#include "RBombFigure.h"
#include "SFML/Graphics.hpp"

#include <set>

namespace match3
{
	Game::Game(int32_t columns, int32_t rows, int32_t movesCount, std::list<std::pair<std::string, int32_t>> figuresConfig)
		: m_columnsSize(columns)
		, m_rowsSize(rows)
		, m_movesCount(movesCount)
		, m_gameState(GameState::Active)
		, m_canDecrementMovesCount(false)
	{
		m_patternManager.reset(new PatternManager());

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
				case SwipeDirection::Up:
					secondFigure = findFigureUnderXY(x, y - TILE_SIZE);
					break;
				case SwipeDirection::Down:
					secondFigure = findFigureUnderXY(x, y + TILE_SIZE);
					break;
				case SwipeDirection::Right:
					secondFigure = findFigureUnderXY(x + TILE_SIZE, y);
					break;
				case SwipeDirection::Left:
					secondFigure = findFigureUnderXY(x - TILE_SIZE, y);
					break;
				}
				if (secondFigure.get() == nullptr) {
					m_activeAnimation.reset(new BlockedMoveAnimation(figure, direction));
				}
				else {
					m_canDecrementMovesCount = true;
					createSwipeAnimation(figure, secondFigure, direction);
				}
			}
		}
	}

	void Game::createSwipeAnimation(std::shared_ptr<FigureBase> first, std::shared_ptr<FigureBase> second,
		SwipeDirection direction, bool isReversive)
	{
		m_activeAnimation.reset(new SwipeAnimation(first, second, direction, isReversive));
		auto tempFigure = first;
		m_gameBoardFigures[first->getCoords().x][first->getCoords().y] = second;
		m_gameBoardFigures[second->getCoords().x][second->getCoords().y] = tempFigure;

		sf::Vector2i temp = first->getCoords();
		first->setCoords(second->getCoords());
		second->setCoords(temp);
	}

	void Game::mouseClickEvent(float x, float y)
	{
		if (m_gameState == GameState::Active && m_activeAnimation.get() == nullptr) {
			std::shared_ptr<FigureBase> figure = findFigureUnderXY(x, y);
			if (figure.get() != nullptr && figure->canClick()) {
				if (figure->type() == FigureType::BombFigureType) {
					m_canDecrementMovesCount = true;
					updateMovesCount();

					auto bomb = std::dynamic_pointer_cast<BombFigureBase>(figure);
					auto affectedFigures = bomb->blow(m_gameBoardFigures);
					m_activeAnimation.reset(new DestroyAnimation(affectedFigures));
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
		m_colorFigureTypesInGame.resize(figuresConfig.size());

		size_t figuresIterator = 0;
		size_t ObjectivesIterator = 0;
		for (auto config : figuresConfig) {
			FigureType figureType = getBoardColorFigureTypesFromColorName(config.first);
			int32_t objectiveTarget = config.second;

			if (objectiveTarget > 0) {
				Objective objective;
				objective.figureType = figureType;
				objective.target = objectiveTarget;

				// create objective sprite
				std::shared_ptr<sf::Sprite> sprite = ColorFigure::createSpriteFromColorFigureType(figureType);

				sf::FloatRect spriteBoundRect = sprite->getGlobalBounds();
				float spriteXDelta = spriteBoundRect.width / 2;
				float spriteYDelta = spriteBoundRect.height / 2;

				float x = (ObjectivesIterator + 0.5f) * OBJECTIVE_TILE_SIZE - spriteXDelta;
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
				objective.centerPos.x = (ObjectivesIterator + 0.5f) * OBJECTIVE_TILE_SIZE;
				objective.centerPos.y = (HEADER_HEIGHT + OBJECTIVE_TILE_SIZE) / 2.0f;

				m_objectives.push_back(objective);

				updateObjectiveTarget(figureType, false);
				++ObjectivesIterator;
			}

			m_colorFigureTypesInGame[figuresIterator] = figureType;
			++figuresIterator;
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
		if (m_canDecrementMovesCount && decrement) {
			m_canDecrementMovesCount = false;
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

	void Game::updateObjectiveTarget(FigureType type, bool decrement)
	{
		int32_t targetAchieved = 0;
		for (auto& objective : m_objectives) {
			if (objective.target == 0) {
				++targetAchieved;
				continue;
			}
			if (objective.figureType == type) {
				if (decrement) {
					if (objective.target > 0) {
						--objective.target;
						if (objective.target == 0) {
							++targetAchieved;
						}
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

		for (int32_t i = 0; i < m_columnsSize; ++i) {
			m_gameBoardFigures[i].resize(m_rowsSize);

			for (int32_t j = 0; j < m_rowsSize; ++j) {
				do {
					m_gameBoardFigures[i][j] = getRandomColorFigure();
				} while (m_patternManager->checkMatch({ sf::Vector2i(i, j) }, m_gameBoardFigures));
				
				setGameBoardFigureCoords(i, j);
			}
		}
	}

	void Game::setGameBoardFigureCoords(int32_t i, int32_t j)
	{
		sf::FloatRect boundRect = m_gameBoardFigures[i][j]->sprite()->getGlobalBounds();
		float x = BOARD_HORIZONTAL_DELTA + (i + 0.5f) * TILE_SIZE - boundRect.width / 2;
		float y = HEADER_HEIGHT + BOARD_VERTICAL_DELTA + (j + 0.5f) * TILE_SIZE - boundRect.height / 2;

		m_gameBoardFigures[i][j]->sprite()->setPosition(x, y);

		m_gameBoardFigures[i][j]->setCoords(i, j);
	}

	std::shared_ptr<FigureBase> Game::getRandomColorFigure()
	{
		int32_t colorPossibiltyCheck = (std::rand() * static_cast<int32_t>(m_colorFigureTypesInGame.size()) - 1) / RAND_MAX;
		return std::shared_ptr<FigureBase>(new ColorFigure(m_colorFigureTypesInGame[colorPossibiltyCheck]));
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

	bool Game::checkListForMatching(std::list<sf::Vector2i> checkList)
	{
		if (m_patternManager->checkMatch(checkList, m_gameBoardFigures)) {
			std::set<std::shared_ptr<FigureBase>> matchFigureList;

			for (auto matchCoord : m_patternManager->getMatchedCoords()) {
				matchFigureList.insert(m_gameBoardFigures[matchCoord.x][matchCoord.y]);
			}

			for (auto bombCoordPair : m_patternManager->getBombs()) {
				std::shared_ptr<FigureBase> bomb(bombCoordPair.second);
				bomb->setNeedToSpawn(true);
				m_gameBoardFigures[bombCoordPair.first.x][bombCoordPair.first.y] = bomb;
				setGameBoardFigureCoords(bombCoordPair.first.x, bombCoordPair.first.y);
				m_spawnList.insert(bomb);
			}

			m_activeAnimation.reset(new DestroyAnimation(matchFigureList));
			return true;
		}
		return false;
	}

	void Game::handleDestoryAnimation()
	{
		auto destroyAnim = std::dynamic_pointer_cast<DestroyAnimation>(m_activeAnimation);

		std::set<std::shared_ptr<FigureBase>> spawnListBeforeDrop;
		for (auto figure : m_spawnList) {
			if (figure->isNeedToSpawn()) {
				spawnListBeforeDrop.insert(figure);
			}
		}
		m_spawnList.clear();

		std::shared_ptr<DropAnimation> dropyAnim;
		std::list<std::pair<std::shared_ptr<FigureBase>, int32_t>> dropingFigures;

		std::set<int32_t> columnIndexes;

		for (auto target : destroyAnim->getTargets()) {
			updateObjectiveTarget(target->type());

			int32_t x = target->getCoords().x;
			int32_t y = target->getCoords().y;
			bool bombFound = false;
			for (auto figure : spawnListBeforeDrop) {
				if (figure->getCoords().x == x && figure->getCoords().y == y) {
					bombFound = true;
					break;
				}
			}

			if (!bombFound) {
				columnIndexes.insert(x);
				m_gameBoardFigures[x][y].reset();
			}
		}

		for (auto i : columnIndexes) {
			for (int32_t j = m_rowsSize - 1; j >= 0; --j) {
				if (m_gameBoardFigures[i][j].get() == nullptr) {
					int32_t dropStepsCount = 1;
					std::shared_ptr<FigureBase> targetFigure = nullptr;

					for (int32_t checkRowIndex = j - 1; checkRowIndex >= 0; --checkRowIndex) {
						if (m_gameBoardFigures[i][checkRowIndex].get() != nullptr) {
							targetFigure = m_gameBoardFigures[i][checkRowIndex];
							m_gameBoardFigures[i][checkRowIndex].reset();
							m_gameBoardFigures[i][j] = targetFigure;
							targetFigure->setCoords(i, j);
							break;
						}
						else {
							++dropStepsCount;
						}
					}

					if (targetFigure.get() != nullptr) {
						dropingFigures.push_back(std::pair<std::shared_ptr<FigureBase>, int32_t>(targetFigure, dropStepsCount));
					}
					else {
						std::shared_ptr<FigureBase> newFigure = getRandomColorFigure();
						m_gameBoardFigures[i][j] = newFigure;
						newFigure->setCoords(i, j);
						newFigure->setNeedToSpawn(true);
						setGameBoardFigureCoords(i, j);
						m_spawnList.insert(newFigure);
					}
				}
			}
		}

		if (dropingFigures.size() > 0) {
			dropyAnim.reset(new DropAnimation(dropingFigures, TILE_SIZE));
		}

		if (spawnListBeforeDrop.size() > 0) {
			if (dropyAnim.get() == nullptr) {
				spawnListBeforeDrop.insert(m_spawnList.begin(), m_spawnList.end());
				m_spawnList.clear();
			}

			m_activeAnimation.reset(new SpawnAnimation(spawnListBeforeDrop));

			m_droppingAnimation = dropyAnim;
		}
		else if (dropyAnim.get() != nullptr) {
			m_droppingAnimation.reset();

			dropyAnim->setTargetPositions();
			m_activeAnimation = dropyAnim;
		}
		else {
			m_activeAnimation.reset(new SpawnAnimation(m_spawnList));
		}
	}

	void Game::handleSwipeAnimation()
	{
		auto swipeAnim = std::dynamic_pointer_cast<SwipeAnimation>(m_activeAnimation);
		std::list<sf::Vector2i> checkList = { swipeAnim->getFigure1()->getCoords(),
			swipeAnim->getFigure2()->getCoords() };

		if (!checkListForMatching(checkList)) {
			if (!swipeAnim->isReverseAnim()) {
				createSwipeAnimation(swipeAnim->getFigure2(), swipeAnim->getFigure1(), swipeAnim->getDirection(), true);
			}
			else {
				m_activeAnimation.reset();
			}
		}
		else {
			updateMovesCount();
		}
	}

	void Game::handleDropAnimatiom()
	{
		auto dropAnim = std::dynamic_pointer_cast<DropAnimation>(m_activeAnimation);
		for (auto target : dropAnim->getTargets()) {
			m_checkCoords.push_back(target->getCoords());
		}

		if (m_spawnList.size() > 0) {
			m_activeAnimation.reset(new SpawnAnimation(m_spawnList));
			m_spawnList.clear();
		}
		else {
			m_activeAnimation.reset();
		}
	}

	void Game::handleSpawnAnimation()
	{
		auto spawnAnim = std::dynamic_pointer_cast<SpawnAnimation>(m_activeAnimation);
		for (auto target : spawnAnim->getTargets()) {
			m_checkCoords.push_back(target->getCoords());
		}

		if (m_droppingAnimation.get() != nullptr) {
			m_droppingAnimation->setTargetPositions();
			m_activeAnimation = m_droppingAnimation;

			m_droppingAnimation.reset();
		}
		else {
			if (!checkListForMatching(m_checkCoords)) {
				m_activeAnimation.reset();
			}
			m_checkCoords.clear();
		}
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
			handleAnimations();
		}

		// draw board figures
		for (auto& row : m_gameBoardFigures) {
			for (auto& figure : row) {
				m_app->draw(*(figure->sprite()));
			}
		}
	}

	void Game::handleAnimations()
	{
		auto animType = m_activeAnimation->type();
		switch (animType)
		{
		case match3::AnimationType::Destroy:
			handleDestoryAnimation();
			break;
		case match3::AnimationType::Swipe:
			handleSwipeAnimation();
			break;
		case match3::AnimationType::Drop:
			handleDropAnimatiom();
			break;
		case match3::AnimationType::Spawn:
			handleSpawnAnimation();
			break;
		default:
			m_activeAnimation.reset();
			break;
		}
	}
}