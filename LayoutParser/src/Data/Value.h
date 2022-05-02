#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace LayoutParser
{
	struct Object;

	struct ObjectValue;
	struct StringValue;
	struct NumberValue;
	struct BooleanValue;
	struct HexColorValue;
	struct ListValue;
	struct DictionaryValue;

	enum class ValueKind
	{
		Object,
		String,
		Number,
		Boolean,
		HexColor,
		List,
		Dictionary
	};

	struct Value
	{
	public:
		Value(ValueKind kind)
			: m_Kind(kind) {}
		virtual ~Value() {}

		inline const ObjectValue* AsObject() const 
		{ return (m_Kind == ValueKind::Object) ? reinterpret_cast<const ObjectValue*>(this) : nullptr; }

		inline const StringValue* AsString() const 
		{ return (m_Kind == ValueKind::String) ? reinterpret_cast<const StringValue*>(this) : nullptr; }

		inline const NumberValue* AsNumber() const 
		{ return (m_Kind == ValueKind::Number) ? reinterpret_cast<const NumberValue*>(this) : nullptr; }

		inline const BooleanValue* AsBoolean() const
		{ return (m_Kind == ValueKind::Boolean) ? reinterpret_cast<const BooleanValue*>(this) : nullptr; }

		inline const HexColorValue* AsHexColor() const 
		{ return (m_Kind == ValueKind::HexColor) ? reinterpret_cast<const HexColorValue*>(this) : nullptr; }

		inline const ListValue* AsList() const 
		{ return (m_Kind == ValueKind::List) ? reinterpret_cast<const ListValue*>(this) : nullptr; }

		inline const DictionaryValue* AsDictionary() const 
		{ return (m_Kind == ValueKind::Dictionary) ? reinterpret_cast<const DictionaryValue*>(this) : nullptr; }

		inline ValueKind GetKind() const { return m_Kind; }

	private:
		ValueKind m_Kind;
	};

	struct ObjectValue : public Value
	{
	public:
		ObjectValue(Object* object)
			: Value(ValueKind::Object), m_Object(object) {}
		virtual ~ObjectValue() override;

		inline const Object* GetValue() const { return m_Object; }

	private:
		Object* m_Object;
	};

	struct StringValue : public Value
	{
	public:
		StringValue(const std::string& string)
			: Value(ValueKind::String), m_String(string) {}

		StringValue(std::string&& string)
			: Value(ValueKind::String), m_String(string) {}

		inline const std::string& GetValue() const { return m_String; }

	private:
		std::string m_String;
	};

	struct NumberValue : public Value
	{
	public:
		NumberValue(float number)
			: Value(ValueKind::Number), m_Number(number) {}

		inline float GetValue() const { return m_Number; }

		template<typename T>
		T GetValueAs() const { return static_cast<T>(m_Number); }

	private:
		float m_Number;
	};

	struct BooleanValue : public Value
	{
	public:
		BooleanValue(bool boolean)
			: Value(ValueKind::Boolean), m_Boolean(boolean) {}

		inline bool GetValue() const { return m_Boolean; }

	private:
		bool m_Boolean;
	};

	struct HexColorValue : public Value
	{
	public:
		HexColorValue(const std::string& stringRepresentation)
			: Value(ValueKind::HexColor)
		{
			// Expected input format: #RRGGBB
			R = static_cast<int8_t>(std::stoi(stringRepresentation.substr(1, 2), nullptr, 16));
			G = static_cast<int8_t>(std::stoi(stringRepresentation.substr(3, 2), nullptr, 16));
			B = static_cast<int8_t>(std::stoi(stringRepresentation.substr(5, 2), nullptr, 16));
		}

		inline uint8_t GetR() const { return R; }
		inline uint8_t GetG() const { return G; }
		inline uint8_t GetB() const { return B; }

	private:
		uint8_t R;
		uint8_t G;
		uint8_t B;
	};

	struct ListValue : public Value
	{
	public:
		ListValue(std::vector<const Value*>&& list)
			: Value(ValueKind::List), m_List(list) {}

		virtual ~ListValue() override;

		inline const std::vector<const Value*>& GetContainer() const { return m_List; }

		inline const Value* FirstValue() const { return m_List.front(); }
		inline const Value* LastValue() const { return m_List.back(); }

		inline bool IsEmpty() const { return m_List.empty(); }

		inline const Value* operator[](size_t identifier) const { return m_List.at(identifier); }

		auto begin() const { return m_List.begin(); }
		auto end() const { return m_List.end(); }

	private:
		std::vector<const Value*> m_List;
	};

	struct DictionaryValue : public Value
	{
	public:
		DictionaryValue(std::unordered_map<std::string, const Value*>&& dictionary)
			: Value(ValueKind::Dictionary), m_Dictionary(dictionary) {}

		virtual ~DictionaryValue() override;

		inline const std::unordered_map<std::string, const Value*>& GetContainer() const { return m_Dictionary; }

		inline const Value* FirstValue() const { return m_Dictionary.begin()->second; }
		inline const Value* LastValue() const { return std::prev(m_Dictionary.end())->second; }

		inline bool IsEmpty() const { return m_Dictionary.empty(); }

		inline const Value* operator[](const std::string& identifier) const { return m_Dictionary.at(identifier); }
		inline const Value* operator[](const char* identifier) const { return m_Dictionary.at(identifier); }

		auto begin() const { return m_Dictionary.begin(); }
		auto end() const { return m_Dictionary.end(); }

	private:
		std::unordered_map<std::string, const Value*> m_Dictionary;
	};
}