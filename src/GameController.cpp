#include "GameController.h"

#include "ResourceManager.h"
#include "GameConfig.h"
#include "SFML/Graphics.hpp"

#include <cmath>

namespace match3
{
	bool GameController::initGame()
	{
		std::string errorString;
		GameConfig config;
		try {
			if (!ResourceManager::getInstance().loadResources()) {
				errorString == "Unable to load resources";
			}

			if (!config.readJsonFile()) {
				errorString = "Unable to read config.json file";
			}
		}
		catch (const std::exception& ex) {
			errorString = ex.what();
		}

		if (errorString.empty()) {
			m_game.reset(new Game(config.getBoardColumns(), config.getBoardRows(),
				config.getMovesCount(), config.getFiguresConfig(), config.getEnableBlockFigures()));
		}
		else {
			m_game.reset(new Game(errorString));
		}

		m_app = m_game->openGameWindow();

		return true;
	}

	void GameController::startGame()
	{
		int32_t pressedX;
		int32_t pressedY;
		int32_t releasedX;
		int32_t releasedY;

		bool pressed = false;
		bool moved = false;
		bool released = false;

		MouseMoveDirection direction;

		while (m_app->isOpen()) {
			m_app->clear(ResourceManager::getColor(Color::BackgroundColor));

			sf::Event event;
			while (m_app->pollEvent(event)) {
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
					pressed = true;
					pressedX = event.mouseButton.x;
					pressedY = event.mouseButton.y;
				}
				else if (pressed && event.type == sf::Event::MouseMoved) {
					int32_t moveX = event.mouseMove.x;
					int32_t moveY = event.mouseMove.y;

					if (std::abs(moveX - pressedX) > MOUSE_MOVE_DELTA) {
						pressed = false;
						moved = true;
						direction = (moveX > pressedX ? MouseMoveDirection::Right : MouseMoveDirection::Left);
					}
					else if (std::abs(moveY - pressedY) > MOUSE_MOVE_DELTA) {
						pressed = false;
						moved = true;
						direction = (moveY > pressedY ? MouseMoveDirection::Down : MouseMoveDirection::Up);
					}					
				}
				else if (pressed && event.type == sf::Event::MouseButtonReleased
					&& event.mouseButton.button == sf::Mouse::Button::Left) {
					pressed = false;
					released = true;

					releasedX = event.mouseButton.x;
					releasedY = event.mouseButton.y;
				}
				else if (event.type == sf::Event::Closed) {
					m_app->close();
				}
			}

			if (moved) {
				moved = false;
				m_game->mouseMoveEvent(pressedX, pressedY, direction);
			}
			else if (released) {
				released = false;
				m_game->mouseClickEvent(pressedX, pressedY);
			}
			else {
				m_game->draw();
			}

			m_app->display();
		}
	}
}