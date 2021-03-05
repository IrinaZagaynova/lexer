#pragma once
#include <set>
#include <string>
#include <map>

const std::set<std::string> KEYWORDS
{
	"main",
	"void",
	"int",
	"float",
	"char",
	"string",
	"bool",
	"if",
	"else",
	"return",
	"while",
	"for",
	"true",
	"false"
};

const std::set<char> BRACKETS
{
	'(', ')', '{', '}'
};

const std::set<char> ARRAY_BRACKETS
{
	'[', ']'
};

const std::set<char> SEPARATORS
{
	' ', ';', ',', ':', '\t', '\n'
};

const std::set<char> IGNORED_SEPARATORS
{
	' ', '\t', '\n'
};

const std::map<TokenType, std::string> TOKEN_NAMES
{
	{ TokenType::Identifier, "Identifier" },
	{ TokenType::Keyword, "Keyword" },
	{ TokenType::Int, "Int" },
	{ TokenType::Float, "Float" },
	{ TokenType::Binary, "Binary" },
	{ TokenType::Octal, "Octal" },
	{ TokenType::Hex, "Hex" },
	{ TokenType::Char, "Char" },
	{ TokenType::String, "String" },
	{ TokenType::Array, "Array" },
	{ TokenType::Addition, "Addition" },
	{ TokenType::Subtraction, "Subtraction" },
	{ TokenType::Multiplication, "Multiplication" },
	{ TokenType::Division, "Division" },
	{ TokenType::Comparison, "Comparison" },
	{ TokenType::Assignment, "Assignment" },
	{ TokenType::Separator, "Separator" },
	{ TokenType::Bracket, "Bracket" },
	{ TokenType::LogicalOperator, "LogicalOperator" },
	{ TokenType::NegationOperator, "NegationOperator" },
	{ TokenType::Error, "Error" },
	{ TokenType::EOFL, "EOFL"}
};