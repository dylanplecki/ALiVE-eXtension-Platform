
// Program Headers
#include "axp/stdafx.h"
#include "axp/shared_ptr.h"


namespace axp
{
	template<class T>
	void shared_ptr<T>::increment(void* index)
	{
		if (!index) return;

		size_t use_count = 0;

		if (data_map_.count(index))
			use_count = data_map_[index];

		data_map_[index] = use_count + 1;
	}


	template<class T>
	void shared_ptr<T>::decrement(void* index)
	{
		if (!index) return;

		if (data_map_.count(index))
		{
			if (data_map_[index])
				data_map_[index] -= 1;
		}
	}


	template<class T>
	shared_ptr<T>::shared_ptr() : data_(nullptr)
	{
	}


	template<class T>
	shared_ptr<T>::shared_ptr(T* data) : data_(data)
	{
		increment(data_);
	}


	template<class T>
	shared_ptr<T>::~shared_ptr()
	{
		reset();
	}


	template<class T> // Copy
	shared_ptr<T>::shared_ptr(const shared_ptr& from) : data_(from.data_)
	{
		increment(data_);
	}


	template<class T> // Assignment
	shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& from)
	{
		if (data_)
			reset();

		set(from.data_);
	}


	template<class T>
	void shared_ptr<T>::set(T* data)
	{
		data_ = data;
		increment(data_);
	}


	template<class T>
	void shared_ptr<T>::reset()
	{
		decrement(data_);

		if (!data_map_[data_])
		{
			delete data_;
			data_map_.erase(data_);
		}

		data_ = nullptr;
	}


	template<class T>
	T* shared_ptr<T>::get() const
	{
		return data_;
	}


	template<class T>
	T& shared_ptr<T>::operator*() const
	{
		return *data_;
	}


	template<class T>
	T* shared_ptr<T>::operator->() const
	{
		return data_;
	}
}
