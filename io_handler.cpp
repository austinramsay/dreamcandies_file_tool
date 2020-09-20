/*
 * Austin Ramsay
 * austinramsay@gmail.com
 *
 * StoneBranch
 * Dream Candies File Tool
 *
 * io_handler.cpp
 *
 */

#include "io_handler.h"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/*
 * vector<string> read_lines(string&) {
 *
 * Given a file name, read the file and return a vector with the lines of the file as its elements
 *
 * Returns: string vector of file content
 */
std::vector<std::string> IO_handler::read_lines(const std::string& filename) {

	std::ifstream file_in;
	std::vector<std::string> lines;
	std::string cur_line;

	file_in.open(filename);

	// If file opened, iterate each line and push each one onto the 'lines' vector
	if (file_in.is_open()) {
		while (file_in) {
			if (getline(file_in, cur_line))
				lines.push_back(cur_line);
		}
	} else {
		std::cout << "File '" << filename << "' failed to open." << std::endl;
	}

	file_in.close();

	return lines;
}

/*
 * int write_lines(string&, vector<string>&) 
 *
 * Given a file name and a vector of strings, write a file with each line extracted from the vector
 *
 * Returns: count of lines written
 */
int IO_handler::write_lines(const std::string& filename, const std::vector<std::string> &lines) {

	int count = 0;
	std::ofstream file_out;

	file_out.open(filename);

	// If file opened, iterate through the vector strings and write each one to the file
	if (file_out.is_open()) {
		for (const auto &str : lines) {
			file_out << str << "\n";
			count++;
		}
		std::cout << "Wrote '" << count << "' lines to file: '" << filename << "'." << std::endl;
	} else {
		std::cout << "File '" << filename << "' failed to open." << std::endl;
	}

	file_out.close();

	return count;
}
