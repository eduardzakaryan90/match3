#include "VBombFigure.h"

#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	VBombFigure::VBombFigure()
		: BombFigureBase(std::shared_ptr<sf::Sprite>(
			new sf::Sprite(ResourceManager::getTexture(Texture::VBombTexture))))
	{

	}

	VBombFigure::~VBombFigure()
	{

	}

	std::set<std::shared_ptr<FigureBase>> VBombFigure::blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
		, std::set<std::shared_ptr<FigureBase>> affectedFigures)
	{
		int32_t affectedColumnIndex = getCoords().x;

		for (size_t i = 0; i < gameBoardFigures.size(); ++i) {
			if (affectedColumnIndex != i) {
				continue;
			}
			for (size_t j = 0; j < gameBoardFigures[i].size(); ++j) {
				addToAffectedList(affectedFigures, gameBoardFigures[i][j], gameBoardFigures);
			}
			break;
		}

		for (auto bomb : m_chainBlowBombs) {
			auto subBombAffectedFigures = bomb->blow(gameBoardFigures, affectedFigures);
			affectedFigures.insert(subBombAffectedFigures.begin(), subBombAffectedFigures.end());
		}

		return affectedFigures;
	}
}