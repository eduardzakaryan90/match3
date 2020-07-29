#include "ColorFigure.h"

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

namespace match3
{
	ColorFigure::ColorFigure(FigureType figureTypes)
		: FigureBase(createSpriteFromColorFigureTpe(figureTypes), figureTypes, false, true)
	{

	}

	ColorFigure::~ColorFigure()
	{

	}

	std::shared_ptr<sf::Sprite> ColorFigure::createSpriteFromColorFigureTpe(FigureType figureTypes)
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
}