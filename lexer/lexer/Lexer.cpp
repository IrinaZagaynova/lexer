#include "Lexer.h"
#include "CommonTypes.h"
#include <regex>

using namespace std;

CLexer::CLexer(istream& istrm)
	: m_istrm(istrm)
{
	ProcessStream();
}

vector<Token> CLexer::GetTokens() const
{
	return m_tokens;
}

void CLexer::PrintTokens(ostream& ostrm) const
{
	for (const auto& token : m_tokens)
	{
		ostrm << token.line << " " << token.column << " " << token.value << " " << TOKEN_NAMES.find(token.type)->second << "\n";
	}
}

void CLexer::ProcessStream()
{
	char ch;
	while (m_istrm.get(ch))
	{
		m_columnNumber++;
		ProcessChar(ch);
		CheckLineEnd();
	}
}

void CLexer::ProcessChar(char ch)
{
	switch (m_currentState)
	{
	case State::Default:
		ProcessCharWhenDefaultState(ch);
		break;
	case State::Quotes:
		ProcessCharInQuotes(ch);
		break;
	case State::Apostrophes:
		ProcessCharInApostrophes(ch);
		break;
	case State::MultiLineComment:
		ProcessCharInMultiLineComment(ch);
		break;
	case State::SquareBrackets:
		ProcessCharInSquareBrackets(ch);
		break;
	}
}

void CLexer::CheckLineEnd()
{
	if (m_istrm.peek() == '\n')
	{
		if (m_currentState == State::Default)
		{
			AddTokenByCurrentLexeme();
		}

		if (m_currentState == State::SingleLineÑomment)
		{
			m_currentState = State::Default;
		}

		if (m_currentState == State::Quotes || m_currentState == State::Apostrophes || m_currentState == State::SquareBrackets)
		{
			AddToken(TokenType::Error);
			m_currentState = State::Default;
		}

		m_columnNumber = 0;
		m_lineNumber++;
	}
}

void CLexer::ProcessCharWhenDefaultState(char ch)
{
	if (IsQuotesStart(ch) || IsApostrophesStart(ch) || IsSquareBracketsStart(ch))
	{
		AddTokenByCurrentLexeme();
		m_currentLexeme.push_back(ch);
		return;
	}

	auto tokenTypeByChar = GetTokenTypeByChar(ch);

	if (m_currentState != State::Default)
	{
		return;
	}

	if (tokenTypeByChar)
	{
		AddTokenByCurrentLexeme();
		if (!IsIgnoredSeparator(ch))
		{
			m_currentLexeme.push_back(ch);
			AddToken(*tokenTypeByChar);
		}
		return;
	}

	m_currentLexeme.push_back(ch);
}

void CLexer::ProcessCharInMultiLineComment(char ch)
{
	m_currentLexeme.push_back(ch);
	if (m_currentLexeme.size() > 1 && m_currentLexeme.substr(m_currentLexeme.size() - 2) == "*/")
	{
		m_currentState = State::Default;
		m_currentLexeme.clear();
	}
}

void CLexer::ProcessCharInQuotes(char ch)
{
	m_currentLexeme.push_back(ch);
	if (ch == '\"' && m_currentLexeme.size() > 1 && m_currentLexeme[m_currentLexeme.size() - 2] != '\\')
	{
		AddToken(TokenType::String);
		m_currentState = State::Default;
	}
}

void CLexer::ProcessCharInApostrophes(char ch)
{
	m_currentLexeme.push_back(ch);

	if (ch == '\'')
	{
		if (m_currentLexeme.size() < 3)
		{
			AddToken(TokenType::Error);
			m_currentState = State::Default;
		}
		else if (m_currentLexeme[m_currentLexeme.size() - 2] != '\\' || m_currentLexeme.size() % 2 == 0)
		{
			AddToken(TokenType::Char);
			m_currentState = State::Default;
		}
	}
}

void CLexer::ProcessCharInSquareBrackets(char ch)
{
	m_currentLexeme.push_back(ch);

	if (ch == ']')
	{
		if (!isError)
		{
			AddToken(TokenType::Array);
		}
		else
		{
			AddToken(TokenType::Error);
			isError = false;
		}
		m_currentState = State::Default;
	}
	else if (!isdigit(ch))
	{
		isError = true;
	}
}

optional<TokenType> CLexer::GetTokenTypeByChar(char ch)
{
	if (IsSeparator(ch))
	{
		return TokenType::Separator;
	}

	if (IsBracket(ch))
	{
		return TokenType::Bracket;
	}

	if (ch == '=' && m_istrm.peek() != '=' && m_currentLexeme != "=" && m_currentLexeme != "<" && m_currentLexeme != ">" && m_currentLexeme != "!")
	{
		return TokenType::Multiplication;
	}

	if ((ch == '>' || ch == '<') && m_istrm.peek() != '=')
	{
		return TokenType::Comparison;
	}

	if (ch == '!' && m_istrm.peek() != '=')
	{
		return TokenType::NegationOperator;
	}

	if (ch == '+')
	{
		return TokenType::Addition;
	}

	if (ch == '-')
	{
		return TokenType::Subtraction;
	}

	if (ch == '*')
	{
		return TokenType::Multiplication;
	}

	if (ch == '/')
	{
		char nextChar = m_istrm.peek();
		if (nextChar == '*')
		{
			AddTokenByCurrentLexeme();
			m_currentState = State::MultiLineComment;
		}
		else if (nextChar == '/')
		{
			AddTokenByCurrentLexeme();
			m_currentState = State::SingleLineÑomment;
		}
		else
		{
			return TokenType::Division;
		}
	}

	return nullopt;
}

optional<TokenType> CLexer::GetTokenTypeByCurrentLexeme()
{
	if (m_currentLexeme.length() == 0)
	{
		return nullopt;
	}

	if (IsCurrentLexemeComparison())
	{
		return TokenType::Comparison;
	}

	if (IsCurrentLexemeLogicalOperator())
	{
		return TokenType::LogicalOperator;
	}

	if (IsCurrentLexemeKeyword())
	{
		return TokenType::Keyword;
	}

	if (IsCurrentLexemeBinaryNumber())
	{
		return TokenType::Binary;
	}

	if (IsCurrentLexemeHexNumber())
	{
		return TokenType::Hex;
	}

	if (IsCurrentLexemeInt())
	{
		return TokenType::Int;
	}

	if (IsCurrentLexemeFloat())
	{
		return TokenType::Float;
	}

	if (IsCurrentLexemeIdentifier())
	{
		return TokenType::Identifier;
	}

	if (m_currentLexeme.size() == 1)
	{
		auto tokenType = GetTokenTypeByChar(m_currentLexeme[0]);
		if (tokenType)
		{
			return *tokenType;
		}
	}

	return TokenType::Error;
}

void CLexer::AddToken(TokenType type)
{
	m_tokens.push_back({ type, m_currentLexeme, m_lineNumber, m_columnNumber - m_currentLexeme.length() - 1});
	m_currentLexeme.clear();
}

bool CLexer::AddTokenByCurrentLexeme()
{
	auto tokenType = GetTokenTypeByCurrentLexeme();
	if (tokenType)
	{
		AddToken(*tokenType);
		return true;
	}
	return false;
}

bool CLexer::IsSeparator(char ch) const
{
	return SEPARATORS.find(ch) != SEPARATORS.end();
}

bool CLexer::IsBracket(char ch) const
{
	return BRACKETS.find(ch) != BRACKETS.end();
}

bool CLexer::IsIgnoredSeparator(char ch) const
{
	return IGNORED_SEPARATORS.find(ch) != IGNORED_SEPARATORS.end();
}

bool CLexer::IsCurrentLexemeKeyword() const
{
	return KEYWORDS.find(m_currentLexeme) != KEYWORDS.end();
}

bool CLexer::IsCurrentLexemeBinaryNumber() const
{
	return regex_match(m_currentLexeme, regex("[-+]?0b[01]+"));
}

bool CLexer::IsCurrentLexemeHexNumber() const
{
	return regex_match(m_currentLexeme, regex("[-+]?0x[0-9A-F]+"));
}

bool CLexer::IsCurrentLexemeInt() const
{
	return regex_match(m_currentLexeme, regex("[-+]?[0-9]+"));
}

bool CLexer::IsCurrentLexemeFloat() const
{
	return regex_match(m_currentLexeme, regex("[+-]?([0-9]*[.])?[0-9]+"));
}

bool CLexer::IsCurrentLexemeIdentifier() const
{
	return regex_match(m_currentLexeme, regex("[a-zA-Z_][0-9a-zA-Z_]*"));
}

bool CLexer::IsCurrentLexemeComparison() const
{
	return m_currentLexeme == "==" || m_currentLexeme == "!=" || m_currentLexeme == "<=" || m_currentLexeme == ">=";
}

bool CLexer::IsCurrentLexemeLogicalOperator() const
{
	return m_currentLexeme == "&&" || m_currentLexeme == "||";;
}

bool CLexer::IsQuotesStart(char ch)
{
	if (m_currentState == State::Default && ch == '\"')
	{
		m_currentState = State::Quotes;
		return true;
	}
	return false;
}

bool CLexer::IsApostrophesStart(char ch)
{
	if (m_currentState == State::Default && ch == '\'')
	{
		m_currentState = State::Apostrophes;
		return true;
	}
	return false;
}

bool CLexer::IsSquareBracketsStart(char ch)
{
	if (m_currentState == State::Default && ch == '[')
	{
		m_currentState = State::SquareBrackets;
		return true;
	}
	return false;
}
