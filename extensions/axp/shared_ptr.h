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
			}
		}


	public:
		shared_ptr() : data_(nullptr)
		{
		}


		shared_ptr(T* data) : data_(data)
		{
			increment(data_);
		}


		~shared_ptr()
		{
			reset();
		}


		shared_ptr(const shared_ptr& from) : data_(from.data_)
		{
			increment(data_);
		}


		shared_ptr& operator=(const shared_ptr& from)
		{
			if (data_)
				reset();

			set(from.data_);

			return *this;
		}


		void set(T* data)
		{
			data_ = data;
			increment(data_);
		}


		void reset()
		{
			decrement(data_);

			if (!data_map_[data_])
			{
				delete data_;
				data_map_.erase(data_);
			}

			data_ = nullptr;
		}


		T* get() const
		{
			return data_;
		}


		T& operator*() const
		{
			return *data_;
		}


		T* operator->() const
		{
			return data_;
		}
	};

	// Load static data members into memory
	template <typename T>
	std::map<void*, size_t> shared_ptr<T>::data_map_;
}
