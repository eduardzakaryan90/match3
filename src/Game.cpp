#include "Game.h"

#include "ResourceManager.h"
#include "SFML/Graphics.hpp"

namespace match3
{
	Game::Game(int32_t columns, int32_t rows, int32_t movesCount, std::list<std::pair<std::string, int32_t>> figuresConfig, bool enableBlockFigures)
		: m_enableBlockFigures(enableBlockFigures)
		, m_movesCount(movesCount)
		, m_gameState(GameState::Active)
	{
		m_windowWidth = static_cast<uint32_t>(columns * TILE_SIZE + 2 * BOARD_HORIZONTAL_DELTA);
		m_windowHeight = static_cast<uint32_t>(rows * TILE_SIZE + 2 * BOARD_VERTICAL_DELTA + HEADER_HEIGHT);

		createObjectiveDrawables(figuresConfig);

		createHeaderDrawables();
		createGameBoardBackgroundTiles(columns, rows);
		
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
			drawHeader();
			drawGameBoard();
		}
		else {
			m_app->draw(*m_gameMessageText);
		}
	}

	void Game::mouseMoveEvent(int32_t X, int32_t Y, MouseMoveDirection direction)
	{
		if (m_gameState == GameState::Active) {
			// TODO: handle mouse move
		}
		draw();
	}

	void Game::mouseClickEvent(int32_t X, int32_t Y)
	{
		if (m_gameState == GameState::Active) {
			// TODO: handle mouse click
		}
		draw();
	}

#pragma warning( push )
#pragma warning( disable : 4715)
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
#pragma warning( pop )

	std::shared_ptr<sf::Sprite> Game::createSpriteFromColorFigureTpe(BoardColorFigureType figureTypes)
	{
		std::shared_ptr<sf::Sprite> sprite;
		switch (figureTypes)
		{
		case match3::RedFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::RedTexture)));
			break;
		case match3::GreenFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::GreenTexture)));
			break;
		case match3::BlueFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::BlueTexture)));
			break;
		case match3::OrangeFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::OrangeTexture)));
			break;
		case match3::VioletFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::VioletTexture)));
			break;
		}
		sprite->setScale(0.6f, 0.6f);
		return sprite;
	}

	void Game::createObjectiveDrawables(std::list<std::pair<std::string, int32_t>> figuresConfig)
	{
		size_t i = 0;
		for (auto config : figuresConfig) {
			BoardColorFigureType figureType = getBoardColorFigureTypesFromColorName(config.first);
			int32_t objectiveTarget = config.second;

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

	void Game::updateObjectiveTarget(BoardColorFigureType type, int32_t decrementValue)
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

	void Game::createGameBoardBackgroundTiles(int32_t columns, int32_t rows)
	{
		m_gameBoardRectangle.reset(new sf::RectangleShape());
		m_gameBoardRectangle->setSize(sf::Vector2f(TILE_SIZE * columns, TILE_SIZE * rows));
		m_gameBoardRectangle->setFillColor(ResourceManager::getColor(Color::Title2Color));
		m_gameBoardRectangle->setOutlineColor(ResourceManager::getColor(Color::OutlineColor));
		m_gameBoardRectangle->setOutlineThickness(3.0f);
		m_gameBoardRectangle->setPosition(BOARD_HORIZONTAL_DELTA, HEADER_HEIGHT + BOARD_VERTICAL_DELTA);
		
		for (size_t i = 0; i < rows; ++i) {
			for (size_t j = 0; j < columns; ++j) {
				if ((i + j) % 2 == 0) {
					continue;
				}
				std::shared_ptr<sf::Sprite> sprite(new sf::Sprite(ResourceManager::getTexture(Texture::Title1Texture)));

				sprite->setPosition(BOARD_HORIZONTAL_DELTA + j * TILE_SIZE, HEADER_HEIGHT + BOARD_VERTICAL_DELTA + i * TILE_SIZE);

				m_gameBoardBackgroundTiles.push_back(sprite);
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
		m_app->draw(*m_gameBoardRectangle);
		for (auto tile : m_gameBoardBackgroundTiles) {
			m_app->draw(*tile);
		}
	}
}