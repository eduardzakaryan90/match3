#include "ColorFigure.h"

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace match3
{
	ColorFigure::ColorFigure(FigureType figureTypes)
		: FigureBase(createSpriteFromColorFigureType(figureTypes), figureTypes, false, true)
	{

	}

	ColorFigure::~ColorFigure()
	{

	}

	std::shared_ptr<sf::Sprite> ColorFigure::createSpriteFromColorFigureType(FigureType figureTypes)
	{
		std::shared_ptr<sf::Sprite> sprite;
		switch (figureTypes)
		{
		case FigureType::RedFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::RedTexture)));
			break;
		case FigureType::GreenFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::GreenTexture)));
			break;
		case FigureType::BlueFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::BlueTexture)));
			break;
		case FigureType::OrangeFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::OrangeTexture)));
			break;
		case FigureType::VioletFigureType:
			sprite.reset(new sf::Sprite(ResourceManager::getTexture(Texture::VioletTexture)));
			break;
		}
		return sprite;
	}
}