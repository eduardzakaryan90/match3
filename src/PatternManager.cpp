#include "PatternManager.h"

#include "HBombFigure.h"
#include "VBombFigure.h"
#include "RBombFigure.h"

namespace match3
{
	bool PatternManager::checkMatch(std::list<sf::Vector2i> affectedCoords, std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures)
	{
		bool hasMatch = false;
		m_matchedCoords.clear();
		m_bombs.clear();

		for (auto& affCoord : affectedCoords) {
			bool alreadyUsedCoord = false;
			for (auto& matchedCoord : m_matchedCoords) {
				if (affCoord == matchedCoord) {
					alreadyUsedCoord = true;
					break;
				}
			}
			if (alreadyUsedCoord) {
				continue;
			}

			auto& figure = gameBoardFigures[affCoord.x][affCoord.y];
			if (figure.get() == nullptr) {
				continue;
			}

			auto type = figure->type();
			if (type == FigureType::BombFigureType) {
				continue;
			}

			for (auto& match : MATCH_PRIORITY_LIST) {
				for (size_t i = 0; i < match.pattern.size(); ++i) {
					bool hasMatchCurrentPattern = true;
					std::list<sf::Vector2i> possibleMatchedCoords{ affCoord };
					for (size_t j = 0; j < match.pattern.size(); ++j) {
						if (i == j) {
							continue;
						}

						int32_t x = affCoord.x + match.pattern[j].x - match.pattern[i].x;
						int32_t y = affCoord.y + match.pattern[j].y - match.pattern[i].y;
						try
						{
							auto& checkFigure = gameBoardFigures.at(x).at(y);
							if (checkFigure.get() == nullptr || type != checkFigure->type()) {
								hasMatchCurrentPattern = false;
								break;
							}
							else {
								possibleMatchedCoords.push_back(sf::Vector2i(x, y));
							}
						}
						catch (const std::out_of_range&)
						{
							hasMatchCurrentPattern = false;
							break;
						}
					}

					if (hasMatchCurrentPattern) {
						hasMatch = true;

						if (match.type != BombType::NoBomb) {
							bool bombFound = false;
							for (auto& pair : m_bombs) {
								if (pair.first.x == affCoord.x && pair.first.y == affCoord.y) {
									bombFound = true;
								}
							}

							if (!bombFound) {
								std::shared_ptr<FigureBase> bomb;
								switch (match.type) {
								case BombType::HBomb:
									bomb.reset(new HBombFigure());
									break;
								case BombType::VBomb:
									bomb.reset(new VBombFigure());
									break;
								case BombType::RBomb:
									bomb.reset(new RBombFigure());
									break;
								}
								m_bombs.push_back(std::pair<sf::Vector2i, std::shared_ptr<FigureBase>>(affCoord, bomb));
							}
						}
						m_matchedCoords.insert(m_matchedCoords.end(), possibleMatchedCoords.begin(), possibleMatchedCoords.end());
						break;
					}
				}
			}
		}


		return hasMatch;
	}

	std::list<sf::Vector2i> PatternManager::getMatchedCoords()
	{
		return m_matchedCoords;
	}

	std::list<std::pair<sf::Vector2i, std::shared_ptr<FigureBase>>> PatternManager::getBombs()
	{
		return m_bombs;
	}
}