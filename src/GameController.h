#pragma once

#include <memory>

namespace sf {
	class RenderWindow;
}

namespace match3
{
	class Game;
	class GameController
	{
	private:
		const int MOUSE_MOVE_DELTA = 10;

	public:
		bool initGame();
		void startGame();

	private:
		std::auto_ptr<Game> m_game;
		std::shared_ptr<sf::RenderWindow> m_app;
	};
}