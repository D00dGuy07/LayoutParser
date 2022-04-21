#pragma once

#include <string>

#include "Analysis/SyntaxKind.h"

namespace LayoutParser
{
	struct SyntaxToken
	{
		// Regular Constructors
		SyntaxToken(SyntaxKind kind, int32_t position, const std::string& text, float value)
			: Kind(kind), Position(position), Text(text), Value(value) {}

		SyntaxToken(SyntaxKind kind, int32_t position, const std::string& text)
			: SyntaxToken(kind, position, text, 0.0f) {}

		SyntaxToken()
			: SyntaxToken(SyntaxKind::BadToken, -1, "", 0.0f) {}

		// Move/Copy Constructors

		SyntaxToken(SyntaxToken&& other) noexcept
			: Kind(other.Kind), Position(other.Position), Text(std::move(other.Text)), Value(other.Value) {}

		SyntaxToken(const SyntaxToken& other) = default;

		inline SyntaxToken& operator=(SyntaxToken&& other) noexcept
		{
			if (this != &other)
			{
				Kind = other.Kind;
				Position = other.Position;
				Text = std::move(other.Text);
				Value = other.Value;
			}

			return *this;
		}

		SyntaxKind Kind;
		int32_t Position;
		std::string Text;
		float Value;
	};
}