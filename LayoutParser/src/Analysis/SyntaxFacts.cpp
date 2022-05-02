#include "Analysis/SyntaxFacts.h"

#include <iostream>

using namespace LayoutParser;

SyntaxKind SyntaxFacts::ParseKeywordKind(const std::string& keyword)
{
	size_t stringSize = keyword.length() + 1;

	SyntaxKind returnType = SyntaxKind::IdentifierToken;
	if (stringSize < 1000) // Completely arbitrary keyword size limit
	{
		char* lowerCaseString = static_cast<char*>(_malloca(stringSize));
		if (lowerCaseString == nullptr)
			return SyntaxKind::IdentifierToken;

		memcpy(lowerCaseString, keyword.c_str(), stringSize);

		// Convert to lowercase
		for (int32_t i = 0; i < static_cast<int32_t>(stringSize); i++)
		{
			char character = lowerCaseString[i];
			if (character >= 'A' && character <= 'Z')
				lowerCaseString[i] = character ^ ' ';
		}

		if (!strcmp(lowerCaseString, "true"))
			returnType = SyntaxKind::TrueKeyword;
		else if (!strcmp(lowerCaseString, "false"))
			returnType = SyntaxKind::FalseKeyword;

		_freea(lowerCaseString);
	}

	return returnType;
}

bool SyntaxFacts::IsExpressionToken(SyntaxKind kind)
{
	switch (kind)
	{
	case SyntaxKind::CaretToken: // Exponent
	case SyntaxKind::StarToken: // Multiplication
	case SyntaxKind::SlashToken: // Division
	case SyntaxKind::PlusToken: // Addition
	case SyntaxKind::MinusToken: // Subtraction
	case SyntaxKind::NumberToken:
	case SyntaxKind::OpenParenthesisToken:
	case SyntaxKind::CloseParenthesisToken:
		return true;
	default:
		return false;
	}
}

int32_t SyntaxFacts::GetOperatorPrecedence(SyntaxKind kind)
{
	switch (kind)
	{
	case SyntaxKind::CaretToken: // Exponent
		return 3;
	case SyntaxKind::StarToken: // Multiplication
		return 2;
	case SyntaxKind::SlashToken: // Division
		return 2;
	case SyntaxKind::PlusToken: // Addition
		return 1;
	case SyntaxKind::MinusToken: // Subtraction
		return 1;
	default:
		return -1; // Error case. Something is wrong if we are here
	}
}

bool SyntaxFacts::IsOperatorLeftAssociative(SyntaxKind kind)
{
	switch (kind)
	{
	case SyntaxKind::CaretToken: // Exponent
		return false;
	case SyntaxKind::StarToken: // Multiplication
		return true;
	case SyntaxKind::SlashToken: // Division
		return true;
	case SyntaxKind::PlusToken: // Addition
		return true;
	case SyntaxKind::MinusToken: // Subtraction
		return true;
	default:
		return true; // Error case. Something is wrong if we are here
	}
}