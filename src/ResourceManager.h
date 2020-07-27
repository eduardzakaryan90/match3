#pragma once

#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Rect.hpp"

#include <string>

namespace match3
{
	enum Texture {
		RedTexture = 0,
		GreenTexture = 1,
		BlueTexture = 2,
		OrangeTexture = 3,
		VioletTexture = 4,
		HBombTexture = 20,
		VBombTexture = 21,
		RBombTexture = 22,
		BlockTexture = 40,
		Title1Texture = 60,
		Title2Texture = 61
	};

	enum Color {
		BackgroundColor = 0,
		Title1Color = 1,
		Title2Color = 2,
		OutlineColor = 3
	};

	enum Font {
		MainFont = 0
	};

	class ResourceManager
	{
	private:
		const std::string resourcesFolderName = "resources";

		const std::string RED_FIGURE_FILE_NAME = "red.png";
		const std::string GREEN_FIGURE_FILE_NAME = "green.png";
		const std::string BLUE_FIGURE_FILE_NAME = "blue.png";
		const std::string ORANGE_FIGURE_FILE_NAME = "orange.png";
		const std::string VIOLET_FIGURE_FILE_NAME = "violet.png";
		const std::string H_BOMB_FIGURE_FILE_NAME = "h_bomb.png";
		const std::string V_BOMB_FIGURE_FILE_NAME = "v_bomb.png";
		const std::string R_BOMB_FIGURE_FILE_NAME = "bomb.png";
		const std::string BLOCK_FIGURE_FILE_NAME = "block.png";
		const std::string TILE_1_FILE_NAME = "tile_1.png";
		const std::string TILE_2_FILE_NAME = "tile_2.png";

		const std::string FONT_FILE_NAME = "Ranchers-Regular.ttf";

	public:
		static ResourceManager& getInstance();

		bool loadResources();

		static sf::Texture& getTexture(Texture texture);

		static sf::Color& getColor(Color color);

		static sf::Font& getFont(Font font);

	private:
		ResourceManager() = default;
		~ResourceManager() = default;
		ResourceManager(const ResourceManager&) = delete;
		ResourceManager& operator=(const ResourceManager&) = delete;

	private:
		sf::Texture m_redFigureTexture;
		sf::Texture m_greenFigureTexture;
		sf::Texture m_blueFigureTexture;
		sf::Texture m_orangeFigureTexture;
		sf::Texture m_violetFIgureTexture;
		sf::Texture m_hBombTexture;
		sf::Texture m_vBombTexture;
		sf::Texture m_rBombTexture;
		sf::Texture m_blockFigure;
		sf::Texture m_tileTexture1;
		sf::Texture m_tileTexture2;

		sf::Color m_backgroundColor = sf::Color(50, 71, 76, 255);
		sf::Color m_titleColor1 = sf::Color(143, 143, 143, 255);
		sf::Color m_titleColor2 = sf::Color(100, 100, 100, 255);
		sf::Color m_titleOutlineColor = sf::Color(25, 25, 25, 255);

		sf::Font m_mainFont;

	};
}