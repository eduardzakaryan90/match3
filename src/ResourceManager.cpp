#include "ResourceManager.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
namespace match3
{
	ResourceManager & ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	bool ResourceManager::loadResources()
	{
		// get root resource path
		fs::path resourcePath = fs::current_path().append(resourcesFolderName);

		// load textures
		if (!m_redFigureTexture.loadFromFile(fs::path(resourcePath).append(RED_FIGURE_FILE_NAME).string())
			|| !m_greenFigureTexture.loadFromFile(fs::path(resourcePath).append(GREEN_FIGURE_FILE_NAME).string())
			|| !m_blueFigureTexture.loadFromFile(fs::path(resourcePath).append(BLUE_FIGURE_FILE_NAME).string())
			|| !m_orangeFigureTexture.loadFromFile(fs::path(resourcePath).append(ORANGE_FIGURE_FILE_NAME).string())
			|| !m_violetFIgureTexture.loadFromFile(fs::path(resourcePath).append(VIOLET_FIGURE_FILE_NAME).string())
			|| !m_hBombTexture.loadFromFile(fs::path(resourcePath).append(H_BOMB_FIGURE_FILE_NAME).string())
			|| !m_vBombTexture.loadFromFile(fs::path(resourcePath).append(V_BOMB_FIGURE_FILE_NAME).string())
			|| !m_rBombTexture.loadFromFile(fs::path(resourcePath).append(R_BOMB_FIGURE_FILE_NAME).string())
			|| !m_blockFigure.loadFromFile(fs::path(resourcePath).append(BLOCK_FIGURE_FILE_NAME).string())
			|| !m_tileTexture1.loadFromFile(fs::path(resourcePath).append(TILE_1_FILE_NAME).string())
			|| !m_tileTexture2.loadFromFile(fs::path(resourcePath).append(TILE_2_FILE_NAME).string())) {
			return false;
		}

		// load fonts
		if (!m_mainFont.loadFromFile(fs::path(resourcePath).append(FONT_FILE_NAME).string())) {
			return false;
		}

		return true;
	}

	sf::Texture & ResourceManager::getTexture(Texture texture)
	{
		ResourceManager& manager = getInstance();
		switch (texture)
		{
		case Texture::RedTexture:
			return manager.m_redFigureTexture;
		case Texture::GreenTexture:
			return manager.m_greenFigureTexture;
		case Texture::BlueTexture:
			return manager.m_blueFigureTexture;
		case Texture::OrangeTexture:
			return manager.m_orangeFigureTexture;
		case Texture::VioletTexture:
			return manager.m_violetFIgureTexture;
		case Texture::HBombTexture:
			return manager.m_hBombTexture;
		case Texture::VBombTexture:
			return manager.m_vBombTexture;
		case Texture::RBombTexture:
			return manager.m_rBombTexture;
		case Texture::BlockTexture:
			return manager.m_blockFigure;
		case Texture::Title1Texture:
			return manager.m_tileTexture1;
		case Texture::Title2Texture:
			return manager.m_tileTexture2;
		}
	}

	sf::Color & ResourceManager::getColor(Color color)
	{
		ResourceManager& manager = getInstance();
		switch (color)
		{
		case Color::BackgroundColor:
			return manager.m_backgroundColor;
		case Color::Title1Color:
			return manager.m_titleColor1;
		case Color::Title2Color:
			return manager.m_titleColor2;
		case Color::OutlineColor:
			return manager.m_titleOutlineColor;
		}
	}

	sf::Font & ResourceManager::getFont(Font font)
	{
		ResourceManager& manager = getInstance();
		switch (font)
		{
		case Font::MainFont:
			return manager.m_mainFont;
		}
	}
}