/*
 * Austin Ramsay
 * austinramsay@gmail.com
 *
 * StoneBranch
 * Dream Candies File Tool
 *
 * parser.cpp
 *
 */

#include "parser.h"
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

/*
 * void strip_ends(string&, char)
 *
 * Provided a string reference, remove a given char from the front and back of the string 
 *
 * Returns: true if char was stripped from both ends, false if not
 */
bool Parser::strip_ends(std::string &token, const char to_strip) {

	// Verify the token is wrapped in requested character to be stripped at both ends
	if (token.front() == to_strip && token.back() == to_strip) {
		token.erase(0, 1);
		token.erase((token.length() - 1), 1);
		
		// Successfully removed from front and back
		return true;
	} else {
		// Symbols didn't exist at front and back, no action taken
		return false;
	}
}

/*
 * int split_tokens(string&, char, char, T)
 * 
 * Given an input string, a token delimiter and a string delimiter (optional), store tokens
 * in 'T result'. 
 *
 * 'tkn_delim' - token delimiter, decides how main tokens should be extracted
 * 'str_delim' - string delimiter, remove this delimiter wrapping the extracted token (or 
 *  null char '\0' for no action)
 *
 *  Returns: number of tokens extracted
 */
template <typename T> 
int Parser::split_tokens(const std::string &input, const char tkn_delim, const char str_delim, T result) {

	int count = 0;
	std::istringstream iss(input);
	std::string item;

	while (std::getline(iss, item, tkn_delim)) {
		if (str_delim != '\0') 
			strip_ends(item, str_delim);

		*result++ = item;
		count++;
	}

	return count;
}

/*
 * vector<string> split_tokens(string&, char, char)
 *
 * Wrapper function for 'void split_tokens' to store results into a vector using a back_inserter
 *
 * Returns: vector containing tokens extracted using given delimiters
 */
std::vector<std::string> Parser::split_tokens(const std::string &input, const char tkn_delim, const char str_delim) {

	std::vector<std::string> result;
	split_tokens(input, tkn_delim, str_delim, std::back_inserter(result));
	return result;
}

/*
 * int get_header_column(string&, string&)
 *
 * Given a header line, determine which token contains the requested header name
 *
 * Returns: header column number matching given name
 */
int Parser::get_header_column(const std::string &header, const std::string &search, const char tkn_delim, const char str_delim) {

	std::vector<std::string> tokens = split_tokens(header, tkn_delim, str_delim);	

	bool found = false;
	int pos = 0;

	// Iterate header tokens until match is found
	for (const auto &token : tokens) {
		if (token == search) {
			found = true;
			break;
		} else {
			pos++;
		}
	}

	// If a match was found, return the position. Otherwise return -1
	if (found) 
		return pos;
	else 
		return -1;
}
