#pragma once

#include <vector>
#include <string>

#include "SyntaxKind.h"

namespace LayoutParser
{
	class DiagnosticCollection
	{
	public:
		DiagnosticCollection() = default;
		DiagnosticCollection(const DiagnosticCollection& other)
			: m_Diagnostics(other.m_Diagnostics) {}

		DiagnosticCollection(DiagnosticCollection&& other) noexcept
			: m_Diagnostics(std::move(other.m_Diagnostics)) {}

		inline DiagnosticCollection& operator=(DiagnosticCollection&& other) noexcept
		{
			if (this != &other)
				m_Diagnostics = std::move(other.m_Diagnostics);
			return *this;
		}

		bool inline IsEmpty() const { return m_Diagnostics.empty(); }

		void ReportInvalidBinaryNumber(const std::string& numberText);
		void ReportInvalidHexNumber(const std::string& numberText);
		void ReportInvalidNumber(const std::string& numberText);

		void ReportMissingDoubleQuote();
		void ReportInvalidHexColorString(char character);
		void ReportBadCharacter(char character);

		void ReportUnexpectedToken(SyntaxKind token, SyntaxKind expectedToken);
		void ReportMismatchedParentheses();
		void ReportInvalidNumberExpression();

		auto begin() { return m_Diagnostics.begin(); }
		auto end() { return m_Diagnostics.end(); }

	private:
		std::vector<std::string> m_Diagnostics;

		inline void Report(std::string&& diagnostic) { m_Diagnostics.emplace_back(diagnostic); }

		const char* GetSyntaxKindName(SyntaxKind kind);
	};
}