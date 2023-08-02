
#include <iostream>
#define INIT_SIZE 10
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

template <typename T> 
class vector {
	T* arr;
	size_t capacity;
	size_t current;
public:
	vector()
	{
		arr = new T[INIT_SIZE];
		capacity = INIT_SIZE;
		current = 0;
	}
	~vector() {delete [] arr;}

	void push(T data)
	{
		if (current >= capacity) {
			T* temp = new T[capacity * 2];

			for (int i = 0; i < capacity; i++)
				temp[i] = arr[i];

			delete[] arr;
			capacity *= 2;
			arr = temp;
		}
		arr[current] = data;
		++current;
	}

    void insert(size_t index, const T& data) {
        if (index > current) 
        {
            throw std::out_of_range("Index out of range");
        }

        if (current >= capacity) 
        {
            // Resize
            T* temp = new T[capacity * 2];
            for (size_t i = 0; i < current; ++i)
                temp[i] = arr[i];

            delete[] arr;
			capacity *= 2;
			arr = temp;
        }

        // Shift elements to the right
        for (size_t i = current; i > index; --i) {
            arr[i] = arr[i - 1];
        }

        arr[index] = data;
        ++current;
    }

    T& operator[](size_t index) 
    {
        if (index >= current) 
        {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

	int size() { return current; }
};

// Driver code
int main()
{
	vector<int> v;
	vector<char> v1;
	v.push(10);
	v.push(20);
	v.push(30);
	v.push(40);
	v.push(50);
	v1.push(71);
	v1.push(72);
	v1.push(73);
	v1.push(74);

	cout << "Vector size : " << v.size() << endl;
	cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << endl;

    v.insert(2, 35);

    cout << "Vector size : " << v.size() << endl;
	cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " " << v[4] << " " << v[5] << endl;

    vector<shared_ptr<int>> vecshar;

	return 0;
}
