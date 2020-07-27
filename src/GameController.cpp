#include "GameController.h"

#include "ResourceManager.h"
#include "GameConfig.h"
#include "Game.h"
#include "SFML/Graphics.hpp"

#include <iostream>
#include <cmath>

namespace match3
{
	bool GameController::initGame()
	{
		GameConfig config;
		try {
			if (!config.readJsonFile()) {
				std::cout << "Unable to read config.json file" << std::endl;
				return false;
			}
		}
		catch (const std::exception& ex) {
			std::cout << ex.what() << std::endl;
			return false;
		}

		if (!ResourceManager::getInstance().loadResources()) {
			std::cout << "Unable to load resources" << std::endl;
			return false;
		}

		m_game.reset(new Game(config.getBoardColumns(), config.getBoardRows(),
			config.getMovesCount(), config.getFiguresConfig(), config.getEnableBlockFigures()));
		m_app = m_game->getApp();

		return true;
	}

	void GameController::startGame()
	{
		int pressedX;
		int pressedY;
		int releasedX;
		int releasedY;

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
					int moveX = event.mouseMove.x;
					int moveY = event.mouseMove.y;

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
				std::cout << pressedX << " " << pressedY << " " << direction << std::endl;
				m_game->mouseMoveEvent(pressedX, pressedY, direction);
			}
			else if (released) {
				released = false;
				std::cout << pressedX << " " << pressedY << " " << std::endl;
				m_game->mouseClickEvent(pressedX, pressedY);
			}
			else {
				m_game->draw();
			}

			m_app->display();
		}
	}
}