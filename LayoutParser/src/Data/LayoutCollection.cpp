#include "Data/LayoutCollection.h"

#include <fstream>
#include <sstream>

#include "Analysis/Parser.h"

#include "Data/Object.h"
#include "Data/Value.h"

using namespace LayoutParser;

LayoutCollection::~LayoutCollection()
{
	for (auto& pair : m_Layouts)
		for (auto& object : pair.second)
			delete object;
}

LayoutCollection LayoutCollection::LoadFromString(const std::string& text)
{
	Parser parser(text);
	return LayoutCollection(std::move(parser.Parse()), std::move(parser.GetDiagnostics()));
}

LayoutCollection LayoutParser::LayoutCollection::LoadFromFile(const std::string& filePath)
{
	std::ifstream inputFile(filePath, std::ios::in);
	std::stringstream fileTextStream;

	fileTextStream << inputFile.rdbuf();
	inputFile.close();

	return LoadFromString(fileTextStream.str());
}

// Pretty print source code

#ifndef LAYOUTPARSER_EXCLUDE_PRETTYPRINT

#include <iostream>
#include <io.h> 
#include <fcntl.h>

std::wstring PrettyString(const Value* value)
{
	switch (value->GetKind())
	{
	case ValueKind::Object:
		return L"ObjectValue";
	case ValueKind::String:
	{
		auto& str = value->AsString()->GetValue();
		std::wstringstream returnString;
		returnString << L"StringValue(" << std::wstring(str.begin(), str.end()) << L")";
		return returnString.str();
	}
	case ValueKind::Number:
	{
		std::wstringstream returnString;
		returnString << L"NumberValue(" << value->AsNumber()->GetValue() << L")";
		return returnString.str();
	}
	case ValueKind::HexColor:
	{
		auto hexColor = value->AsHexColor();
		std::wstringstream returnString;
		returnString << L"HexColorValue(" <<
			static_cast<int32_t>(hexColor->GetR()) << L", " <<
			static_cast<int32_t>(hexColor->GetG()) << L", " <<
			static_cast<int32_t>(hexColor->GetB()) << L")";
		return returnString.str();
	}
	case ValueKind::List:
		return L"ListValue";
	case ValueKind::Dictionary:
		return L"DictionaryValue";
	default:
		return L"BadValue";
		break;
	}
}

void LayoutCollection::PrettyPrint(const Value* value, const std::wstring& propertyName, std::wstring indent, bool isLast)
{
	const wchar_t* marker = isLast ? L"\u2514\u2500\u2500" : L"\u251C\u2500\u2500";

	if (!propertyName.empty())
		std::wcout << indent << marker << propertyName << L": ";
	else
		std::wcout << indent << marker;

	indent.append(isLast ? L"   " : L"\u2502  ");

	std::wstring formattedValue = PrettyString(value);
	switch (value->GetKind())
	{
	case ValueKind::Object:
		std::wcout << formattedValue << L'\n';
		PrettyPrint(value->AsObject()->GetValue(), indent, true);
		return;
	case ValueKind::String:
		std::wcout << formattedValue << L'\n';
		return;
	case ValueKind::Number:
		std::wcout << formattedValue << L'\n';
		return;
	case ValueKind::HexColor:
		std::wcout << formattedValue << L'\n';
		return;
	case ValueKind::List:
	{
		std::wcout << formattedValue << L'\n';
		auto list = value->AsList();
		if (!list->IsEmpty())
		{
			auto last = list->LastValue();

			for (auto& value : *list)
				PrettyPrint(value, L"", indent, value == last);
		}
		return;
	}
	case ValueKind::Dictionary:
	{
		std::wcout << formattedValue << L'\n';
		auto dictionary = value->AsDictionary();
		if (!dictionary->IsEmpty())
		{
			auto last = dictionary->LastValue();

			for (auto& pair : *dictionary)
				PrettyPrint(pair.second, std::wstring(pair.first.begin(), pair.first.end()), indent, pair.second == last);
		}
	}
	default:
		break;
	}
}

void LayoutCollection::PrettyPrint(const Object* object, std::wstring indent, bool isLast)
{
	const wchar_t* marker = isLast ? L"\u2514\u2500\u2500" : L"\u251C\u2500\u2500";

	std::wcout << indent << marker;

	auto& identifier = object->GetIdentifier();
	if (object->GetConstructor() == nullptr)
		std::wcout << std::wstring(identifier.begin(), identifier.end()) << L'\n';
	else
		std::wcout << std::wstring(identifier.begin(), identifier.end()) << L'(' << PrettyString(object->GetConstructor()) << L")\n";

	indent.append(isLast ? L"   " : L"\u2502  ");

	if (!object->IsEmpty())
	{
		auto last = object->LastProperty();

		for (auto& pair : *object)
			PrettyPrint(pair.second, std::wstring(pair.first.begin(), pair.first.end()), indent, pair.second == last);
	}
}

void LayoutCollection::PrettyPrint(const Layout* layout, const std::wstring& layoutName, std::wstring indent, bool isLast)
{
	const wchar_t* marker = isLast ? L"\u2514\u2500\u2500" : L"\u251C\u2500\u2500";

	std::wcout << indent << marker << layoutName << L'\n';

	indent.append(isLast ? L"   " : L"\u2502  ");

	if (!layout->IsEmpty())
	{
		auto last = layout->LastObject();

		for (auto& object : *layout)
			PrettyPrint(object, indent, object == last);
	}
}

void LayoutCollection::PrettyPrint(const LayoutCollection& collection, std::wstring indent, bool isLast)
{
	int32_t previousMode = _setmode(_fileno(stdout), _O_U8TEXT);

	const wchar_t* marker = isLast ? L"\u2514\u2500\u2500" : L"\u251C\u2500\u2500";

	std::wcout << indent << marker << L"Collection" << L'\n';

	indent.append(isLast ? L"   " : L"\u2502  ");

	if (!collection.IsEmpty())
	{
		auto last = &collection.LastLayout();

		for (auto& pair : collection.m_Layouts)
			PrettyPrint(&pair.second, std::wstring(pair.first.begin(), pair.first.end()), indent, &pair.second == last);
	}

	(void)_setmode(_fileno(stdout), previousMode);
}

#endif