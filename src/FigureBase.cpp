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
	{
		m_sprite->setScale(0.5f, 0.5f);
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

	void FigureBase::setCoordX(size_t x)
	{
		m_coordX = x;
	}

	void FigureBase::setCoordY(size_t y)
	{
		m_coordY = y;
	}

	std::shared_ptr<sf::Sprite> FigureBase::sprite()
	{
		return m_sprite;
	}

	size_t FigureBase::getCoordX()
	{
		return m_coordX;
	}

	size_t FigureBase::getCoordY()
	{
		return m_coordY;
	}
}