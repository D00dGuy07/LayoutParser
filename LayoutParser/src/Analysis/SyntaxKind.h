#pragma once

namespace LayoutParser
{
	enum class SyntaxKind
	{
		// Tokens
		BadToken,
		EndOfFileToken,
		WhitespaceToken,
		CommentToken,
		NewlineToken,

		NumberToken,
		StringToken,
		HexColorToken,

		PlusToken,
		MinusToken,
		StarToken,
		SlashToken,
		CaretToken,

		OpenAngleBracketToken,
		CloseAngleBracketToken,
		OpenSquigglyBracketToken,
		CloseSquigglyBracketToken,
		OpenSquareBracketToken,
		CloseSquareBracketToken,
		OpenParenthesisToken,
		CloseParenthesisToken,

		EqualsToken,
		CommaToken,
		IdentifierToken,

		// Keywords
		TrueKeyword,
		FalseKeyword
	};
}