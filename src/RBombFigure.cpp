#include "RBombFigure.h"

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	RBombFigure::RBombFigure()
		: BombFigureBase(std::shared_ptr<sf::Sprite>(
			new sf::Sprite(ResourceManager::getTexture(Texture::RBombTexture))))
	{

	}

	RBombFigure::~RBombFigure()
	{

	}

	std::set<std::shared_ptr<FigureBase>> RBombFigure::blow(std::vector<std::vector<std::shared_ptr<FigureBase>>>& gameBoardFigures
		, std::set<std::shared_ptr<FigureBase>> affectedFigures)
	{
		int32_t affectedRowIndex = getCoords().y;
		int32_t affectedColumnIndex = getCoords().x;

		for (int32_t i = affectedColumnIndex - 1; i <= affectedColumnIndex + 1; ++i) {
			std::string a = "asd";
			for (int32_t j = affectedRowIndex - 1; j <= affectedRowIndex + 1; ++j) {
				try
				{
					addToAffectedList(affectedFigures, gameBoardFigures.at(i).at(j), gameBoardFigures);
				}
				catch (const std::out_of_range&)
				{
					// Just skip this
				}
			}
		}

		for (auto& bomb : m_chainBlowBombs) {
			auto& subBombAffectedFigures = bomb->blow(gameBoardFigures, affectedFigures);
			affectedFigures.insert(subBombAffectedFigures.begin(), subBombAffectedFigures.end());
		}

		return affectedFigures;
	}
}