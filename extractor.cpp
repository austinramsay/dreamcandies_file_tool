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
#include <iostream>
#include <algorithm>

// Expected CSV paths - customer sample codes and full DB CSV extracts
#define CUSTOMER_SAMPLE_PATH "CUSTOMER_SAMPLE.CSV"
#define CUSTOMER_TABLE_PATH "CUSTOMER.CSV"
#define INVOICE_TABLE_PATH "INVOICE.CSV"
#define INVOICE_ITEM_TABLE_PATH "INVOICE_ITEM.CSV"

// Output CSV paths for compacted tables
#define CUSTOMER_EXTRACTED_PATH "CUSTOMER_EXTRACTED.CSV"
#define INVOICE_EXTRACTED_PATH "INVOICE_EXTRACTED.CSV"

// Expected parameters
#define PRESELECT_COUNT 1000
#define TKN_DELIM ','
#define STR_DELIM '"'

// CSV header labels
#define CCODE_HEADER "CUSTOMER_CODE"
#define ICODE_HEADER "INVOICE_CODE"

// Function prototypes
std::vector<std::string> extract_column(const std::string, const std::string);
std::vector<std::string> get_matching_entries(const std::string, const bool, std::vector<std::string>&);

/* 
 * Main driver function for DreamCandies File Tool
 */
int main() {

	IO_handler io_handle;
	std::vector<std::string> buffer;
		
	// Extract pre-selected customer codes using predefined expected parameters from specification
	std::vector<std::string> cust_codes = extract_column(CUSTOMER_SAMPLE_PATH, CCODE_HEADER);

	// Verify number of customer codes extracted
	if (cust_codes.size() > (PRESELECT_COUNT + 1)) {	// offset 1 for header compensation
		std::cout << "Warning: extracted customer code count is higher than expected." << std::endl;
        } else if (cust_codes.empty()) {
                std::cout << "Error: No pre-selected customer codes found (required)." << std::endl;
                return 1;
        }

	// Extract relevant entries from the CUSTOMER.CSV file
	buffer = get_matching_entries(CUSTOMER_TABLE_PATH, true, cust_codes);
	io_handle.write_lines(CUSTOMER_EXTRACTED_PATH, buffer);
	buffer.clear();

	// Extract relevant entries from the INVOICE.CSV file
	buffer = get_matching_entries(INVOICE_TABLE_PATH, false, cust_codes);
	io_handle.write_lines(INVOICE_EXTRACTED_PATH, buffer);

	return 0;
}

std::vector<std::string> extract_column(const std::string path, const std::string column_header) {

	Parser parser;
        IO_handler io_handle;
        std::vector<std::string> tokens;

        // Pre-selected customer codes vector
        std::vector<std::string> raw_data = io_handle.read_lines(path);
        std::vector<std::string> extr_data;

        // Determine what column number the requested header is located in
	// Header expected at line 0
        int column_pos = parser.get_header_column(raw_data.at(0), column_header, TKN_DELIM, STR_DELIM);

        // For each line of the table, extract the requested token
        for (int i = 1; i < raw_data.size(); i++) {

                tokens = parser.split_tokens(raw_data.at(i), TKN_DELIM, STR_DELIM);

                extr_data.push_back(tokens.at(column_pos));
        }

	return extr_data;
}

std::vector<std::string> get_matching_entries(const std::string path, const bool optimize, std::vector<std::string> &cust_codes) {

	Parser parser;
	IO_handler io_handle;
	std::vector<std::string> buffer;
	std::vector<std::string> tokens;
	std::vector<std::string> cust_code;

	// Read lines of the specified CSV file into a vector
	std::vector<std::string> cust_entries = io_handle.read_lines(path);
	
	// Verify we extracted any information from the CSV file
	if (cust_entries.empty()) {
		std::cout << "Warning: no entries read from customer table CSV." << std::endl;
		return cust_entries;
	}

	// Verify CUSTOMER_CODE header is position zero, if not set to correct position
	int ccode_column = parser.get_header_column(cust_entries.at(0), CCODE_HEADER, TKN_DELIM, STR_DELIM);
	if (ccode_column != 0) {
		std::cout << "Warning: expected header at positon 0 not found in customer DB table CSV." << std::endl;
		std::cout << "Setting CUSTOMER_CODE header at position '" << ccode_column << "'." << std::endl;
	}

	// Iterate through the customer codes we're looking for
	for (const auto &search : cust_codes) {

		// Iterate through customer entries until finding a match
		for (const auto &line : cust_entries) {
			tokens = parser.split_tokens(line, TKN_DELIM, STR_DELIM);

			// If customer code matches current search token, add to buffer and go to next iteration
			if (tokens.at(ccode_column) == search) {
				buffer.push_back(line);
				if (optimize)
					break;
			}
		}
	}

	return buffer;
}
