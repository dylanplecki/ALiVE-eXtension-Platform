
// Program Headers
#include "axp/stdafx.h"
#include "axp/sqf_object.h"

// Boost Headers
#include <boost/utility/string_ref.hpp> 

namespace axp
{
	namespace sqf
	{
		void variable::copy_data(const variable &from)
		{
			type_ = from.type_;
			
			// Copy data
			switch (type_)
			{
			case array:
				data_ = new std::vector<sqf::variable>(*((std::vector<sqf::variable>*)from.data_));
				break;

			case boolean:
				data_ = new bool(*((bool*)from.data_));
				break;
			
			case control: case display: case group: case object:
				data_ = new uint32_t(*((uint32_t*)from.data_));
				break;
			
			case scalar:
				data_ = new float(*((float*)from.data_));
				break;

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				data_ = new std::string(*((std::string*)from.data_));
				break;

			default:
				data_ = nullptr;
				break;
			}
		}


		variable::variable()
		{
			type_ = nil;
			data_ = nullptr;
		}


		variable::variable(const data_type &data_type)
		{
			type_ = data_type;

			switch (data_type)
			{
			case array:
				data_ = new std::vector<sqf::variable>;
				break;

			case boolean:
				data_ = new bool;
				break;

			case control: case display: case group: case object:
				data_ = new uint32_t;
				break;
			
			case scalar:
				data_ = new float;
				break;

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				data_ = new std::string;
				break;

			default:
				data_ = nullptr;
				break;
			}
		}
		

		variable::variable(const variable &ref_object)
		{
			copy_data(ref_object);
		}


		variable::variable(const std::string &parse_string)
		{
			std::string* str_data;

			if (parse_string.size() <= 0)
				type_ = nil;
			else
			{
				str_data = new std::string(parse_string);
				type_ = static_cast<data_type>((*str_data)[0]);
				str_data->erase(0, 1);
			}

			switch (type_)
			{
			case array:
				{
					std::vector<sqf::variable> var_array = *(new std::vector<sqf::variable>);

					size_t start_pos = 0;

					for (size_t cur_pos = 0; (cur_pos < str_data->size()) && ((*str_data)[cur_pos] != 30); ++cur_pos) // Record Separator (UTF+001E &#30)
					{
						if ((*str_data)[cur_pos] == 31) // Unit Separator (UTF+001F &#31)
						{
							var_array.emplace_back(str_data->substr(start_pos, cur_pos));
							start_pos = cur_pos + 1;
						}
					}

					data_ = &var_array;
				}

				delete str_data;
				break;

			case boolean:
				data_ = new bool(((*str_data)[0] != '0') && (*str_data != "false"));
				delete str_data;
				break;

			case control: case display: case group: case object:
				data_ = new uint32_t(std::stoul(*str_data));
				delete str_data;
				break;

			case scalar:
				data_ = new float(std::stof(*str_data));
				delete str_data;
				break;

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				data_ = str_data;
				break;

			default:
				data_ = nullptr;
				delete str_data;
				break;
			}
		}


		variable::variable(const data_type &type, void* value)
		{
			type_ = type;
			data_ = value;
		}


		variable::~variable()
		{
			if (data_)
				delete data_;
		}



		data_type variable::type() const
		{
			return type_;
		}


		std::string variable::compile() const
		{
			switch (type_)
			{
			case string: case text: case script:
				return ('"' + this->to_string() + '"'); // Encapsulate with quotes

			case nil:
				return "nil";

			default:
				return this->to_string();
			}
		}


		std::string variable::type_name() const
		{
			return data_type_names[type_];
		}


		bool variable::is_kind_of(const data_type &type) const
		{
			return (type_ == type);
		}


		bool variable::is_kind_of(const variable &op_var) const
		{
			return (this->type_ == op_var.type_);
		}


		variable::operator bool() const
		{
			if (!data_)
				return false;

			switch (type_)
			{
			case scalar:
				return (*((float*)data_) == 0.0f);

			default:
				return (to_int() == 0);
			}
		}


		variable variable::operator= (const variable &op_var)
		{
			if (data_)
				delete data_;
			copy_data(op_var);
			return *this;
		}


		bool variable::operator== (const variable &op_var) const
		{
			if (!this->data_ || !op_var.data_) // Either variable is nil
				return false;

			if (this->type_ != op_var.type_) // Different data types
				return false;

			if (this->data_ == op_var.data_) // Same pointer reference
				return true;

			// Assume same data type, non-nil and non-same; then compare
			switch (type_)
			{
			case array:
				return *((std::vector<sqf::variable>*)this->data_) == *((std::vector<sqf::variable>*)op_var.data_);

			case boolean:
				return *((bool*)this->data_) && *((bool*)op_var.data_);

			case control: case display: case group: case object:
				return *((uint32_t*)this->data_) == *((uint32_t*)op_var.data_);
			
			case scalar:
				return *((float*)this->data_) == *((float*)op_var.data_);

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				return *((std::string*)this->data_) == *((std::string*)op_var.data_);

			default:
				return false;
			}
		}


		float variable::to_float() const
		{
			if (!data_)
				return 0;

			switch (type_)
			{
			case scalar:
				return *((float*)data_);

			default:
				return (float)to_int();
			}
		}


		uint32_t variable::to_int() const
		{
			if (!data_)
				return 0;

			switch (type_)
			{
			case array:
				return ((std::vector<sqf::variable>*)data_)->size();

			case boolean:
				return  (uint32_t)*((bool*)data_);
			
			case control: case display: case group: case object:
				return *((uint32_t*)data_);

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				return ((std::string*)data_)->size();

			case scalar:
				return (uint32_t)*((float*)data_);

			default:
				return 0;
			}
		}


		std::string variable::to_string() const
		{
			if (!data_)
				return "";

			switch (type_)
			{
			case array:
				{
					std::string return_str;
					return_str.push_back('[');

					for (auto sqf_var : *((std::vector<sqf::variable>*)data_))
					{
						return_str.append(sqf_var.to_string());
						return_str.push_back(',');
					}

					if (return_str.back() == ',')
						return_str.back() = ']';
					else
						return_str.push_back(']');

					return return_str;
				}

			case boolean:
				return (*((bool*)data_)) ? "true" : "false";

			case control: case display: case group: case object:
				return std::to_string(*((uint32_t*)data_));
			
			case scalar:
				return std::to_string(*((float*)data_));

			case code: case config: case location: case script: case side: case string: case text: case name_space:
				return *((std::string*)data_);

			default:
				return "";
			}
		}


		std::vector<sqf::variable> variable::to_vector() const
		{
			if (!data_)
				return std::vector<sqf::variable>();

			switch (type_)
			{
			case array:
				return *((std::vector<sqf::variable>*)data_);

			default:
				return std::vector<sqf::variable>{*this};
			}
		}
	}
}
