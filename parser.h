#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

class Parser {

	public:
		bool strip_ends(std::string&, const char);
		template <typename T> int split_tokens(const std::string&, const char, const char, T);
		std::vector<std::string> split_tokens(const std::string&, const char, const char);
		int get_header_column(const std::string&, const std::string&, const char, const char);
};

#endif
