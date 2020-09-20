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
#define INVOICE_ITEM_EXTRACTED_PATH "INVOICE_ITEM_EXTRACTED.CSV"

// Expected parameters
#define PRESELECT_COUNT 1000
#define TKN_DELIM ','
#define STR_DELIM '"'

// CSV header labels
#define CCODE_HEADER "CUSTOMER_CODE"
#define ICODE_HEADER "INVOICE_CODE"

// Function prototypes
std::vector<std::string> extract_column(const std::string, const std::string);
std::vector<std::string> get_matching_entries(const std::string, const std::string, std::vector<std::string>&, const bool optimize);

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
	buffer = get_matching_entries(CUSTOMER_TABLE_PATH, CCODE_HEADER, cust_codes, true);
	io_handle.write_lines(CUSTOMER_EXTRACTED_PATH, buffer);
	buffer.clear();

	// Extract relevant entries from the INVOICE.CSV file
	buffer = get_matching_entries(INVOICE_TABLE_PATH, CCODE_HEADER, cust_codes, false);
	io_handle.write_lines(INVOICE_EXTRACTED_PATH, buffer);
	buffer.clear();

	// Extract invoice codes from INVOICE_EXTRACTED.CSV to prepare for extraction of relevant invoice items
	std::vector<std::string> inv_codes = extract_column(INVOICE_EXTRACTED_PATH, ICODE_HEADER);

	// Extract relevant invoice items using invoice codes from INVOICE_EXTRACTED.CSV
	buffer = get_matching_entries(INVOICE_ITEM_TABLE_PATH, ICODE_HEADER, inv_codes, false);
	io_handle.write_lines(INVOICE_ITEM_EXTRACTED_PATH, buffer);
	buffer.clear();

	return 0;
}

/*
 * vector<string> extract_column(string, string)
 *
 * Given a CSV file path and a specified column header, extract all tokens belonging to that header from the CSV content
 *
 * Returns: string vector containing relevant tokens
 */
std::vector<std::string> extract_column(const std::string path, const std::string column_header) {

	Parser parser;
        IO_handler io_handle;
        std::vector<std::string> tokens;

        // Pre-selected customer codes vector
        std::vector<std::string> raw_data = io_handle.read_lines(path);
        std::vector<std::string> extr_data;

	// Verify we extracted any information from the CSV file
	if (raw_data.empty()) {
		std::cout << "Warning: no entries read from '" << path << "'." << std::endl;
		return raw_data;
	}

        // Determine what column number the requested header is located in
	// Header expected at line 0
        int column_pos = parser.get_header_column(raw_data.at(0), column_header, TKN_DELIM, STR_DELIM);

        // For each line of the table, extract the requested token and push onto extracted data vector
        for (int i = 1; i < raw_data.size(); i++) {
                tokens = parser.split_tokens(raw_data.at(i), TKN_DELIM, STR_DELIM);
                extr_data.push_back(tokens.at(column_pos));
        }

	return extr_data;
}

/*
 * vector<string> get_matching_entries(string, string, vector<string>&, const bool)
 *
 * Given a CSV file path, a column header name, tokens to match to, and an optimization flag, 
 * extract full line entries where the line/column value matches one of the tokens provided to match to. 
 *
 * Optimization flag just allows the function to break from the inner loop to prevent full x*y loops when not necessary
 *
 * Returns: string vector containing full lines of a CSV file with a matching value to one of its columns
 */
std::vector<std::string> get_matching_entries(const std::string path, const std::string match_header, std::vector<std::string> &match_tokens, const bool optimize) {

	Parser parser;
	IO_handler io_handle;
	std::vector<std::string> buffer;
	std::vector<std::string> tokens;

	// Read lines of the specified CSV file into a vector
	std::vector<std::string> entries = io_handle.read_lines(path);
	
	// Verify we extracted any information from the CSV file
	if (entries.empty()) {
		std::cout << "Warning: no entries read from '" << path << "'." << std::endl;
		return entries;
	}

	// Determine column header position
	int column_pos = parser.get_header_column(entries.at(0), match_header, TKN_DELIM, STR_DELIM);

	// Insert header at beginning of buffer
	buffer.insert(buffer.begin(), entries.at(0));

	// Remove header from our raw data to avoid unnecessary loops
	entries.erase(entries.begin());

	// Iterate through the customer codes we're looking for
	for (const auto &search : match_tokens) {

		// Iterate through customer entries until finding a match
		for (const auto &line : entries) {
			tokens = parser.split_tokens(line, TKN_DELIM, STR_DELIM);

			// If customer code matches current search token, add to buffer and go to next iteration
			if (tokens.at(column_pos) == search) {
				buffer.push_back(line);
				if (optimize)
					break;
			}
		}
	}


	return buffer;
}
