#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <stdexcept>

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			throw std::invalid_argument("Invalid arguments count\nUsage: lexer.exe <input file name> <output file name>\n");
		}

		std::ifstream input(argv[1]);
		if (!input.is_open())
		{
			throw std::exception("Failed to open file for reading\n");
		}

		std::ofstream output(argv[2]);
		if (!output.is_open())
		{
			throw std::exception("Failed to open file for writing\n");
		}

		CLexer lexer(input);
		lexer.PrintTokens(std::cout);
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}
