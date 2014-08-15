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


		void increment(void* index)
		{
			if (!index) return;

			size_t use_count = 0;

			if (data_map_.count(index))
				use_count = data_map_[index];

			data_map_[index] = use_count + 1;
		}


		void decrement(void* index)
		{
			if (!index) return;

			if (data_map_.count(index))
			{
				if (data_map_[index])
					data_map_[index] -= 1;

				if (!data_map_[index])
				{
					delete index;
					data_map_.erase(index);
				}
			}
		}


	public:
		LibLink shared_ptr() : data_(nullptr)
		{
		}


		LibLink shared_ptr(T* data) : data_(data)
		{
			increment(data_);
		}


		LibLink ~shared_ptr()
		{
			decrement(data_);
		}


		LibLink shared_ptr(const shared_ptr& from) : data_(from.data_)
		{
			increment(data_);
		}


		LibLink shared_ptr& operator=(const shared_ptr& from)
		{
			if (data_)
				reset();

			set(from.data_);

			return *this;
		}


		LibLink void set(T* data)
		{
			reset();
			data_ = data;
			increment(data_);
		}


		LibLink void reset()
		{
			decrement(data_);
			data_ = nullptr;
		}


		LibLink T* get() const
		{
			return data_;
		}


		LibLink T& operator*() const
		{
			return *data_;
		}


		LibLink T* operator->() const
		{
			return data_;
		}


		LibLink operator bool() const
		{
			return (data_ != nullptr);
		}
	};

	// Load static data members into memory
	template <typename T>
	std::map<void*, size_t> shared_ptr<T>::data_map_;
}
