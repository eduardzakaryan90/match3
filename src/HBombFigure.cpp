#include "HBombFigure.h"

#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	HBombFigure::HBombFigure()
		: BombFigureBase(std::shared_ptr<sf::Sprite>(
			new sf::Sprite(ResourceManager::getTexture(Texture::HBombTexture))))
	{

	}

	HBombFigure::~HBombFigure()
	{

	}

	std::set<std::shared_ptr<FigureBase>> HBombFigure::blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
		, std::set<std::shared_ptr<FigureBase>> affectedFigures)
	{
		int32_t affectedRowIndex = getCoords().y;

		for (size_t i = 0; i < gameBoardFigures.size(); ++i) {
			for (size_t j = 0; j < gameBoardFigures[i].size(); ++j) {
				if (affectedRowIndex != j) {
					continue;
				}
				addToAffectedList(affectedFigures, gameBoardFigures[i][j], gameBoardFigures);
			}
		}

		for (auto bomb : m_chainBlowBombs) {
			auto subBombAffectedFigures = bomb->blow(gameBoardFigures, affectedFigures);
			affectedFigures.insert(subBombAffectedFigures.begin(), subBombAffectedFigures.end());
		}

		return affectedFigures;
	}
}