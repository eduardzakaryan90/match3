#include "FigureBase.h"

#include "ResourceManager.h"
#include "SFML/Graphics/Sprite.hpp"

namespace match3
{
	FigureBase::FigureBase(std::shared_ptr<sf::Sprite> sprite, FigureType type, bool canClick, bool canSwipe)
		: m_sprite(sprite)
		, m_type(type)
		, m_canClick(canClick)
		, m_canSwipe(canSwipe)
		, m_needToSpawn(false)
	{
		m_sprite->setScale(DEFAULT_SCALE, DEFAULT_SCALE);
	}

	FigureBase::~FigureBase()
	{

	}

	FigureType FigureBase::type()
	{
		return m_type;
	}

	bool FigureBase::canClick()
	{
		return m_canClick;
	}

	bool FigureBase::canSwipe()
	{
		return m_canSwipe;
	}

	void FigureBase::setCoords(size_t x, size_t y)
	{
		sf::Vector2i coordXY(static_cast<int32_t>(x), static_cast<int32_t>(y));
		setCoords(coordXY);
	}

	void FigureBase::setCoords(sf::Vector2i coordXY)
	{
		m_coords = coordXY;
	}

	std::shared_ptr<sf::Sprite> FigureBase::sprite()
	{
		return m_sprite;
	}

	sf::Vector2i FigureBase::getCoords()
	{
		return m_coords;
	}

	void FigureBase::setNeedToSpawn(bool needToSpawn)
	{
		if (needToSpawn) {
			sprite()->setScale(0.0f, 0.0f);
		}
		m_needToSpawn = needToSpawn;
	}

	bool FigureBase::isNeedToSpawn()
	{
		return m_needToSpawn;
	}
}