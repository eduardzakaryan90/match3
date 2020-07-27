#pragma once

#include <sstream>
#include <string>
#include <list>
#include <set>

namespace match3
{
	class GameConfig
	{
	private:
		const int MIN_COLUMNS = 7;
		const int MAX_COLUMNS = 10;
		const int MIN_ROWS = 7;
		const int MAX_ROWS = 10;
		const int MIN_MOVES_COUNT = 1;
		const int MAX_MOVES_COUNT = 999;
		const int MIN_OBJECTIVE_TARGET = 0;
		const int MAX_OBJECTIVE_TARGET = 999;
		const int MIN_FIGURES_COUNT = 3;
		const int MAX_FIGURES_COUNT = 5;
		const int MIN_OBJECTIVES_COUNT = 1;
		const int MAX_OBJECTIVES_COUNT = 3;

		const std::string ENABLE_BLOCK_FIGURES_FIELD_NAME = "enableBloackFigures";
		const std::string BOARD_COLUMNS_FIELD_NAME = "boardColumns";
		const std::string BOARD_ROWS_FIELD_NAME = "boardRows";
		const std::string MOVES_COUNT_FIELD_NAME = "movesCount";
		const std::string FIGURE_LIST_FIELD_NAME = "figures";
		const std::string FIGURE_COLOR_FIELD_NAME = "color";
		const std::string FIGURE_OBJECTIVE_FIELD_NAME = "objective";

		const std::string JSON_FILE_NAME = "config.json";

		std::set<std::string> COLORS{ "red", "green", "blue", "orange", "violet" };

	public:
		bool readJsonFile();

		// setters
		void setEnableBlockFigures(const bool enable);
		void setBoardColumns(const int width);
		void setBoardRows(const int height);
		void setMovesCount(const int movesCount);
		void addToFiguresConfig(std::string colorName, int objective);

		// getters
		bool getEnableBlockFigures() const;
		int getBoardColumns() const;
		int getBoardRows() const;
		int getMovesCount() const;
		std::list<std::pair<std::string, int>> getFiguresConfig() const;

	private:
		bool m_enableBlockFigures;
		int m_boardColumns;
		int m_boardRows;
		int m_movesCount;
		std::list<std::pair<std::string, int>> m_figuresConfig; // key - figure color name, value - objective. If value is 0 - no objective set.

		int m_figuresSet = 0;
		int m_objectivesSet = 0;
	};

	struct WrongConfigJsonRuleException : public std::exception
	{
		const std::string m_field;
		const int m_minValue;
		const int m_maxValue;
		const int m_actualValue;

		mutable std::string errorString;

		explicit WrongConfigJsonRuleException(const std::string field, const int minValue, const int maxValue, const int actualValue)
			: std::exception()
			, m_field(field)
			, m_minValue(minValue)
			, m_maxValue(maxValue)
			, m_actualValue(actualValue)
		{

		}

		virtual ~WrongConfigJsonRuleException() throw () override {}

		virtual const char * what() const throw () override
		{
			std::stringstream ss;
			ss << "Out of config JSON rules, field name: " << m_field << ". Min value is: " << m_minValue
				<< ", max value is: " << m_maxValue << ", but actual value was: " << m_actualValue;
			errorString = ss.str();
			return errorString.c_str();
		}
	};

	struct MissingConfigJsonFieldException : public std::exception
	{
		const std::string m_field;

		mutable std::string errorString;

		explicit MissingConfigJsonFieldException(const std::string field)
			: std::exception()
			, m_field(field)
		{

		}

		virtual ~MissingConfigJsonFieldException() throw () override {}

		virtual const char * what() const throw () override
		{
			std::stringstream ss;
			ss << "Field is missing or has wrong type, field name: " << m_field;
			errorString = ss.str();
			return errorString.c_str();
		}
	};

	struct InvalidOrDuplicateColorException : public std::exception
	{
		const std::string m_colorName;

		mutable std::string errorString;

		explicit InvalidOrDuplicateColorException(const std::string colorName)
			: std::exception()
			, m_colorName(colorName)
		{

		}

		virtual ~InvalidOrDuplicateColorException() throw () override {}

		virtual const char * what() const throw () override
		{
			std::stringstream ss;
			ss << "Invalid or duplicate color name: " << m_colorName;
			errorString = ss.str();
			return errorString.c_str();
		}
	};
}