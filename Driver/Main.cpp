#include <iostream>
#include <string>

#include "LayoutParser/LayoutParser.h"

int main()
{
	LayoutParser::LayoutCollection layouts = LayoutParser::LayoutCollection::LoadFromFile("test.lp");

	for (const std::string& message : layouts.GetDiagnostics())
		std::cout << message << '\n';
	std::cout << '\n';

	LayoutParser::LayoutCollection::PrettyPrint(layouts);
}