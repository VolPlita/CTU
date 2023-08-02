#ifndef __PROGTEST__
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
#endif /* __PROGTEST__ */

template <typename T_>
class CSparseMatrix
{
public:
    /*! Constructor
     *  @param rows max rows atrix will have
     *  @param cols max columns atrix will have
     */
    CSparseMatrix ( size_t rows,
                    size_t cols )
            : m_Rows ( rows ),
              m_Cols ( cols )
    {}

    /*! Fuction that returns reference to element on ( row, col ) position
     *  ( Conastant variant was not necessary )
     *  @param row row
     *  @param col column
     *  @return non-const reference to the element on ( row, col ) position
     *  @throw std::out_of_range if given row or col index is bigger that matrix size
     */
    T_ & operator() ( size_t row,
                      size_t col )
    {
        //! We shouldn't have to check if < 0
        if ( row >= m_Rows || col >= m_Cols )
            throw out_of_range ( "Index error" );

        //! Using lower_bound to find a row we need
        auto it1 = lower_bound ( m_Vec . begin(), m_Vec . end(),
                                 make_pair ( row, vector < pair < size_t, T_ > > {} ),
                                 []( const pair < size_t, vector < pair < size_t, T_ > > > & a,
                                     const pair < size_t, vector < pair < size_t, T_ > > > & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! If our table does not contain such row create it
        if (   it1 == m_Vec . end()
               || (*it1) . first != row )
            m_Vec . insert ( it1, make_pair ( row, vector < pair < size_t, T_ > >{} ) );

        // ! Find this row after insertion ( probably can be replaced with --it1 )
        auto it2 = lower_bound ( m_Vec . begin(), m_Vec . end(),
                                 make_pair ( row, vector < pair < size_t, T_ > > {} ),
                                 []( const pair < size_t, vector < pair < size_t, T_ > > > & a,
                                     const pair < size_t, vector < pair < size_t, T_ > > > & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! Using lower_bound to find a column we need
        auto it3 = lower_bound ( (*it2) . second . begin() , (*it2) . second . end(),
                                 make_pair ( col, T_ () ),
                                 []( const pair < size_t, T_ > & a,
                                     const pair < size_t, T_ > & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! If our table does not contain such column create it
        if (   it3 == (*it2) . second . end()
               || (*it3) . first != col )
            (*it2) . second . insert ( it3, make_pair ( col, T_ () ) );

        // ! Find this column after insertion ( probably can be replaced with --it3 )
        auto it4 = lower_bound ( (*it2) . second . begin() , (*it2) . second . end(),
                                 make_pair ( col, T_() ),
                                 []( const pair < size_t, T_ > & a,
                                     const pair < size_t, T_ > & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! Now we can return a reference to this element
        return (*it4) . second;
    }

    /*! Fuction checkes if position ( row, col ) was previously used by operator()
     *  @param row row
     *  @param col col
     *  @return true if it was used and false if it was not
     */
    bool contains ( size_t row,
                    size_t col )
    {
        //! Using lower_bound to find a row we need
        auto it1 = lower_bound ( m_Vec . begin(), m_Vec . end(),
                                 make_pair ( row, vector < pair < size_t, T_ > > {} ),
                                 []( const pair <size_t, vector<pair<size_t, T_>>> & a,
                                     const pair <size_t, vector<pair<size_t, T_>>> & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! If there is ho such row return false
        if (   it1 == m_Vec . end()
               || (*it1) . first != row )
            return false;

        //! Using lower_bound to find a column we need
        auto it2 = lower_bound ( (*it1) . second . begin() , (*it1) . second . end(),
                                 make_pair ( col, T_() ),
                                 []( const pair < size_t, T_ > & a,
                                     const pair < size_t, T_ > & b )
                                 {
                                     return a . first < b . first;
                                 } );

        //! If there is ho such column return false
        if (   it2 == (*it1) . second . end()
               || (*it2) . first != col )
            return false;

        //! If we are here, there is an element of this poisition so return true
        return true;
    }
private:
    size_t m_Rows;
    size_t m_Cols;
    //! Vector that represents the matrix itself
    //! First pair represents one row
    //! First member is a row's index, second is a vector of pairs where first member is column's index
    //  and second is a vector of T_ elements
    vector <pair< size_t, vector < pair < size_t, T_ > > > > m_Vec;
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

