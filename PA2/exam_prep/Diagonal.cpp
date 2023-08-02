//The task is to implement a class template for diagonal matrices.
//
//The implemented template will represent a diagonal 2D matrix of values.
// The matrix will be square and have a fixed n x n size specified when created.
// From these values, it will be possible to work only with elements on the main diagonal
// and on the adjacent diagonals (see picture). The matrix will typically be large
// (large n, but the number of diagonals k will typically be small.
// Therefore, we want a memory-efficient implementation that will allocate memory
// space only for the elements on the desired diagonals.
//
//An array element can be any type that:
//
//has an implicit construction (DefaultConstructible),
//
//is copyable (CopyConstructible, CopyAssignable)
//
//is moveable (MoveConstructible, MoveAssignable)
//
//is destructible (Destructible).
//
//Additional operations may not be allowed by the matrix element
// (but they are not needed for the implementation anyway).
// Required interface of the CDiagonalMatrix class template:
//
//constructor ( n, k )
//
//initializes the object, remembers the size of the matrix and the number of diagonals.
// At the same time, it allocates space for the elements and fills them with their default value (T_()).
// The value of k must be at least 1 and at most n. If it is not within these limits,
// the constructor will throw a `std::out_of_range` exception.
//
//destructor
//
//frees the space allocated for the array. Only implement it if the automatically generated destructor is not enough.
//
//= operator, copy constructor
//
//will ensure that the matrix is correctly copied. Implement them only if the automatically generated variants are not enough.
//
//operator ( row, col )
//
//provides access to the elements of the matrix according to the specified pair of indices.
// We count the indices from 0. If the indices are entered correctly,
// the operator makes the required element of the matrix available. If any index is outside the allocated diagonals,
// the operator throws a std::out_of_range exception with the corresponding error description (see sample run).
//
//exists ( row, col ) const
//
//method checks if an element exists in the array at the specified index.
// The return value is true (the indices refer to a valid element) or
// false (the indices are out of bounds or the element does not exist on the allocated diagonals).
//
//reshape ( k )
//
//method changes the number of allocated diagonals in the matrix.
// The value of k must be within the valid range of 1 to n. If it is outside these limits,
// the method throws a std::out_of_range exception. If the new value is less than the original value,
// the method frees memory for unneeded previously allocated elements,
// but leaves the values of the elements on the retained diagonals unchanged. Analogously,
// if k is greater than the original value, the method preserves the values of the original elements,
// allocates the necessary space for the new elements, and fills the new elements with the default T_().

// ***PICTURE***

//There are some STL containers available in the test environment (see attached source code),
// it is reasonable to use them. When designing, consider the following:
//
//try to design the class so that you don't need to implement your own destructor/copy constructor/operator=,
//
//cannot create the entire 2D array of values. The test environment
// counts how many element instances you have allocated (see sample run),
//
//access to an array element must be fast. The () operator must have
// logarithmic complexity or better (ideally constant), the exists method should
// have constant complexity,
//
//the first speed test calls the reshape method infrequently, while speed test 2 calls it very often.

#ifndef __PROGTEST__

#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>
#include <memory>
#include <stdexcept>

using namespace std;
#endif /* __PROGTEST__ */

template<typename T_>
class CBoundedVector
{
    vector<T_> m_vec;
    unsigned m_length;
    unsigned m_padding;
public:
    CBoundedVector(unsigned l, unsigned p) : m_length(l), m_padding(p)
    {
        m_vec.resize(l);
    }
    CBoundedVector(const CBoundedVector & another) : m_length(another.m_length), m_padding(another.m_padding)
    {
        m_vec = another.m_vec;
    }
    CBoundedVector & operator = (const CBoundedVector & another)
    {
        m_vec = another.m_vec;
        m_length = another.m_length;
        m_padding = another.m_padding;
    }
    T_ & operator [] (unsigned col)
    {
        if (col - m_padding < 0 || col - m_padding >= m_length) throw out_of_range("Column out of range");
        return m_vec[col - m_padding];
    }
    const T_ & operator [] (unsigned col) const
    {
        if (col - m_padding < 0 || col - m_padding >= m_length) throw out_of_range("Column out of range");
        return m_vec[col - m_padding];
    }
    bool exists(unsigned col) const
    {
        if (col - m_padding >= 0 || col - m_padding < m_length) return true;
        return false;
    }
//    void remove_last(unsigned amount)
//    {
//        for (unsigned i = m_length - 1; i > m_length - 1 - amount; --i)
//        {
//            m_vec[i].remove();
//        }
//        m_length -= amount;
//        m_vec.resize(m_length);
//    }
    void remove_for_first(unsigned amount, unsigned max_len) // max_len = 2 * k - 1
    {
        m_vec.remove(m_length - amount, m_length - 1);
        m_vec.remove(0, (m_length - amount) - (max_len - 2 * amount) - 1);
        m_padding += m_length - amount - (max_len - 2 * amount);
        m_length -= m_vec.size();
        m_vec.resize(m_length); // Unnecessary
    }
    void remove_for_last(unsigned amount, unsigned max_len)
    {
        m_vec.remove(0, amount - 1);
        m_vec.remove((max_len - 2 * amount), m_length - 1);
        m_padding += amount;
        m_length = m_vec.size();
        m_vec.resize(m_length);
    }
//    void remove_both(unsigned amount)
//    {
//        for (unsigned i = m_length - 1; i > m_length - 1 - amount; --i)
//        {
//            m_vec[i].remove();
//        }
//        for (unsigned i = 0; i < amount; ++i)
//        {
//            m_vec[i].remove();
//        }
//        m_length -= amount * 2;
//        m_vec.resize(m_length);
//        m_padding += amount;
//    }
};

template<typename T_>
class CDiagonalMatrix {
    vector<CBoundedVector<T_>> matrix;
    unsigned m_n;
    unsigned m_k;
public:
    // constructor ( n, k )
    CDiagonalMatrix(unsigned n, unsigned k) : m_n(n), m_k(k)
    {
        for (unsigned i = 0; i < k; ++i)
            matrix.emplace_back(k + i, 0);
        for (unsigned i = k; i < n - k; ++i)
            matrix.emplace_back(k * 2 - 1, i - k);
        unsigned l = 0;
        for (unsigned i = n - k; i < n; ++i)
        {
            matrix.emplace_back(k * 2 - 1 - l, i - k);
            ++l;
        }
    }
    // destructor, copy cons, operator = (if needed)
    // operator () ( row, col )
    T_ & operator () (unsigned row, unsigned col)
    {
        if (row > m_n) throw out_of_range("Row out of range.");
        return matrix[row][col];
    }
    // exists ( row, col ) const
    bool exists (unsigned row, unsigned col) const
    {
        if (row < m_n && matrix[row].exists(col) && matrix[row][col] != T_()) return true;
        return false;
    }
    // reshape ( k )
    void reshape(unsigned k)
    {
        if (k == m_k) return;
        if (k > m_k)
        {
            // Enlarge
        }
        else
        {
            // Shrink
            unsigned shrink_amount = m_k - k;
        }
    }

private:
    // todo
};

#ifndef __PROGTEST__

class CTestCnt {
public:
    CTestCnt(int val = 0)
            : m_Val(val) {
        c_Inst++;
    }

    CTestCnt(const CTestCnt &src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    CTestCnt(CTestCnt &&src) noexcept
            : m_Val(src.m_Val) {
        c_Inst++;
    }

    ~CTestCnt(void) noexcept {
        c_Inst--;
    }

    CTestCnt &operator=(CTestCnt src) noexcept {
        swap(src.m_Val, m_Val);
        return *this;
    }

    inline static size_t c_Inst = 0;
private:
    int m_Val;
};

int main(void) {
    CDiagonalMatrix<int> m1(5, 4);

    for (int i = 0; i < 5; i++)
        m1(i, i) = 10 + i;
    for (int i = 0; i < 4; i++) {
        m1(i, i + 1) = 20 + i;
        m1(i + 1, i) = 30 + i;
    }
    for (int i = 0; i < 3; i++) {
        m1(i, i + 2) = 40 + i;
        m1(i + 2, i) = 50 + i;
    }
    m1(0, 3) = 60;
    m1(1, 4) = 61;
    m1(3, 0) = 70;
    m1(4, 1) = 71;

    CDiagonalMatrix<int> m2(m1);

    for (int i = 0; i < 5; i++)
        assert (m2(i, i) == 10 + i);
    for (int i = 0; i < 4; i++) {
        assert (m2(i, i + 1) == 20 + i);
        assert (m2(i + 1, i) == 30 + i);
    }
    for (int i = 0; i < 3; i++) {
        assert (m2(i, i + 2) == 40 + i);
        assert (m2(i + 2, i) == 50 + i);
    }
    assert (m2(0, 3) == 60);
    assert (m2(1, 4) == 61);
    assert (m2(3, 0) == 70);
    assert (m2(4, 1) == 71);


    m2(0, 0) = 100;
    assert (m1(0, 0) == 10);
    assert (m2(0, 0) == 100);


    assert (m1.exists(4, 1) && m1(4, 1) == 71);
    m1.reshape(2);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (!m1.exists(0, 2));
    try {
        m1(0, 0) = m1(0, 2);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        assert (e.what() == "invalid index ( 0, 2 )"s);
    }


    m1.reshape(4);
    assert (m1.exists(0, 1) && m1(0, 1) == 20);
    assert (m1.exists(0, 2) && m1(0, 2) == 0);

    try {
        m1.reshape(6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        assert (e.what() == "invalid matrix size"s);
    }

    try {
        CDiagonalMatrix<int> m3(5, 6);
        assert ("Missing an exception" == nullptr);
    }
    catch (const std::out_of_range &e) {
        assert (e.what() == "invalid matrix size"s);
    }


    CDiagonalMatrix<CTestCnt> m10(12, 1);
    assert (CTestCnt::c_Inst == 12);
    m10.reshape(4);
    assert (CTestCnt::c_Inst == 72);
    m10.reshape(2);
    assert (CTestCnt::c_Inst == 34);
    m10.reshape(11);
    assert (CTestCnt::c_Inst == 142);
    m10.reshape(12);
    assert (CTestCnt::c_Inst == 144);
    m10.reshape(8);
    assert (CTestCnt::c_Inst == 124);

    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */