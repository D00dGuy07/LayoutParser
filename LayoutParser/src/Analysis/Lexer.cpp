#include "Analysis/Lexer.h"

#include <sstream>

#include "Analysis/SyntaxFacts.h"

using namespace LayoutParser;

SyntaxToken Lexer::Lex()
{
	if (m_Position >= m_Text.length())
	return SyntaxToken(SyntaxKind::EndOfFileToken, m_Position, "\0");

	int32_t start = m_Position;

	// This is kinda overkill but I like it
	if (SyntaxFacts::IdentifyNumberLiteral(Current()))
	{
		char baseIdentifier = Lookahead();

		Next();
		while (SyntaxFacts::IsDigitAnyBase(Current()))
			Next();

		int32_t length = m_Position - start;
		switch (baseIdentifier)
		{
		case 'b':
		{
			std::string tokenText = m_Text.substr(start, length);
			float value = 0.0f;
			try
			{
				std::string numberText = m_Text.substr(static_cast<size_t>(start) + 2, static_cast<size_t>(length) - 2);
				value = static_cast<float>(std::stoi(numberText, nullptr, 2));
			}
			catch (std::exception)
			{
				m_Diagnostics.ReportInvalidBinaryNumber(tokenText);
				
			}

			return SyntaxToken(SyntaxKind::NumberToken, start, tokenText, value);
		}
		case 'x':
		{
			std::string tokenText = m_Text.substr(start, length);
			float value = 0.0f;
			try
			{
				std::string numberText = m_Text.substr(static_cast<size_t>(start) + 2, static_cast<size_t>(length) - 2);
				value = static_cast<float>(std::stoi(numberText, nullptr, 16));
			}
			catch (std::exception)
			{
				m_Diagnostics.ReportInvalidHexNumber(tokenText);
			}

			return SyntaxToken(SyntaxKind::NumberToken, start, tokenText, value);
		}
		default:
		{
			std::string tokenText = m_Text.substr(start, length);
			float value = 0.0f;
			try
			{
				value = std::stof(tokenText);
			}
			catch (std::exception)
			{
				m_Diagnostics.ReportInvalidNumber(tokenText);
			}

			return SyntaxToken(SyntaxKind::NumberToken, start, tokenText, value);
		}
		}
	}

	// String literals
	if (Current() == '"')
	{
		Next();
		while (Current() != '"')
		{
			if (Current() == '\0')
			{
				m_Diagnostics.ReportMissingDoubleQuote();
				break;
			}
			else
				Next();
		}
		Next();

		int32_t length = m_Position - start;
		std::string text = m_Text.substr(start, length);
		return SyntaxToken(SyntaxKind::StringToken, start, text);
	}

	// Hex color
	if (Current() == '#')
	{
		Next();
		for (int32_t i = 0; i < 6; i++)
		{
			if (!SyntaxFacts::IsDigitHex(Current()))
			{
				m_Diagnostics.ReportInvalidHexColorString(Current());
			}
			Next();
		}

		int32_t length = m_Position - start;
		std::string text = m_Text.substr(start, length);
		return SyntaxToken(SyntaxKind::HexColorToken, start, text);
	}

	// Identifiers
	if (SyntaxFacts::IdentifyIdentifier(Current()))
	{
		Next();
		while (SyntaxFacts::IsIdentifierCharacter(Current()))
			Next();

		int32_t length = m_Position - start;
		std::string text = m_Text.substr(start, length);
		return SyntaxToken(SyntaxFacts::ParseKeywordKind(text), start, text);
	}

	// Whitespace
	if (SyntaxFacts::IsWhitespaceCharacter(Current()))
	{
		Next();
		while (SyntaxFacts::IsWhitespaceCharacter(Current()))
			Next();

		int32_t length = m_Position - start;
		std::string text = m_Text.substr(start, length);
		return SyntaxToken(SyntaxKind::WhitespaceToken, start, text);
	}

	// Comments
	if (Current() == '/' && Lookahead() == '/')
	{
		m_Position += 2;
		while (Current() != '\n' && Current() != '\0')
			Next();

		int32_t length = m_Position - start;
		std::string text = m_Text.substr(start, length);
		return SyntaxToken(SyntaxKind::CommentToken, start, text);
	}

	switch (Current())
	{
	case '\n':
		return SyntaxToken(SyntaxKind::NewlineToken, m_Position++, "\n");

	case '+':
		return SyntaxToken(SyntaxKind::PlusToken, m_Position++, "+");
	case '-':
		return SyntaxToken(SyntaxKind::MinusToken, m_Position++, "-");
	case '*':
		return SyntaxToken(SyntaxKind::StarToken, m_Position++, "*");
	case '/':
		return SyntaxToken(SyntaxKind::SlashToken, m_Position++, "/");
	case '^':
		return SyntaxToken(SyntaxKind::CaretToken, m_Position++, "^");

	case '<':
		return SyntaxToken(SyntaxKind::OpenAngleBracketToken, m_Position++, "<");
	case '>':
		return SyntaxToken(SyntaxKind::CloseAngleBracketToken, m_Position++, ">");
	case '{':
		return SyntaxToken(SyntaxKind::OpenSquigglyBracketToken, m_Position++, "{");
	case '}':
		return SyntaxToken(SyntaxKind::CloseSquigglyBracketToken, m_Position++, "}");
	case '[':
		return SyntaxToken(SyntaxKind::OpenSquareBracketToken, m_Position++, "[");
	case ']':
		return SyntaxToken(SyntaxKind::CloseSquareBracketToken, m_Position++, "]");
	case '(':
		return SyntaxToken(SyntaxKind::OpenParenthesisToken, m_Position++, "(");
	case ')':
		return SyntaxToken(SyntaxKind::CloseParenthesisToken, m_Position++, ")");

	case '=':
		return SyntaxToken(SyntaxKind::EqualsToken, m_Position++, "=");
	case ',':
		return SyntaxToken(SyntaxKind::CommaToken, m_Position++, ",");
	}

	m_Diagnostics.ReportBadCharacter(Current());

	return SyntaxToken(SyntaxKind::BadToken, m_Position++, m_Text.substr(static_cast<size_t>(m_Position) - 1, 1));
}

char Lexer::Peek(int32_t offset) const
{
	int32_t index = m_Position + offset;
	if (index >= m_Text.length())
		return '\0';

	return m_Text[index];
}