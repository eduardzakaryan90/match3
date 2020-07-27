#include "GameConfig.h"

#include "rapidjson/document.h"

#include <fstream>
#include <experimental/filesystem>

namespace match3
{
	bool GameConfig::readJsonFile()
	{
		std::string path =  std::experimental::filesystem::current_path().append(JSON_FILE_NAME).string();
		std::ifstream jsonFile(path);
		if (!jsonFile.is_open()) {
			return false;
		}

		std::ostringstream ss;
		ss << jsonFile.rdbuf();
		
		rapidjson::Document root;
		root.Parse(ss.str().c_str());
		if (root.IsNull() || !root.IsObject()) {
			return false;
		}


		const char* enableBloackFiguresFieldName = ENABLE_BLOCK_FIGURES_FIELD_NAME.c_str();
		if (!root.HasMember(enableBloackFiguresFieldName)) {
			throw MissingConfigJsonFieldException(ENABLE_BLOCK_FIGURES_FIELD_NAME);
		}
		rapidjson::Value& enableBloackFigures = root[enableBloackFiguresFieldName];
		if (enableBloackFigures.IsNull() || !enableBloackFigures.IsBool()) {
			throw MissingConfigJsonFieldException(ENABLE_BLOCK_FIGURES_FIELD_NAME);
		}
		setEnableBlockFigures(enableBloackFigures.GetBool());


		const char* boardWidthFieldName = BOARD_COLUMNS_FIELD_NAME.c_str();
		if (!root.HasMember(boardWidthFieldName)) {
			throw MissingConfigJsonFieldException(BOARD_COLUMNS_FIELD_NAME);
		}
		rapidjson::Value& boardWidth = root[boardWidthFieldName];
		if (boardWidth.IsNull() || !boardWidth.IsInt()) {
			throw MissingConfigJsonFieldException(BOARD_COLUMNS_FIELD_NAME);
		}
		setBoardColumns(boardWidth.GetInt());


		const char* boardHeightFieldName = BOARD_ROWS_FIELD_NAME.c_str();
		if (!root.HasMember(boardHeightFieldName)) {
			throw MissingConfigJsonFieldException(BOARD_ROWS_FIELD_NAME);
		}
		rapidjson::Value& boardHeight = root[boardHeightFieldName];
		if (boardHeight.IsNull() || !boardHeight.IsInt()) {
			throw MissingConfigJsonFieldException(BOARD_ROWS_FIELD_NAME);
		}
		setBoardRows(boardHeight.GetInt());


		const char* movesCountFieldName = MOVES_COUNT_FIELD_NAME.c_str();
		if (!root.HasMember(movesCountFieldName)) {
			throw MissingConfigJsonFieldException(MOVES_COUNT_FIELD_NAME);
		}
		rapidjson::Value& movesCount = root[movesCountFieldName];
		if (movesCount.IsNull() || !movesCount.IsInt()) {
			throw MissingConfigJsonFieldException(MOVES_COUNT_FIELD_NAME);
		}
		setMovesCount(movesCount.GetInt());


		const char* figuresFieldName = FIGURE_LIST_FIELD_NAME.c_str();
		if (!root.HasMember(figuresFieldName)) {
			throw MissingConfigJsonFieldException(FIGURE_LIST_FIELD_NAME);
		}
		rapidjson::Value& figures = root[figuresFieldName];
		if (figures.IsNull() || !figures.IsArray()) {
			throw MissingConfigJsonFieldException(FIGURE_LIST_FIELD_NAME);
		}


		rapidjson::Document::Array& figuresArrayObj = figures.GetArray();
		for (rapidjson::SizeType i = 0; i < figuresArrayObj.Size(); i++) {
			rapidjson::Value& figure = figuresArrayObj[i];
			if (figure.IsNull() || !figure.IsObject()) {
				continue;
			}

			++m_figuresSet;

			const char* colorFieldName = FIGURE_COLOR_FIELD_NAME.c_str();
			if (!figure.HasMember(colorFieldName)) {
				throw MissingConfigJsonFieldException(FIGURE_COLOR_FIELD_NAME);
			}
			rapidjson::Value& colorName = figure[colorFieldName];
			if (colorName.IsNull() || !colorName.IsString()) {
				throw MissingConfigJsonFieldException(FIGURE_COLOR_FIELD_NAME);
			}
			std::string color = colorName.GetString();

			int32_t objectiveValue = 0;
			const char* objectiveFieldName = FIGURE_OBJECTIVE_FIELD_NAME.c_str();
			if (figure.HasMember(objectiveFieldName)) {
				rapidjson::Value& objective = figure[objectiveFieldName];
				if (objective.IsNull() || !objective.IsInt()) {
					throw MissingConfigJsonFieldException(FIGURE_OBJECTIVE_FIELD_NAME);
				}
				objectiveValue = objective.GetInt();
				++m_objectivesSet;
			}

			addToFiguresConfig(color, objectiveValue);
		}


		if (m_figuresSet < MIN_FIGURES_COUNT || m_figuresSet > MAX_FIGURES_COUNT) {
			throw WrongConfigJsonRuleException(FIGURE_LIST_FIELD_NAME,
				MIN_FIGURES_COUNT, MAX_FIGURES_COUNT, m_figuresSet);
		}
		if (m_objectivesSet < MIN_OBJECTIVES_COUNT || m_objectivesSet > MAX_OBJECTIVES_COUNT) {
			throw WrongConfigJsonRuleException(FIGURE_OBJECTIVE_FIELD_NAME,
				MIN_OBJECTIVES_COUNT, MAX_OBJECTIVES_COUNT, m_objectivesSet);
		}

		return true;
	}

	void GameConfig::setEnableBlockFigures(const bool enable)
	{
		m_enableBlockFigures = enable;
	}

	void GameConfig::setBoardColumns(const int32_t width)
	{
		if (width < MIN_COLUMNS || width > MAX_COLUMNS) {
			throw WrongConfigJsonRuleException(BOARD_COLUMNS_FIELD_NAME,
				MIN_COLUMNS, MAX_COLUMNS, width);
		}

		m_boardColumns = width;
	}

	void GameConfig::setBoardRows(const int32_t height)
	{
		if (height < MIN_ROWS || height > MAX_ROWS) {
			throw WrongConfigJsonRuleException(BOARD_ROWS_FIELD_NAME,
				MIN_ROWS, MAX_ROWS, height);
		}

		m_boardRows = height;
	}

	void GameConfig::setMovesCount(const int32_t movesCount)
	{
		if (movesCount < MIN_MOVES_COUNT || movesCount > MAX_MOVES_COUNT) {
			throw WrongConfigJsonRuleException(MOVES_COUNT_FIELD_NAME,
				MIN_MOVES_COUNT, MAX_MOVES_COUNT, movesCount);
		}

		m_movesCount = movesCount;
	}

	void GameConfig::addToFiguresConfig(std::string colorName, int32_t objective)
	{
		if (COLORS.erase(colorName) == 0) {
			throw InvalidOrDuplicateColorException(colorName);
		}
		
		if (objective < MIN_OBJECTIVE_TARGET || objective > MAX_OBJECTIVE_TARGET) {
			throw WrongConfigJsonRuleException(FIGURE_OBJECTIVE_FIELD_NAME,
				MIN_OBJECTIVE_TARGET, MAX_OBJECTIVE_TARGET, objective);
		}

		m_figuresConfig.push_back(std::pair<std::string, int32_t>(colorName, objective));
	}

	bool GameConfig::getEnableBlockFigures() const
	{
		return m_enableBlockFigures;
	}

	int32_t GameConfig::getBoardColumns() const
	{
		return m_boardColumns;
	}

	int32_t GameConfig::getBoardRows() const
	{
		return m_boardRows;
	}

	int32_t GameConfig::getMovesCount() const
	{
		return m_movesCount;
	}
	std::list<std::pair<std::string, int32_t>> GameConfig::getFiguresConfig() const
	{
		return m_figuresConfig;
	}
}