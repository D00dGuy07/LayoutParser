#include "Analysis/SyntaxFacts.h"

using namespace LayoutParser;

bool LayoutParser::SyntaxFacts::IsExpressionToken(SyntaxKind kind)
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

int32_t LayoutParser::SyntaxFacts::GetOperatorPrecedence(SyntaxKind kind)
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

bool LayoutParser::SyntaxFacts::IsOperatorLeftAssociative(SyntaxKind kind)
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