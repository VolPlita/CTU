//The task is to create a template of the CMatrix3 class that will represent
// a three-dimensional matrix with elements of a generic data type.
//
//The required interface of the CMatrix3 class is as follows:
//
//data type T_
// any data type that makes up the individual elements/values in an array.
// It has the properties DefaultConstructible, CopyConstructible, MoveConstructible
// and Destructible guaranteed. We also know that this type has an overloaded comparison operator for equality ( == )
// and addition ( + ) between elements of this type.
//
//constructor (unsigned, unsigned, unsigned)
//
//initializes the matrix with the size of the individual dimensions according to the values entered as parameters,
// the individual elements inside the matrix are initialized by default/implicitly.
//
//destructor
//
//frees resources allocated by an instance of the class.
//
//copy constructor
//
//initializes the array as a deep copy of the passed instance.
// The new matrix must be independent of the original object, i.e.
// after changing one instance, the other instance must not change.
//
//assignment operator
//
//replaces the current instance with the passed CMatrix3 object.
// The dimensions and stored elements must be taken from the passed instance,
// the new content must be independent of the original object, i.e. after changing one instance,
// the other instance must not change.
//
//indexing operator [][][]
//
//accesses and returns the element at the specified triple of indices.
// If an index/indexes that are outside the range of the array are specified, it will throw an out_of_range exception.
// C++ doesn't allow you to create this operator directly, so you'll have to think about the implementation.
//
//== and + operators between array elements
//
//the elements that are returned by accessing the array indices must be able to be compared for equality and added
// to each other (see examples in the template).
//
//Submit the source code with a completed implementation of the required class.
// In the attached archive you will find the declaration of the required class and the main function with a basic test.

#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CMatrix3
{
    // todo
    T_ *** matrix;
    unsigned x, y, z;
public:

    // constructor ( unsigned n1, unsigned n2, unsigned n3 )
    CMatrix3( unsigned n1, unsigned n2, unsigned n3 ) : x(n1), y(n2), z(n3)
    {
        matrix = new T_** [x];
        for (unsigned i = 0; i < x; ++i)
        {
            matrix[i] = new T_*[y];
            for (unsigned j = 0; j < y; ++j)
            {
                matrix[i][j] = new T_[z];
            }
        }
    }
    // destructor
    ~CMatrix3()
    {
        for (unsigned i = 0; i < x; ++i)
        {
            for (unsigned j = 0; j < y; ++j)
            {
                delete [] matrix[i][j];
            }
            delete [] matrix[i];
        }
        delete [] matrix;
    }
    // copy constructor
    CMatrix3(const CMatrix3 & another) : x(another.x), y(another.y), z(another.z)
    {
        matrix = new T_** [x];
        for (unsigned i = 0; i < x; ++i)
        {
            matrix[i] = new T_*[y];
            for (unsigned j = 0; j < y; ++j)
            {
                matrix[i][j] = new T_[z];
                for (unsigned k = 0; k < z; ++k)
                {
                    matrix[i][j][k] = another.matrix[i][j][k];
                }
            }
        }
    }
    // operator =
    CMatrix3 & operator = (const CMatrix3 & another)
    {
        for (unsigned i = 0; i < x; ++i)
        {
            for (unsigned j = 0; j < y; ++j)
            {
                delete [] matrix[i][j];
            }
            delete [] matrix[i];
        }
        delete [] matrix;

        x = another.x;
        y = another.y;
        z = another.z;

        matrix = new T_** [x];
        for (unsigned i = 0; i < x; ++i)
        {
            matrix[i] = new T_*[y];
            for (unsigned j = 0; j < y; ++j)
            {
                matrix[i][j] = new T_[z];
                for (unsigned k = 0; k < z; ++k)
                {
                    matrix[i][j][k] = another.matrix[i][j][k];
                }
            }
        }
        return *this;
    }
    // operator [][][]
    class CMatrix1
    {
        T_ * data;
        unsigned len_z;
    public:
        CMatrix1(T_* pointer, unsigned mz) : data(pointer), len_z(mz) {}
        T_& operator [] (unsigned index)
        {
            if(index >= len_z) throw out_of_range("Index x out of bounds");
            return data[index];
        }
    };
    class CMatrix2
    {
        T_ ** data;
        unsigned len_y;
        unsigned len_z;
    public:
        CMatrix2(T_** pointer, unsigned my, unsigned mz) : data(pointer), len_y(my), len_z(mz) {}
        CMatrix1 operator [] (unsigned index)
        {
            if(index >= len_y) throw out_of_range("Index y out of bounds");
            return CMatrix1(data[index], len_z);
        }
    };
    CMatrix2 operator [] (unsigned index)
    {
        if(index >= x) throw out_of_range("Index x out of bounds");
        return CMatrix2(matrix[index], y, z);
    }
    // operator +
    // operator ==
};

#ifndef __PROGTEST__
int main ( void )
{
    CMatrix3<int> m ( 3, 4, 5 );
    m[0][0][0] = 128;
    m[2][3][1] = 7;
    assert ( m[0][0][0] == 128 );
    assert ( m[2][3][1] == 7 );

    m[1][1][1] = m[0][0][0] + m[2][3][1];
    m[2][3][4] = 135;
    assert ( m[1][1][1] == m[2][3][4] );

    CMatrix3<int> copy ( m );
    try { copy[4][1][2]; assert ( "Missing exception for access i0 = 4, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[1][4][2]; assert ( "Missing exception for access i1 = 4, limit = 3" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }
    try { copy[2][1][6]; assert ( "Missing exception for access i2 = 6, limit = 4" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }

    copy[2][3][1] = 42;
    assert ( m[2][3][1] == 7 );
    assert ( copy[2][3][1] == 42 );

    CMatrix3<int> n ( 2, 2, 2 );
    n[0][1][0] = 56;
    n[1][0][1] = 42;
    m = n;
    n[0][1][0] = 0;
    assert ( m[0][1][0] == 56 );
    try { m[2][1][0]; assert ( "Missing exception for access i0 = 2, limit = 2" == nullptr ); }
    catch ( out_of_range & e ) { }
    catch ( ... ) { assert ( "Incorrect exception thrown" == nullptr ); }

    CMatrix3<string> s ( 2, 1, 1 );
    s[0][0][0] = "Hello ";
    s[1][0][0] = "World!";
    assert ( ( s[0][0][0] + s[1][0][0] ) == string("Hello World!") );

    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

