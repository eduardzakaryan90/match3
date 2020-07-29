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
		const int32_t MIN_COLUMNS = 7;
		const int32_t MAX_COLUMNS = 10;
		const int32_t MIN_ROWS = 7;
		const int32_t MAX_ROWS = 10;
		const int32_t MIN_MOVES_COUNT = 1;
		const int32_t MAX_MOVES_COUNT = 999;
		const int32_t MIN_OBJECTIVE_TARGET = 0;
		const int32_t MAX_OBJECTIVE_TARGET = 999;
		const int32_t MIN_FIGURES_COUNT = 3;
		const int32_t MAX_FIGURES_COUNT = 5;
		const int32_t MIN_OBJECTIVES_COUNT = 1;
		const int32_t MAX_OBJECTIVES_COUNT = 3;

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
		void setBoardColumns(const int32_t width);
		void setBoardRows(const int32_t height);
		void setMovesCount(const int32_t movesCount);
		void addToFiguresConfig(std::string colorName, int32_t objective);

		// getters
		int32_t getBoardColumns() const;
		int32_t getBoardRows() const;
		int32_t getMovesCount() const;
		std::list<std::pair<std::string, int32_t>> getFiguresConfig() const;

	private:
		int32_t m_boardColumns;
		int32_t m_boardRows;
		int32_t m_movesCount;
		std::list<std::pair<std::string, int32_t>> m_figuresConfig; // key - figure color name, value - objective. If value is 0 - no objective set.

		int32_t m_figuresSet = 0;
		int32_t m_objectivesSet = 0;
	};

	struct WrongConfigJsonRuleException : public std::exception
	{
		const std::string m_field;
		const int32_t m_minValue;
		const int32_t m_maxValue;
		const int32_t m_actualValue;

		mutable std::string errorString;

		explicit WrongConfigJsonRuleException(const std::string field, const int32_t minValue, const int32_t maxValue, const int32_t actualValue)
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