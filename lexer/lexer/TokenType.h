#pragma once

enum class TokenType
{
	Identifier,
	Keyword,
	Int,
	Float,
	Binary,
	Hex,
	Char,
	String,
	Array,
	Addition,
	Subtraction,
	Multiplication,
	Division,
	Comparison,
	Assignment,
	Separator,
	Bracket,
	LogicalOperator,
	NegationOperator,
	Error
};