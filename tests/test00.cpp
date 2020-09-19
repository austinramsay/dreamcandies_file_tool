#include "parser.h"
#include <string>
#include <iostream>

int main() {

	Parser parser;
	int column[3];
	std::string test_header;
	
	// Create a test header: "CUSTOMER_CODE","FIRSTNAME","LASTNAME"
	test_header = "\"CUSTOMER_CODE\",\"FIRSTNAME\",\"LASTNAME\"";

	column[0] = parser.get_header_column(test_header, "CUSTOMER_CODE", ',', '"');
	column[1] = parser.get_header_column(test_header, "FIRSTNAME", ',', '"');
	column[2] = parser.get_header_column(test_header, "LASTNAME", ',', '"');

	for (int i = 0; i < (sizeof(column) / sizeof(column[0])); i++) {
		if (column[i] == i) {
			std::cout << "test00: column " << i << " passed." << std::endl;
		} else {
			std::cout << "test00: column " << i << " failed." << std::endl;
		}
	}
}

