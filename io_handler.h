#ifndef IO_HANDLER_H_
#define IO_HANDLER_H

#include <vector>
#include <string>

class IO_handler {

	public:
		std::vector<std::string> read_lines(const std::string&);
		int write_lines(const std::string&, const std::vector<std::string>&);
		// template <typename iotype> open(const std::string&, iotype);
};
#endif
