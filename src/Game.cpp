#include "Game.h"

#include "ResourceManager.h"
#include "SFML/Graphics.hpp"

namespace match3
{
	Game::Game(int columns, int rows, int movesCount, std::list<std::pair<std::string, int>> figuresConfig, bool enableBlockFigures)
		: m_enableBlockFigures(enableBlockFigures)
		, m_movesCount(movesCount)
		, m_gameState(GameState::Active)
	{
		m_windowWidth = (unsigned int) (columns * TILE_SIZE + 2 * BOARD_HORIZONTAL_DELTA);
		m_windowHeight = (unsigned int) (rows * TILE_SIZE + 2 * BOARD_VERTICAL_DELTA + HEADER_HEIGHT);

		m_app.reset(new sf::RenderWindow(sf::VideoMode(m_windowWidth, m_windowHeight), "Match 3", sf::Style::Close));
		m_app->setFramerateLimit(FRAME_RATE);

		createObjectiveDrawables(figuresConfig);

		createHeaderDrawables();
		createGameBoardBackgroundTiles(columns, rows);
		
		m_endGameText.reset(new sf::Text("", ResourceManager::getFont(Font::MainFont), 50));
		m_endGameText->setStyle(sf::Text::Bold);
		m_endGameText->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_endGameText->setOutlineThickness(7.0f);
		m_endGameText->setLetterSpacing(1.5f);
	}

	std::shared_ptr<sf::RenderWindow> Game::getApp()
	{
		return m_app;
	}

	void Game::draw()
	{
		if (m_gameState == GameState::Active) {
			drawHeader();
			drawGameBoard();
		}
		else {
			m_app->draw(*m_endGameText);
		}
	}

	void Game::mouseMoveEvent(int X, int Y, MouseMoveDirection direction)
	{
		// TODO
		draw();
	}

	void Game::mouseClickEvent(int X, int Y)
	{
		// TODO
		draw();
	}

	BoardColorFigureType Game::getBoardColorFigureTypesFromColorName(std::string colorName)
	{
		if (colorName == RED_COLOR_NAME) {
			return BoardColorFigureType::RedFigureType;
		}
		else if (colorName == GREEEN_COLOR_NAME) {
			return BoardColorFigureType::GreenFigureType;
		}
		else if (colorName == BLUE_COLOR_NAME) {
			return BoardColorFigureType::BlueFigureType;
		}
		else if (colorName == ORANGE_COLOR_NAME) {
			return BoardColorFigureType::OrangeFigureType;
		}
		else if (colorName == VIOLET_COLOR_NAME) {
			return BoardColorFigureType::VioletFigureType;
		}
	}

	std::shared_ptr<sf::Sprite> Game::createSpriteFromColorFigureTpe(BoardColorFigureType figureTypes)
	{
		sf::Sprite * sprite;
		switch (figureTypes)
		{
		case match3::RedFigureType:
			sprite = new sf::Sprite(ResourceManager::getTexture(Texture::RedTexture));
			break;
		case match3::GreenFigureType:
			sprite = new sf::Sprite(ResourceManager::getTexture(Texture::GreenTexture));
			break;
		case match3::BlueFigureType:
			sprite = new sf::Sprite(ResourceManager::getTexture(Texture::BlueTexture));
			break;
		case match3::OrangeFigureType:
			sprite = new sf::Sprite(ResourceManager::getTexture(Texture::OrangeTexture));
			break;
		case match3::VioletFigureType:
			sprite = new sf::Sprite(ResourceManager::getTexture(Texture::VioletTexture));
			break;
		}
		sprite->setScale(0.6f, 0.6f);
		return std::shared_ptr<sf::Sprite>(sprite);
	}

	void Game::createObjectiveDrawables(std::list<std::pair<std::string, int>> figuresConfig)
	{
		int i = 0;
		for (auto config : figuresConfig) {
			BoardColorFigureType figureType = getBoardColorFigureTypesFromColorName(config.first);
			int objectiveTarget = config.second;

			if (objectiveTarget > 0) {
				Objective objective;
				objective.figureType = figureType;
				objective.target = objectiveTarget;

				// create objective sprite
				std::shared_ptr<sf::Sprite> sprite = createSpriteFromColorFigureTpe(figureType);

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
				m_endGameText->setString(LOST_TEXT);
				m_endGameText->setFillColor(sf::Color::Red);
				sf::FloatRect boundRect = m_endGameText->getGlobalBounds();
				float xDelta = boundRect.width / 2;
				float yDelta = boundRect.height / 2;
				m_endGameText->setPosition(m_windowWidth / 2 - xDelta, m_windowHeight / 2 - yDelta);
				m_gameState = GameState::Lost;
			}
		}

		m_movesCountText->setString(std::to_string(m_movesCount));
		sf::FloatRect boundRect = m_movesCountText->getGlobalBounds();
		float xDelta = boundRect.width / 2;
		float yDelta = boundRect.height / 2;
		m_movesCountText->setPosition(m_movesCountCenterPos.x - xDelta, m_movesCountCenterPos.y - yDelta);
	}

	void Game::updateObjectiveTarget(BoardColorFigureType type, int decrementValue)
	{
		int targetAchieved = 0;
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
			m_endGameText->setString(WON_TEXT);
			m_endGameText->setFillColor(sf::Color::Green);
			sf::FloatRect boundRect = m_endGameText->getGlobalBounds();
			float xDelta = boundRect.width / 2;
			float yDelta = boundRect.height / 2;
			m_endGameText->setPosition(m_windowWidth / 2 - xDelta, m_windowHeight / 2 - yDelta);
			m_gameState = GameState::Won;
		}
	}

	void Game::createGameBoardBackgroundTiles(int columns, int rows)
	{
		m_gameBoardBackgroundTiles.resize(rows);

		for (int i = 0; i < rows; ++i) {
			m_gameBoardBackgroundTiles[i].resize(columns);
			for (int j = 0; j < columns; ++j) {
				std::shared_ptr<sf::Sprite> sprite(
					new sf::Sprite((i + j) % 2 == 0? ResourceManager::getTexture(Texture::Title2Texture)
						: ResourceManager::getTexture(Texture::Title1Texture)));

				sprite->setPosition(BOARD_HORIZONTAL_DELTA + j * TILE_SIZE, HEADER_HEIGHT + BOARD_VERTICAL_DELTA + i * TILE_SIZE);

				m_gameBoardBackgroundTiles[i][j] = sprite;
			}
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
		for (auto& row : m_gameBoardBackgroundTiles) {
			for (auto& tile : row) {
				m_app->draw(*tile);
			}
		}
	}
}