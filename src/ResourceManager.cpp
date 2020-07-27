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

	sf::Texture & ResourceManager::getTexture(Textures texture)
	{
		ResourceManager& manager = getInstance();
		switch (texture)
		{
		case Textures::RedTexture:
			return manager.m_redFigureTexture;
		case Textures::GreenTexture:
			return manager.m_greenFigureTexture;
		case Textures::BlueTexture:
			return manager.m_blueFigureTexture;
		case Textures::OrangeTexture:
			return manager.m_orangeFigureTexture;
		case Textures::VioletTexture:
			return manager.m_violetFIgureTexture;
		case Textures::HBombTexture:
			return manager.m_hBombTexture;
		case Textures::VBombTexture:
			return manager.m_vBombTexture;
		case Textures::RBombTexture:
			return manager.m_rBombTexture;
		case Textures::BlockTexture:
			return manager.m_blockFigure;
		case Textures::Title1Texture:
			return manager.m_tileTexture1;
		case Textures::Title2Texture:
			return manager.m_tileTexture2;
		}
	}

	sf::Color & ResourceManager::getColor(Colors color)
	{
		ResourceManager& manager = getInstance();
		switch (color)
		{
		case Colors::BackgroundColor:
			return manager.m_backgroundColor;
		case Colors::Title1Color:
			return manager.m_titleColor1;
		case Colors::Title2Color:
			return manager.m_titleColor2;
		case Colors::OutlineColor:
			return manager.m_titleOutlineColor;
		}
	}

	sf::Font & ResourceManager::getFont(Fonts font)
	{
		ResourceManager& manager = getInstance();
		switch (font)
		{
		case Fonts::MainFont:
			return manager.m_mainFont;
		}
	}
}