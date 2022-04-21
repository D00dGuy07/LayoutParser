#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Analysis/SyntaxToken.h"
#include "Analysis/Diagnostics.h"

#include "Data/LayoutCollection.h"

namespace LayoutParser
{
	// Forward declaration
	struct Object;
	struct Value;
	struct ListValue;
	struct DictionaryValue;
	struct NumberValue;

	class Parser
	{
	public:
		Parser(const std::string& text);

		inline DiagnosticCollection& GetDiagnostics() { return m_Diagnostics; }

		std::unordered_map<std::string, Layout> Parse();

	private:
		std::vector<SyntaxToken> m_Tokens;
		DiagnosticCollection m_Diagnostics;

		int32_t m_Position;

		// Returning const because vector is returning that and the parser shouldn't edit anything anyway
		const SyntaxToken& Peek(int32_t offset) const;

		inline const SyntaxToken& Current() const { return Peek(0); }

		inline const SyntaxToken& NextToken()
		{
			m_Position++;
			return Peek(-1); // Kinda hacky but should save a constructor call in theory
		}

		SyntaxToken MatchToken(SyntaxKind kind);

		std::vector<Object*> ParseLayoutBody();

		Object* ParseObject();

		Value* ParseValue();

		ListValue* ParseList();

		DictionaryValue* ParseDictionary();

		NumberValue* ParseNumber();
	};
}