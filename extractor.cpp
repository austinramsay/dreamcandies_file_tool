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

// Expected parameters
#define PRESELECT_COUNT 1000
#define TKN_DELIM ','
#define STR_DELIM '"'

// Customer sample CSV header labels
#define CS_CCODE_HEADER "CUSTOMER_CODE"

// Customer table CSV header labels
#define CT_CCODE_HEADER "CUSTOMER_CODE"

// Function prototypes
std::vector<std::string> get_cust_codes();
std::vector<std::string> get_cust_entries(std::vector<std::string>&);

/* 
 * Main driver function for DreamCandies File Tool
 */
int main() {

	IO_handler io_handle;
	std::vector<std::string> buffer;
		
	// Extract pre-selected customer codes using predefined expected parameters from specification
	std::vector<std::string> cust_codes = get_cust_codes();
	if (cust_codes.size() == 0) {
		std::cout << "No customer codes were read. Exiting." << std::endl;
		return 1;
	}

	// Extract relevant entries from the CUSTOMER.CSV file
	buffer = get_cust_entries(cust_codes);
	io_handle.write_lines(CUSTOMER_EXTRACTED_PATH, buffer);

	return 0;
}

std::vector<std::string> get_cust_codes() {

	Parser parser;
        IO_handler io_handle;
        std::vector<std::string> tokens;

        // Begin extracting pre-selected customer codes

        // Pre-selected customer codes vector
        std::vector<std::string> raw_cust_codes = io_handle.read_lines(CUSTOMER_SAMPLE_PATH);
        std::vector<std::string> cust_codes;
        std::string cust_code;

        // Specified pre-selected customers is 1000 plus header line
        if (raw_cust_codes.size() > (PRESELECT_COUNT + 1)) {
                std::cout << "Warning: extracted customer code count is higher than expected." << std::endl;
        } else if (raw_cust_codes.empty()) {
                std::cout << "Error: No pre-selected customer codes found (required)." << std::endl;
                return cust_codes;
        }

        // Verify CUSTOMER_CODE header is position zero, if not set to correct position
        int ccode_column = parser.get_header_column(raw_cust_codes.at(0), CS_CCODE_HEADER, TKN_DELIM, STR_DELIM);
        if (ccode_column != 0) {
                std::cout << "Warning: expected header at position 0 not found in customer code CSV." << std::endl;
                std::cout << "Setting CUSTOMER_CODE header at position '" << ccode_column << "'." << std::endl;
        }

        // For each line of the customer sample table, extract the customer code token
        for (int i = 1; i < raw_cust_codes.size(); i++) {

                tokens = parser.split_tokens(raw_cust_codes.at(i), TKN_DELIM, STR_DELIM);

                cust_code = tokens.at(ccode_column);
                cust_codes.push_back(cust_code);
        }

	return cust_codes;
}

std::vector<std::string> get_cust_entries(std::vector<std::string> &cust_codes) {

	Parser parser;
	IO_handler io_handle;
	std::vector<std::string> buffer;
	std::vector<std::string> tokens;
	std::vector<std::string> cust_code;

	// Read lines of the CUSTOMER.CSV file into a vector
	std::vector<std::string> cust_entries = io_handle.read_lines(CUSTOMER_TABLE_PATH);
	
	// Verify we extracted any information from the CSV file
	if (cust_entries.empty()) {
		std::cout << "Warning: no entries read from customer table CSV." << std::endl;
		return cust_entries;
	}

	// Verify CUSTOMER_CODE header is position zero, if not set to correct position
	int ccode_column = parser.get_header_column(cust_entries.at(0), CT_CCODE_HEADER, TKN_DELIM, STR_DELIM);
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
				break;
			}
		}
	}

	return buffer;
}
