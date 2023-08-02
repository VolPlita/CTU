#include <iostream>
#include <cassert>
using namespace std;

class CoolArray 
{
    int* m_data;
    size_t m_capacity;

public:
    CoolArray()
        : m_data(nullptr)
        , m_capacity(0)
    {
    }

    CoolArray(const CoolArray& src)
        : m_data(nullptr)
        , m_capacity(src.m_capacity)
    {
        m_data = new int[m_capacity];
        copy(src.m_data, src.m_data + src.m_capacity, m_data);
    }

    CoolArray& operator=(const CoolArray& src)
    {
        if(&src == this) 
            return *this;

        delete[] m_data;

        m_capacity = src.m_capacity;
        m_data = new int[m_capacity];
        copy(src.m_data, src.m_data + src.m_capacity, m_data);
        
        return *this;
    }

    ~CoolArray()
    {
        delete[] m_data;
    }

    int& operator[](size_t index)
    {
        ensure(index);

        return m_data[index];
    }

    int operator[](size_t index) const
    {
        return m_data[index];
    }

private:
    void ensure(size_t index)
    {
        if(index < m_capacity)
            return;

        size_t oldCapacity = m_capacity;
        m_capacity = max(m_capacity * 2, index) + 1;

        int *newData = new int[m_capacity]{0};

        copy(m_data, m_data + oldCapacity, newData);

        delete[] m_data;
        m_data = newData;
    }
};

void DeepCopy_Test() 
{
    CoolArray arr1, arr2;
    arr1[666] = 100;
    assert(arr1[666] == 100);

    arr2 = arr1;
    assert(arr1[666] == 100);
    assert(arr2[666] == 100);
    
    CoolArray arr3 = arr1;
    assert(arr3[666] == 100);

    arr3[123] = 123;
    assert(arr1[123] != 123);
    assert(arr2[123] != 123);
    assert(arr3[123] == 123);

    arr1[666] = 666;
    assert(arr1[666] == 666);
    assert(arr2[666] == 100);
    assert(arr3[666] == 100);
    arr1[666] = 111;
    arr3 = arr2 = arr1;
    assert(arr1[666] == 111);
    assert(arr2[666] == 111);
    assert(arr3[666] == 111);

    arr1[6000] = 12345;
    assert(arr3[6000] != 12345);
}

void ShallowCopy_Test() 
{
    CoolArray arr1, arr2;
    arr1[666] = 100;

    arr2[123] = 123;
    // Assignment Operator
    arr2 = arr1;
    // Copy Constructor
    CoolArray arr3 = arr1;

    arr3[123] = 123;
    assert(arr1[123] == 123);
    assert(arr2[123] == 123);
    assert(arr3[123] == 123);

    assert(arr1[666] == 100);
    assert(arr2[666] == 100);
    assert(arr3[666] == 100);

    arr1[666] = 111;
    assert(arr1[666] == 111);
    assert(arr2[666] == 111);
    assert(arr3[666] == 111);
    //// shared after reallocation?
    // arr1[6000] = 12345;
    // assert(arr3[6000] == 12345);
}


void CopyOnWrite_Test() {
    CoolArray arr1, arr2;
    arr1[666] = 100;
    assert(arr1[666] == 100);

    arr2 = arr1;
    assert(arr1[666] == 100);
    assert(arr2[666] == 100);
    
    CoolArray arr3 = arr1;
    assert(arr3[666] == 100);

    arr3[123] = 123;
    assert(arr1[123] != 123);
    assert(arr2[123] != 123);
    assert(arr3[123] == 123);

    arr1[666] = 666;
    assert(arr1[666] == 666);
    assert(arr2[666] == 100);
    assert(arr3[666] == 100);
    arr1[666] = 111;
    arr3 = arr2 = arr1;
    assert(arr1[666] == 111);
    assert(arr2[666] == 111);
    assert(arr3[666] == 111);
}

int main()
{
    // TESTS
    {
        // comment out inappropriate tests
        DeepCopy_Test();
        // ShallowCopy_Test();
        // CopyOnWrite_Test();
    }

    std::cout << "All tests passed.\n";
    
    return 0;
}