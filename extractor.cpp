/*
 * Austin Ramsay
 * austinramsay@gmail.com
 *
 * StoneBranch
 * Dream Candies File Tool
 *
 * main.cpp 
 *
 * This program takes in a file containing customer codes in a CSV format
 * with a header line consisting of the column description for the data that  
 * that is listed below it. For each customer code in the input file, the 
 * program will reference a 'customer.csv', 'invoice.csv' and 'invoice_item.csv'
 * in the same path to extract matching customer records.
 *
 */

#include "parser.h"
#include "io_handler.h"
#include <string>
#include <vector>
#include <iterator>
#include <iostream>
#include <sstream>

#define CUSTOMER_SAMPLE_PATH "CUSTOMER_SAMPLE.CSV"
#define CUSTOMER_TABLE_PATH "CUSTOMER.CSV"
#define INVOICE_TABLE_PATH "INVOICE.CSV"
#define INVOICE_ITEM_TABLE_PATH "INVOICE_ITEM.CSV"

int main() {

	Parser parser;
	IO_handler io_handle;
	
	std::vector<std::string> cust_codes = io_handle.read_lines(CUSTOMER_SAMPLE_PATH);
	std::vector<std::string> tokens;

	// Extract the pre-selected customer codes
	for (int i = 0; i < cust_codes.size(); i++) {

		// For each line of the customer sample table, extract the customer code token
		tokens = parser.split_tokens(cust_codes.at(i), ',', '"');

		for (int k = 0; k < tokens.size(); k++) {	
			// std::cout << tokens.at(k) << std::endl;

		}
	}
	io_handle.write_lines("testing.csv", cust_codes);

	return 0;
}
