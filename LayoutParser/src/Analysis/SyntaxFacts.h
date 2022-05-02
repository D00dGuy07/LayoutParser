#pragma once

#include <string>

#include "Analysis/SyntaxKind.h"

namespace LayoutParser
{
	namespace SyntaxFacts
	{
		// Lexer data/helpers
		const static std::string DIGITS_DEC = "0123456789.";
		const static std::string DIGITS_HEX = "0123456789abcdefABCDEF";
		const static std::string DIGITS_ALL = "0123456789abcdefABCDEF.";

		inline bool IdentifyNumberLiteral(char character)
		{
			return DIGITS_DEC.find(character) != std::string::npos;
		}

		inline bool IdentifyIdentifier(char character)
		{
			return character == '_' || std::isalpha(character) != 0;
		}

		inline bool IsDigitAnyBase(char character)
		{
			return DIGITS_ALL.find(character) != std::string::npos;
		}

		inline bool IsDigitHex(char character)
		{
			return DIGITS_HEX.find(character) != std::string::npos;
		}

		inline bool IsIdentifierCharacter(char character)
		{
			return character == '_' || std::isalpha(character) != 0 || std::isdigit(character) != 0;
		}

		inline bool IsWhitespaceCharacter(char character)
		{
			return character == ' ' || character == '\t' || character == '\v' || character == '\f';
		}

		SyntaxKind ParseKeywordKind(const std::string& keyword);

		// Parser data/helpers

		bool IsExpressionToken(SyntaxKind kind);
		int32_t GetOperatorPrecedence(SyntaxKind kind);
		bool IsOperatorLeftAssociative(SyntaxKind kind);
	}
}