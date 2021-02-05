#pragma once
#include "TokenType.h"
#include <string>

struct Token
{
	TokenType type;
	std::string value;
	size_t line = 0;
	size_t column = 0;
};