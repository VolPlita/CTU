#include <iostream>
using namespace std;

template<class T>
class shared_ptr
{
private:
	T * ptr = nullptr;
	unsigned * refCount = nullptr;

public:
	shared_ptr() : ptr(nullptr), refCount(new unsigned(0)) // default constructor
	{}
	shared_ptr(T * ptr) : ptr(ptr), refCount(new unsigned(1)) // constructor
	{}
    ~shared_ptr() {__cleanup__();}

	/*** Copy Semantics ***/
	shared_ptr(const shared_ptr & obj) // copy constructor
	{
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			this->refCount++; // if the pointer is not null, increment the refCount
		}
	}

	shared_ptr& operator=(const shared_ptr & obj) // copy assignment
	{
		__cleanup__(); // cleanup any existing data
		// Assign incoming object's data to this object
		this->ptr = obj.ptr; // share the underlying pointer
		this->refCount = obj.refCount;
		if (nullptr != obj.ptr)
		{
			(*this->refCount)++; // if the pointer is not null, increment the refCount
		}
	}

	/*** Move Semantics ***/
	shared_ptr(shared_ptr && dyingObj) // move constructor
	{
		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}

	shared_ptr& operator=(shared_ptr && dyingObj) // move assignment
	{
		__cleanup__(); // cleanup any existing data
		
		this->ptr = dyingObj.ptr; // share the underlying pointer
		this->refCount = dyingObj.refCount;

		dyingObj.ptr = dyingObj.refCount = nullptr; // clean the dying object
	}
    
	unsigned get_count() const {return *refCount;}
	T* get() const {return this->ptr;}
	T* operator->() const {return this->ptr;}
	T& operator*() const {return this->ptr;}

private:
	void __cleanup__()
	{
		(*refCount)--;
		if (*refCount == 0)
		{
			if (ptr != nullptr)
				delete ptr;
			delete refCount;
		}
	}
};