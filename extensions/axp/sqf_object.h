#pragma once

// Program Headers
#include <axp/linkages.h>

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

		LibLink extern const char* data_type_names[];

		class variable
		{
		protected:
			data_type type_;
			void* data_;

			void copy_data(const variable &from);

		public:
			LibLink variable();
			LibLink variable(const data_type &data_type);
			LibLink variable(const variable &ref_object);
			LibLink variable(const std::string &parse_string); // passed data_type is index+1
			LibLink variable(const data_type &type, void* value);
			LibLink ~variable();

			// General Behaviors
			LibLink data_type type() const;
			LibLink std::string compile() const;
			LibLink std::string type_name() const;
			LibLink bool is_kind_of(const data_type &type) const;
			LibLink bool is_kind_of(const variable &op_var) const;

			// General Operators
			LibLink operator bool() const;								// Boolean
			LibLink variable operator= (const variable &op_var);		// Assignment
			LibLink bool operator== (const variable &op_var) const;		// Equal to

			// General Accessors
			LibLink float to_float() const;
			LibLink uint32_t to_int() const;
			LibLink std::string to_string() const;
			LibLink std::vector<sqf::variable> to_vector() const;
		};
	}
}
