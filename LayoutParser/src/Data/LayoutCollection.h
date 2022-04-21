#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "../Analysis/Diagnostics.h"

namespace LayoutParser
{
	struct Object;
	struct Value;

	struct Layout
	{
	public:
		Layout(std::vector<Object*>&& objects)
			: m_Objects(std::move(objects)) {}
		Layout(const Layout& other)
			: m_Objects(other.m_Objects) {}

		inline const Object* FirstObject() const { return m_Objects.front(); }
		inline const Object* LastObject() const { return m_Objects.back(); }

		inline bool IsEmpty() const { return m_Objects.empty(); }

		inline const Object* operator[](const size_t index) const { return m_Objects.at(index); }

		inline Layout& operator=(const Layout& other) = default;
		inline Layout& operator=(Layout&& other) noexcept
		{
			if (this != &other)
				m_Objects = std::move(other.m_Objects);
			return *this;
		}

		auto begin() const { return m_Objects.begin(); }
		auto end() const { return m_Objects.end(); }

	private:
		std::vector<Object*> m_Objects;
	};

	class LayoutCollection
	{
	public:
		LayoutCollection(const LayoutCollection& other)
			: m_Layouts(other.m_Layouts), m_Diagnostics(other.m_Diagnostics) {}

		// The layouts themselves don't have ownership of the objects in case they are copied
		~LayoutCollection();

		static LayoutCollection LoadFromString(const std::string& text);
		static LayoutCollection LoadFromFile(const std::string& filePath);

		inline DiagnosticCollection& GetDiagnostics() { return m_Diagnostics; }

		inline const Layout& FirstLayout() const { return m_Layouts.begin()->second; }
		inline const Layout& LastLayout() const { return std::prev(m_Layouts.end())->second; }

		inline bool IsEmpty() const { return m_Layouts.empty(); }

		// These will both throw exceptions if the key is not found
		inline const Layout& operator[](const std::string& identifier) const { return m_Layouts.at(identifier); }
		inline const Layout& operator[](const char* identifier) const { return m_Layouts.at(identifier); }

		inline LayoutCollection& operator=(const LayoutCollection& other) = default;
		inline LayoutCollection& operator=(LayoutCollection&& other) noexcept
		{
			if (this != &other)
			{
				m_Layouts = std::move(other.m_Layouts);
				m_Diagnostics = std::move(other.m_Diagnostics);
			}
			return *this;
		}

		auto begin() const { return m_Layouts.begin(); }
		auto end() const { return m_Layouts.end(); }

#ifndef LAYOUTPARSER_EXCLUDE_PRETTYPRINT
		static void PrettyPrint(const LayoutCollection& collection, std::wstring indent = L"", bool isLast = true);
		static void PrettyPrint(const Layout* layout, const std::wstring& layoutName, std::wstring indent = L"", bool isLast = true);
		static void PrettyPrint(const Object* object, std::wstring indent, bool isLast);
		static void PrettyPrint(const Value* value, const std::wstring& propertyName, std::wstring indent = L"", bool isLast = true);
#endif

	private:
		LayoutCollection(std::unordered_map<std::string, Layout>&& layouts, DiagnosticCollection&& diagnostics)
			: m_Layouts(std::move(layouts)), m_Diagnostics(std::move(diagnostics)) {}
		
		std::unordered_map<std::string, Layout> m_Layouts;
		DiagnosticCollection m_Diagnostics;
	};
}