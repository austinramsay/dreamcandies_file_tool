/*
 * test00
 *
 * Test the functionality of the get_header_column function
 * Should return a column number 0-n of the given string
 */

#include "parser.h"
#include <string>
#include <iostream>

int main() {

	Parser parser;
	int column[4];
	std::string test_header;
	
	// Create a test header: "CUSTOMER_CODE","FIRSTNAME","LASTNAME"
	test_header = "\"CUSTOMER_CODE\",\"FIRSTNAME\",\"LASTNAME\"";

	column[0] = parser.get_header_column(test_header, "CUSTOMER_CODE", ',', '"');
	column[1] = parser.get_header_column(test_header, "FIRSTNAME", ',', '"');
	column[2] = parser.get_header_column(test_header, "LASTNAME", ',', '"');
	column[3] = parser.get_header_column(test_header, "DOESNTEXIST", ',', '"');

	for (int i = 0; i < (sizeof(column) / sizeof(column[0])); i++) {
		if (column[i] == i) {
			std::cout << "test00: column " << i << " passed." << std::endl;
		} else {
			std::cout << "test00: column " << i << " failed." << std::endl;
		}
	}

	std::cout << "Expected results: pass, pass, pass, fail." << std::endl;
}

