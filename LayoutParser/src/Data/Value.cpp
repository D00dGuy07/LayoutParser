#include "Data/Value.h"

#include "Data/Object.h"

using namespace LayoutParser;

LayoutParser::ObjectValue::~ObjectValue()
{
	delete m_Object;
}

LayoutParser::ListValue::~ListValue()
{
	for (auto& object : m_List)
		delete object;
}

LayoutParser::DictionaryValue::~DictionaryValue()
{
	for (auto& pair : m_Dictionary)
		delete pair.second;
}
