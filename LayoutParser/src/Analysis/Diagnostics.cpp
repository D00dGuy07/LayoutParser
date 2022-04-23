#include "Analysis/Diagnostics.h"

#include <sstream>

using namespace LayoutParser;

void DiagnosticCollection::ReportInvalidBinaryNumber(const std::string& numberText)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Failed to parse binary number '" << numberText << "'.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportInvalidHexNumber(const std::string& numberText)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Failed to parse hexidecimal number '" << numberText << "'.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportInvalidNumber(const std::string& numberText)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Failed to parse number '" << numberText << "'.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportMissingDoubleQuote()
{
	m_Diagnostics.push_back("Missing double quotation mark when parsing string literal.");
}

void DiagnosticCollection::ReportInvalidHexColorString(char character)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Expected hexidecimal digit while parsing hex color but found '" << character << "' instead.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportBadCharacter(char character)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Bad character found: '" << character << "'.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportUnexpectedToken(SyntaxKind token, SyntaxKind expectedToken)
{
	std::stringstream errorText = std::stringstream();
	errorText << "Unexpected token found <" << GetSyntaxKindName(token) << ">. Expected <" << GetSyntaxKindName(expectedToken) << ">.";
	m_Diagnostics.push_back(errorText.str());
}

void DiagnosticCollection::ReportMismatchedParentheses()
{
	m_Diagnostics.push_back("Mismatched parentheses found while evaluating number value.");
}

void DiagnosticCollection::ReportInvalidNumberExpression()
{
	m_Diagnostics.push_back("Invalid expression encountered evaluating number value");
}

const char* DiagnosticCollection::GetSyntaxKindName(SyntaxKind kind)
{
	switch (kind)
	{
	case SyntaxKind::BadToken:
		return "BadToken";
	case SyntaxKind::EndOfFileToken:
		return "EndOfFileToken";
	case SyntaxKind::WhitespaceToken:
		return "WhitespaceToken";
	case SyntaxKind::CommentToken:
		return "CommentToken";
	case SyntaxKind::NewlineToken:
		return "NewlineToken";
	case SyntaxKind::NumberToken:
		return "NumberToken";
	case SyntaxKind::StringToken:
		return "StringToken";
	case SyntaxKind::HexColorToken:
		return "HexColorToken";
	case SyntaxKind::PlusToken:
		return "PlusToken";
	case SyntaxKind::MinusToken:
		return "MinusToken";
	case SyntaxKind::StarToken:
		return "StarToken";
	case SyntaxKind::SlashToken:
		return "SlashToken";
	case SyntaxKind::CaretToken:
		return "CaretToken";
	case SyntaxKind::OpenAngleBracketToken:
		return "OpenAngleBracketToken";
	case SyntaxKind::CloseAngleBracketToken:
		return "CloseAngleBracketToken";
	case SyntaxKind::OpenSquigglyBracketToken:
		return "OpenSquigglyBracketToken";
	case SyntaxKind::CloseSquigglyBracketToken:
		return "CloseSquigglyBracketToken";
	case SyntaxKind::OpenSquareBracketToken:
		return "OpenSquareBracketToken";
	case SyntaxKind::CloseSquareBracketToken:
		return "CloseSquareBracketToken";
	case SyntaxKind::OpenParenthesisToken:
		return "OpenParenthesisToken";
	case SyntaxKind::CloseParenthesisToken:
		return "CloseParenthesisToken";
	case SyntaxKind::EqualsToken:
		return "EqualsToken";
	case SyntaxKind::CommaToken:
		return "CommaToken";
	case SyntaxKind::IdentifierToken:
		return "IdentifierToken";
	default:
		return "InvalidToken";
	}
}