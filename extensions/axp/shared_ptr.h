#pragma once

// STD Headers
#include <map>


namespace axp
{
	template<class T>
	class shared_ptr
	{
	private:
		static std::map<void*, size_t> data_map_;
		T* data_;

		void increment(void* index);
		void decrement(void* index);

	public:
		shared_ptr(T* data);
		~shared_ptr();

		void set(T* data);
		void reset();
	};
}
