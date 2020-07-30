#pragma once

#include "SFML/System/Vector2.hpp"

#include <memory>
#include <vector>
#include <list>

namespace match3
{
	enum class BombType {
		NoBomb = 0,
		HBomb = 1,
		VBomb = 2,
		RBomb = 3
	};

	struct Match
	{
		Match(const std::vector<sf::Vector2i> pat, const BombType ty)
			: pattern(pat)
			, type(ty)
		{

		}

		std::vector<sf::Vector2i> pattern;
		BombType type;
	};

	class FigureBase;
	class PatternManager
	{
	private:
		const std::vector<Match> MATCH_PRIORITY_LIST
		{
			Match({ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 0 } }, BombType::RBomb),
			Match({ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 } }, BombType::VBomb),
			Match({ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 } }, BombType::HBomb),
			Match({ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } }, BombType::VBomb),
			Match({ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }, BombType::HBomb),
			Match({ { 0, 0 }, { 0, 1 }, { 0, 2 } }, BombType::NoBomb),
			Match({ { 0, 0 }, { 1, 0 }, { 2, 0 } }, BombType::NoBomb)
		};

	public:
		bool checkMatch(std::list<sf::Vector2i> affectedCoords,
			std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures);

		std::list<sf::Vector2i> getMatchedCoords();
		std::list<std::pair<sf::Vector2i, std::shared_ptr<FigureBase>>> getBombs();

	private:
		std::list<sf::Vector2i> m_matchedCoords;
		std::list<std::pair<sf::Vector2i, std::shared_ptr<FigureBase>>> m_bombs;
	};
}