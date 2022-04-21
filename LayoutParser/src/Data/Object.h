#pragma once

#include <string>
#include <unordered_map>

namespace LayoutParser
{
	struct Value;

	struct Object
	{
	public:
		Object(const std::string& identifier, Value* constructor, std::unordered_map<std::string, Value*>&& properties)
			: m_Identifier(identifier), m_Constructor(constructor), m_Properties(std::move(properties)) {}
		~Object();

		const std::string& GetIdentifier() const { return m_Identifier; }
		const Value* GetConstructor() const { return m_Constructor; }

		inline const Value* FirstProperty() const { return m_Properties.begin()->second; }
		inline const Value* LastProperty() const { return std::prev(m_Properties.end())->second; }

		inline bool IsEmpty() const { return m_Properties.empty(); }
		
		inline const Value* operator[](const std::string& identifier) const { return m_Properties.at(identifier); }
		inline const Value* operator[](const char* identifier) const { return m_Properties.at(identifier); }

		auto begin() const { return m_Properties.begin(); }
		auto end() const { return m_Properties.end(); }

	private:
		std::string m_Identifier;
		Value* m_Constructor;
		std::unordered_map<std::string, Value*> m_Properties;
	};
}