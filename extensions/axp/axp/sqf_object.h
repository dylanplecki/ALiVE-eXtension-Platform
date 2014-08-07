#pragma once

// STD Headers
#include <string>
#include <vector>
#include <stdint.h>


namespace axp
{
	namespace sqf
	{
		const enum data_type
		{
		/*	Type Name	// Data Storage Type			*/	
			array,		// std::vector<axp::sqf::variable>
			boolean,	// bool
			code,		// std::string
			config,		// std::string
			control,	// uint32_t (control id)
			display,	// uint32_t (display id)
			group,		// uint32_t (netid)	
			location,	// std::string
			nil,		// nullptr
			object,		// uint32_t (netid)
			scalar,		// float
			script,		// std::string
			side,		// std::string
			string,		// std::string
			text,		// std::string
			name_space	// std::string
		};

		static const char* data_type_names[] = { "ARRAY", "BOOL", "CODE", "CONFIG", "CONTROL", "DISPLAY", "GROUP", "LOCATION", "" /* "NIL" */, "OBJECT", "SCALAR", "SCRIPT", "SIDE", "STRING", "TEXT", "NAMESPACE" };

		class variable
		{
		protected:
			data_type type_;
			void* data_;

			void copy_data(const variable &from);

		public:
			variable();
			variable(const data_type &data_type);
			variable(const variable &ref_object);
			variable(const std::string &parse_string); // passed data_type is index+1
			variable(const data_type &type, void* value);
			~variable();

			// General Behaviors
			data_type type() const;
			std::string compile() const;
			std::string type_name() const;
			bool is_kind_of(const data_type &type) const;
			bool is_kind_of(const variable &op_var) const;

			// General Operators
			operator bool() const;								// Boolean
			variable operator= (const variable &op_var);		// Assignment
			bool operator== (const variable &op_var) const;		// Equal to

			// General Accessors
			float to_float() const;
			uint32_t to_int() const;
			std::string to_string() const;
			std::vector<sqf::variable> to_vector() const;
		};
	}
}
