#pragma once

#include <string>
#include <vector>

#include "Analysis/SyntaxToken.h"
#include "Analysis/Diagnostics.h"

namespace LayoutParser
{
	class Lexer
	{
	public:
		Lexer(const std::string& text)
			: m_Text(text), m_Position(0) {}

		SyntaxToken Lex();

		inline DiagnosticCollection& GetDiagnostics() { return m_Diagnostics; }

	private:
		const std::string m_Text;
		int32_t m_Position;
		DiagnosticCollection m_Diagnostics;

		char Peek(int32_t offset) const;

		inline void Next()
		{
			m_Position++;
		}

		inline char Current() const { return Peek(0); }
		inline char Lookahead() const { return Peek(1); }
	};
}