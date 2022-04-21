#include "Data/Object.h"

#include "Data/Value.h"

using namespace LayoutParser;

LayoutParser::Object::~Object()
{
	delete m_Constructor;
	for (auto& pair : m_Properties)
		delete pair.second;
}