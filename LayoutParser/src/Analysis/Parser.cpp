#include "Analysis/Parser.h"

#include <stack>

#include "Analysis/Lexer.h"
#include "Analysis/SyntaxFacts.h"

#include "Data/Object.h"
#include "Data/Value.h"

using namespace LayoutParser;

Parser::Parser(const std::string& text)
	: m_Tokens(), m_Position(0)
{
	Lexer lexer(text);
	SyntaxToken token;
	do
	{
		token = lexer.Lex();

		if (token.Kind != SyntaxKind::WhitespaceToken &&
			token.Kind != SyntaxKind::CommentToken &&
			token.Kind != SyntaxKind::NewlineToken &&
			token.Kind != SyntaxKind::BadToken)
		{
			m_Tokens.push_back(token);
		}
	} while (token.Kind != SyntaxKind::EndOfFileToken);

	if (!lexer.GetDiagnostics().IsEmpty())
		m_Diagnostics = std::move(lexer.GetDiagnostics());
}

// Helpers
const SyntaxToken& Parser::Peek(int32_t offset) const
{
	int32_t index = m_Position + offset;
	if (index >= m_Tokens.size())
		return m_Tokens[m_Tokens.size() - 1];

	return m_Tokens[index];
}

SyntaxToken Parser::MatchToken(SyntaxKind kind)
{
	// Kinda an expensive function since it always creates a new string
	if (Current().Kind == kind)
		return NextToken();

	m_Diagnostics.ReportUnexpectedToken(Current().Kind, kind);
	return SyntaxToken(kind, Current().Position, "");
}

// Parse logic
std::unordered_map<std::string, Layout> Parser::Parse()
{
	std::unordered_map<std::string, Layout> layouts;
	do
	{
		if (Current().Kind == SyntaxKind::EndOfFileToken)
			break;

		SyntaxToken layoutIdentifier = MatchToken(SyntaxKind::IdentifierToken);
		layouts.emplace(layoutIdentifier.Text, std::move(ParseLayoutBody()));

	} while (Current().Kind == SyntaxKind::IdentifierToken);

	return layouts;
}

std::vector<Object*> Parser::ParseLayoutBody()
{
	MatchToken(SyntaxKind::OpenSquigglyBracketToken);

	std::vector<Object*> objects;
	do
	{
		if (Current().Kind == SyntaxKind::CloseSquigglyBracketToken)
			break;

		objects.push_back(ParseObject());

	} while (Current().Kind == SyntaxKind::OpenAngleBracketToken);

	MatchToken(SyntaxKind::CloseSquigglyBracketToken);

	return objects;
}

Object* Parser::ParseObject()
{
	MatchToken(SyntaxKind::OpenAngleBracketToken);
	SyntaxToken identifier = MatchToken(SyntaxKind::IdentifierToken);

	MatchToken(SyntaxKind::OpenParenthesisToken);
	Value* constructor = nullptr;
	if (Current().Kind != SyntaxKind::CloseParenthesisToken)
		constructor = ParseValue();
	MatchToken(SyntaxKind::CloseParenthesisToken);

	std::unordered_map<std::string, Value*> properties;
	do
	{
		if (Current().Kind == SyntaxKind::CommaToken)
			NextToken();
		else if (Current().Kind == SyntaxKind::CloseAngleBracketToken)
			break;

		SyntaxToken propertyName = MatchToken(SyntaxKind::IdentifierToken);
		MatchToken(SyntaxKind::EqualsToken);
		properties[propertyName.Text] = ParseValue();

	} while (Current().Kind == SyntaxKind::CommaToken);

	MatchToken(SyntaxKind::CloseAngleBracketToken);

	return new Object(identifier.Text, constructor, std::move(properties));
}

Value* Parser::ParseValue()
{
	switch (Current().Kind)
	{
	case SyntaxKind::OpenAngleBracketToken:
		return new ObjectValue(ParseObject());
	case SyntaxKind::StringToken:
	{
		// Using pointer to avoid copies. Not sure if this is the best solution
		const std::string* tokenText = &NextToken().Text;
		return new StringValue(tokenText->substr(1, tokenText->length() - 2));
	}
	case SyntaxKind::TrueKeyword:
	case SyntaxKind::FalseKeyword:
		return new BooleanValue(NextToken().Kind == SyntaxKind::TrueKeyword);
	case SyntaxKind::HexColorToken:
		return new HexColorValue(NextToken().Text);
	case SyntaxKind::OpenSquareBracketToken:
		return ParseDictionary();
	case SyntaxKind::OpenSquigglyBracketToken:
		return ParseList();
	default:
		return ParseNumber();
	}
}

ListValue* Parser::ParseList()
{
	MatchToken(SyntaxKind::OpenSquigglyBracketToken);

	std::vector<const Value*> listValues;
	do
	{
		if (Current().Kind == SyntaxKind::CommaToken)
			NextToken();
		else if (Current().Kind == SyntaxKind::CloseSquigglyBracketToken)
			break;

		listValues.push_back(ParseValue());

	} while (Current().Kind == SyntaxKind::CommaToken);

	MatchToken(SyntaxKind::CloseSquigglyBracketToken);

	return new ListValue(std::move(listValues));
}

DictionaryValue* Parser::ParseDictionary()
{
	MatchToken(SyntaxKind::OpenSquareBracketToken);

	std::unordered_map<std::string, const Value*> dictionaryValues;
	do
	{
		if (Current().Kind == SyntaxKind::CommaToken)
			NextToken();
		else if (Current().Kind == SyntaxKind::CloseSquareBracketToken)
			break;

		SyntaxToken keyIdentifier = MatchToken(SyntaxKind::IdentifierToken);
		MatchToken(SyntaxKind::EqualsToken);
		dictionaryValues[keyIdentifier.Text] = ParseValue();

	} while (Current().Kind == SyntaxKind::CommaToken);

	MatchToken(SyntaxKind::CloseSquareBracketToken);

	return new DictionaryValue(std::move(dictionaryValues));
}

NumberValue* Parser::ParseNumber()
{
	// Get a list of expression tokens
	int32_t parenthesisDepth = 0;
	std::vector<const SyntaxToken*> expressionTokens;
	while (SyntaxFacts::IsExpressionToken(Current().Kind) &&
		(Current().Kind != SyntaxKind::CloseParenthesisToken || parenthesisDepth != 0))
	{
		if (Current().Kind == SyntaxKind::OpenParenthesisToken)
			parenthesisDepth++;
		else if (Current().Kind == SyntaxKind::CloseParenthesisToken)
			parenthesisDepth--;
		expressionTokens.push_back(&NextToken());
	}

	// Convert infix to postfix
	std::vector<const SyntaxToken*> postfixResult;

	{
		std::stack<const SyntaxToken*> algorithmStack;

		for (const SyntaxToken* token : expressionTokens)
		{
			switch (token->Kind)
			{
			case SyntaxKind::NumberToken:
				postfixResult.push_back(token);
				break;
			case SyntaxKind::OpenParenthesisToken:
				algorithmStack.push(token);
				break;
			case SyntaxKind::CloseParenthesisToken:
				while (!algorithmStack.empty() && algorithmStack.top()->Kind != SyntaxKind::OpenParenthesisToken)
				{
					postfixResult.push_back(algorithmStack.top());
					algorithmStack.pop();
				}
				algorithmStack.pop();
				break;
			default: // Operator token
			{
				const SyntaxKind currentKind = token->Kind;
				while (!algorithmStack.empty())
				{
					const SyntaxKind topKind = algorithmStack.top()->Kind;
					if ((SyntaxFacts::IsOperatorLeftAssociative(currentKind) &&
						SyntaxFacts::GetOperatorPrecedence(currentKind) <= SyntaxFacts::GetOperatorPrecedence(topKind)) ||
						(!SyntaxFacts::IsOperatorLeftAssociative(currentKind) &&
							SyntaxFacts::GetOperatorPrecedence(currentKind) < SyntaxFacts::GetOperatorPrecedence(topKind)))
					{
						postfixResult.push_back(algorithmStack.top());
						algorithmStack.pop();
						continue;
					}
					break;
				}
				algorithmStack.push(token);
				break;
			}
			}
		}

		while (!algorithmStack.empty())
		{
			if (algorithmStack.top()->Kind == SyntaxKind::OpenParenthesisToken)
			{
				m_Diagnostics.ReportMismatchedParentheses();
				return new NumberValue(0.0f);
			}

			postfixResult.push_back(algorithmStack.top());
			algorithmStack.pop();
		}
	}

	// Evaluate postfix

	std::stack<float> evaluationStack;

	for (const SyntaxToken* token : postfixResult)
	{
		switch (token->Kind)
		{
		case SyntaxKind::NumberToken:
			evaluationStack.push(token->Value);
			break;
		case SyntaxKind::OpenParenthesisToken:
			m_Diagnostics.ReportMismatchedParentheses();
			return new NumberValue(0.0f);
		case SyntaxKind::CloseParenthesisToken:
			m_Diagnostics.ReportMismatchedParentheses();
			return new NumberValue(0.0f);
		default: // Operator
		{
			if (evaluationStack.size() < 2)
			{
				m_Diagnostics.ReportInvalidNumberExpression();
				return new NumberValue(0.0f);
			}

			float right = evaluationStack.top();
			evaluationStack.pop();
			float left = evaluationStack.top();
			evaluationStack.pop();

			switch (token->Kind)
			{
			case SyntaxKind::CaretToken: // Exponent
				evaluationStack.push(std::powf(left, right));
				break;
			case SyntaxKind::StarToken: // Multiplication
				evaluationStack.push(left * right);
				break;
			case SyntaxKind::SlashToken: // Division
				evaluationStack.push(left / right);
				break;
			case SyntaxKind::PlusToken: // Addition
				evaluationStack.push(left + right);
				break;
			case SyntaxKind::MinusToken: // Subtraction
				evaluationStack.push(left - right);
				break;
			default:
				break;
			}
			break;
		}
		}
	}

	if (evaluationStack.size() == 1)
		return new NumberValue(evaluationStack.top());
	else if (evaluationStack.size() != 0)
		m_Diagnostics.ReportInvalidNumberExpression();

	return new NumberValue(0.0f);
}