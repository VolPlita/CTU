//The task is to create a template of the CSparseMatrix class that will represent a sparse 2D matrix
// with elements of a generic data type. The dimensions of the matrix are given from the start,
// but 99% of the matrix will never be used, so you need to design a memory-efficient way to represent it.
// The required interface of the CSparseMatrix class is as follows:
//
//data type T_
//
//any data type that makes up the individual elements/values in an array.
// It is guaranteed to be DefaultConstructible, CopyConstructible, MoveConstructible,
// and Destructible. Also, that type already has the == operator and the + operator between the elements of that type.
//
//constructor ( row, col )
//
//initializes the matrix with the size of the individual dimensions according to the values entered as parameters,
// the individual elements inside the matrix are initialized by default/implicitly.
//
//destructor
//
//deallocates used resources
//
//copy constructor
//
//initializes the array as a deep copy of the passed instance.
// The new matrix must be independent of the original object, i.e. after changing one instance,
// the other instance must not change.
//
//assignment operator
//
//replaces the current instance with the passed CSparseMatrix object.
// The dimensions and stored elements must be taken from the passed instance,
// the new content must be independent of the original object, i.e. after changing one instance,
// the other instance must not change.
//
//operator () ( row, col )
//
//accesses and returns an element at the specified index.
// If an index/indexes that are outside the range of the array are specified,
// it will throw an out_of_range exception.
//
//Submit the source code with a completed implementation of the required class.
// In the attached archive you will find the declaration of the required class and the main function with a basic test.
//
//Your implementation must handle destructor, copy constructor and operator =, but if you design a suitable container,
// their default form will suffice
//        Ideally, you don't need to use the new and delete operators even once
//You have several stl containers to use, but there are no sets or maps for example
//        Don't try to store the whole matrix, or the array of pointers to elements,
//        this solution will not go through the memory limit
//Search using operator() and contains() must be faster than linear

// Maybe HashSet?

#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <string>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CSparseMatrix
{
    vector<list<pair<int, T_>>> matrix; // vector of lists of pairs (column - value)
    int max_row;
    int max_col;

public:
    // constructor
    CSparseMatrix(int r, int c) : max_row(r), max_col(c){}
    // destructor, operator =, copy constructor if needed
    CSparseMatrix& operator = (CSparseMatrix & another)
    {
        for (int i = 0, l = matrix.size(); i < l; ++i)
        {
            matrix[i].clear();
        }
        matrix.clear();

        matrix = another.matrix;
        // Probably unnecessary
        for (int i = 0, l = another.matrix.size(); i < l; ++i)
        {
            matrix[i] = another.matrix[i]; // Copying lists
        }
        max_row = another.max_row;
        max_col = another.max_col;

        return *this;
    }
    // operator ( row, col )
    T_& operator () (int row, int col)
    {
        if (row >= max_row || col >= max_col) throw out_of_range("Indexes out of range");
        std::list<std::pair<int, T_>>& list_of_columns = matrix[row];

        if (list_of_columns.empty()) list_of_columns.emplace_front(col, T_());
        auto it = list_of_columns.begin();
        advance(it, col);
        if(it->first == col)
        {
            return it->second;
        }
        list_of_columns.emplace_front(col, T_());
        return list_of_columns.begin()->second;

//        for (auto it = matrix[row].begin(); it != matrix[row].end(); ++it)
//        {
//            if(it->first == col)
//            {
//                return it->second;
//            }
//        }
//        matrix[row].emplace_back(col, T_());
//        for (auto it = matrix[row].begin(); it != matrix[row].end(); ++it)
//        {
//            if(it->first == col)
//            {
//                return it->second;
//            }
//        }
    }
    // contains ( row, col )
    bool contains(int row, int col)
    {
        if (row >= max_row || col >= max_col) return false;
        std::list<std::pair<int, T_>>& list_of_columns = matrix[row];
        if (list_of_columns.empty()) return false;
        auto it = list_of_columns.begin();
        advance(it, col);
        if(it->first == col)
        {
            return true;
        }
        return false;
    }
};

#ifndef __PROGTEST__
int main ( void )
{
    CSparseMatrix<int> m1 ( 300, 700 );

    assert ( !m1 . contains ( 0, 0 ) );
    m1 ( 0, 0 ) = 123;
    m1 ( 1, 1 ) = 321;
    m1 ( 2, 2 ) = m1 ( 1, 1 ) + m1 ( 0, 0 );
    assert ( m1 . contains ( 0, 0 ) && m1 ( 0, 0 ) == 123 );
    assert ( m1 . contains ( 1, 1 ) && m1 ( 1, 1 ) == 321 );
    assert ( m1 . contains ( 2, 2 ) && m1 ( 2, 2 ) == 444 );

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            m1 ( i, j ) = i * j;

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            assert ( m1 . contains ( i, j ) && m1 ( i, j ) == i * j );

    try
    {
        m1 ( 300, 300 ) = 666;
        assert ( false );
    }
    catch ( out_of_range & o )
    {
        assert ( o . what() == "Index error"s );
    }
    catch ( ... )
    {
        assert ( false );
    }

    try
    {
        m1 ( 299, 300 ) = 322;
    }
    catch ( ... )
    {
        assert ( false );
    }

    CSparseMatrix m2 = m1;

    for ( int i = 0; i < 300; ++i )
        for ( int j = 0; j < 300; ++j )
            assert ( m2 . contains( i, j ) && m2 ( i, j ) == i * j );
    m1 ( 0, 0 ) = 15;

    assert ( m2 ( 0, 0 ) == 0 );
    assert ( m1 ( 0, 0 ) == 15 );

    CSparseMatrix <int> m3 ( 1000, 1000 );
    assert ( !m3 . contains ( 666, 666 ) );
    m3 ( 666, 666 ) = 666;
    assert ( m3 . contains ( 666, 666 ) && m3 ( 666, 666 ) == 666 );
    m3 = m1;
    assert ( !m3 . contains ( 666, 666 ) );
    assert ( m3 . contains ( 0, 0 ) && m3 ( 0, 0 ) == 15 );

    for ( int i = 1; i < 300; ++i )
        for ( int j = 1; j < 300; ++j )
            assert ( m3 . contains( i, j ) && m3 ( i, j ) == i * j );

    CSparseMatrix <string> m4 ( 1000000, 1000000 );
    assert ( !m4 . contains ( 0, 0 ) );
    assert ( !m4 . contains ( 999999, 999999 ) );
    m4 ( 0, 0 ) = "P";
    m4 ( 10, 40 ) = "r";
    m4 ( 55, 50000 ) = "o";
    m4 ( 400000, 11 ) = "g";
    m4 ( 999999, 1221 ) = "t";
    m4 ( 121, 169 ) = "e";
    m4 ( 66, 6 ) = "s";
    m4 ( 6, 66 ) = "t";
    assert ( m4 . contains ( 0, 0 ) && m4 ( 0, 0 ) == "P"s );
    assert ( m4 . contains ( 10, 40 ) && m4 ( 10, 40 ) == "r"s );
    assert ( m4 . contains ( 55, 50000 ) && m4 ( 55, 50000 ) == "o"s );
    assert ( m4 . contains ( 400000, 11 ) && m4 ( 400000, 11 ) == "g"s );
    assert ( m4 . contains ( 999999, 1221 ) && m4 ( 999999, 1221 ) == "t"s );
    assert ( m4 . contains ( 121, 169 ) && m4 ( 121, 169 ) == "e"s );
    assert ( m4 . contains ( 66, 6 ) && m4 ( 66, 6 ) == "s"s );
    assert ( m4 . contains ( 6, 66 ) && m4 ( 6, 66 ) == "t"s );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */

