#pragma once
#include "State.h"
#include "TokenType.h"
#include "Token.h"
#include <iostream>
#include <string>
#include <vector>
#include <optional>

class CLexer
{
public:
	CLexer(std::istream& istrm);

	std::vector<Token> GetTokens() const;
	void PrintTokens(std::ostream& ostrm) const;

private:
	void ProcessStream();
	void ProcessChar(char ch);
	void CheckLineEnd();
	void ProcessCharWhenDefaultState(char ch);
	void ProcessCharInMultiLineComment(char ch);
	void ProcessCharInQuotes(char ch);
	void ProcessCharInApostrophes(char ch);
	void ProcessCharInSquareBrackets(char ch);
	void ProcessCharInExponentialNotation(char ch);

	std::optional<TokenType> GetTokenTypeByChar(char ch);
	std::optional<TokenType> GetTokenTypeByCurrentLexeme();

	void AddToken(TokenType type);
	bool AddTokenByCurrentLexeme();
	void FlushLexeme();

	bool IsSeparator(char ch) const;
	bool IsBracket(char ch) const;
	bool IsIgnoredSeparator(char ch) const;
	bool IsCurrentLexemeKeyword() const;
	bool IsCurrentLexemeBinaryNumber() const;
	bool IsCurrentLexemeOctalNumber() const;
	bool IsCurrentLexemeHexNumber() const;
	bool IsCurrentLexemeInt() const;
	bool IsCurrentLexemeFloat() const;
	bool IsCurrentLexemeIdentifier() const;
	bool IsCurrentLexemeComparison() const;
	bool IsCurrentLexemeLogicalOperator() const;
	bool IsQuotesStart(char ch);
	bool IsApostrophesStart(char ch);
	bool IsSquareBracketsStart(char ch);
	bool IsSingleLineCommentStart(char ch);
	bool IsMultiLineCommentStart(char ch);
	bool IsPartOfExponentialNotation(char ch);

private:
	std::istream& m_istrm;
	std::string m_currentLexeme;
	size_t m_lineNumber = 1;
	size_t m_columnNumber = 1;
	State m_currentState = State::Default;
	bool isError = false;
	std::vector<Token> m_tokens;
	size_t m_ePosition = 0;
};