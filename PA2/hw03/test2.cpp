#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <ctime>
#include <climits>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange
{
public:
	CRange (long long s, long long e)
	{
		if (s <= e) 
		{
			m_start = s;
			m_end = e;
		}
		else throw logic_error("Lower bound needs to be smaller than or equal to the upper bound");
	}
    long long get_start() const {return m_start;}
	long long get_end() const {return m_end;}

	void set_start(long long a)
	{
		if (a <= m_end) m_start = a;
		else throw logic_error("You can't do this");
	}
	void set_end(long long a)
	{
		if (a >= m_start) m_end = a;
		else throw logic_error("You can't do this");
	}
	bool in_bounds(long long val) const
	{
		if (m_start == LLONG_MIN || m_end == LLONG_MAX) return true;
		return (val >= (m_start - 1) && val <= (m_end + 1));
	}
	bool in_bounds(CRange interval) const // Shows if another is in bounds of this
	{
		if (m_start == LLONG_MIN || m_end == LLONG_MAX) return true;
		return (interval.get_start() >= (m_start - 1) && interval.get_end() <= (m_end + 1));
	}
	int direction(CRange another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (m_start != LLONG_MIN && another.get_end() < (m_start - 1)) return -1;
        if (m_end != LLONG_MAX && another.get_start() > (m_end + 1)) return 1;
        else return 0;
    }
	int direction(long long another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (m_start != LLONG_MIN && another < (m_start - 1)) return -1;
        if (m_end != LLONG_MAX && another > (m_end + 1)) return 1;
        else return 0;
    }
	// Needed operators
	bool operator == (const CRange & another) const
	{
		return m_start == another.get_start() && m_end == another.get_end();
	}
	bool operator != (const CRange & another) const
	{
		return m_start != another.get_start() || m_end != another.get_end();
	}
    // Merging function
    bool merge(CRange & another) // If mergable - modify this & return true, in no return false
    {
        if (this->in_bounds(another.get_end()) || this->in_bounds(another.get_start()) || 
            another.in_bounds(this->get_start()) || another.in_bounds(this->get_end()))
        {
            m_start = min(m_start, another.get_start());
            m_end = max(m_end, another.get_end());
            return true;
        }
        return false;
    }

private:
	long long m_start;
	long long m_end;
};
struct compareVal // Returns true if a is strictly before b
{
	bool operator ()(const CRange a, const long long b) const
	{
		if (b == LLONG_MIN || a.get_end() == LLONG_MAX) return false;
		return (a.get_end() < (b - 1));
	}
};

class CRangeList
{
  public:
    // constructor
	CRangeList () = default;
    // includes long long / range
	bool includes(long long val) const
	{
		auto ran = lower_bound(m_list.begin(), m_list.end(), val, compareVal());
        if (ran == m_list.end()) return false;
		if (val >= ran->get_start() && val <= ran->get_end()) return true;
		return false;
	}
	bool includes (CRange interval) const
	{
		auto ran_s = lower_bound(m_list.begin(), m_list.end(), interval.get_start(), compareVal());
        if (ran_s == m_list.end()) return false;
		if (interval.get_start() >= ran_s->get_start() && interval.get_end() <= ran_s->get_end())
			return true;
		return false;
	}
    // += range / range list
	CRangeList & operator += (CRange interval)
	{
		if (m_list.empty()) {m_list.push_back(interval); return *this;}
		auto bound_start = lower_bound(m_list.begin(), m_list.end(), interval.get_start(), compareVal());
        if (bound_start == m_list.end()) {m_list.push_back(interval); return *this;}

        bound_start = m_list.insert(bound_start, interval);
        while (bound_start != m_list.end() && (bound_start + 1) != m_list.end() && bound_start->merge(*(bound_start + 1)))
            m_list.erase(bound_start + 1);
        return *this;
	}
	CRangeList & operator += (const CRangeList& list)
	{
		for (CRange range : list.m_list)
			*this += range;
		return *this;
	}
    // -= range / range list
    CRangeList & operator -= (CRange interval)
	{
		if (m_list.empty()) return *this;
		auto bound_start = lower_bound(m_list.begin(), m_list.end(), interval.get_start(), compareVal());
		if (bound_start == m_list.end() || (bound_start->get_start() != LLONG_MIN && interval.get_end() < bound_start->get_start() - 1)) return *this;
        // Do set exclusion
        bool mod_start = false;
        if (interval.get_start() > bound_start->get_start() && interval.get_start() <= bound_start->get_end())
        {
            bound_start = m_list.insert(bound_start, CRange(bound_start->get_start(), interval.get_start() - 1));
            //bound_start->set_end(interval.get_start() - 1) /* Interval.get_start() is definitelly not LLONG_MIN */
        }
        else mod_start = true;
        while (bound_start != m_list.end() && (bound_start + 1) != m_list.end() && interval.get_end() >= (bound_start + 1)->get_end())
        {
            (bound_start + 1) = m_list.erase(bound_start + 1);
        }

        // Modify the last one
        if (bound_start != m_list.end() && (bound_start + 1) != m_list.end() && interval.get_end() >= (bound_start + 1)->get_start())
        {
            (bound_start + 1)->set_start(interval.get_end() + 1);
        }
        // Modify or erase the first one if needed
        if (mod_start) 
        {
            if (interval.get_end() < bound_start->get_end())
                bound_start->set_start(interval.get_end() + 1);
            else
                m_list.erase(bound_start);
        }
        return *this;
	}
	CRangeList & operator -= (const CRangeList& list)
	{
		for (CRange range : list.m_list)
			*this -= range;
		return *this;
	}
    // = range / range list
	CRangeList & operator = (CRange interval)
	{
		m_list.clear();
		m_list.push_back(interval);
		return *this;
	}
    // operator ==
	bool operator == (const CRangeList & list) const
	{
		long long len = (long long)list.m_list.size();
		if (len != (long long)m_list.size()) return false;
		for (long long i = 0; i < len; ++i)
		{
			if (list.m_list[i] != m_list[i]) return false;
		}
		return true;
	}
    // operator !=
	bool operator != (const CRangeList & list) const
	{
		return !(*this == list);
	}
    // operator <<
	friend ostream & operator << (ostream & stream, const CRangeList & list);
private:
    vector<CRange> m_list;
};
// operator << 
ostream & operator << (ostream & stream, const CRangeList & list)
{
	stream << "{";
	for (auto it = list.m_list.begin(), endit = prev(list.m_list.end()); it <= endit; ++it)
	{
		if (it->get_start() != it->get_end())
			stream << "<" << it->get_start() << ".." << it->get_end() << ">";
		else 
			stream << it->get_start();
		if (it != endit) stream << ",";
	}
	stream << "}";
	return stream;
}
// Other operators
CRangeList operator +(CRange a, CRange b)
{
	CRangeList ret;
	ret = a;
	ret += b;
	return ret;
}
CRangeList operator +(CRangeList a, CRange b)
{
	return a += b;
}
CRangeList operator -(CRange a, CRange b)
{
	CRangeList ret;
	ret = a;
	ret -= b;
	return ret;
}
CRangeList operator -(CRangeList a, CRange b)
{
	return a -= b;
}

#ifndef __PROGTEST__
string toString ( const CRangeList& x )
{
  ostringstream oss;
  oss << x;
  return oss . str ();
}

int main ( void )
{
  CRangeList a, b;

  assert ( sizeof ( CRange ) <= 2 * sizeof ( long long ) );
  a = CRange ( 5, 10 );
  a += CRange ( 25, 100 );
  assert ( toString ( a ) == "{<5..10>,<25..100>}" );
  a += CRange ( -5, 0 );
  a += CRange ( 8, 50 );
  assert ( toString ( a ) == "{<-5..0>,<5..100>}" );
	//
  //auto c = CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  a += CRange ( 101, 105 ) + CRange ( 120, 150 ) + CRange ( 160, 180 ) + CRange ( 190, 210 );
  assert ( toString ( a ) == "{<-5..0>,<5..105>,<120..150>,<160..180>,<190..210>}" );
  a += CRange ( 106, 119 ) + CRange ( 152, 158 );
  assert ( toString ( a ) == "{<-5..0>,<5..150>,<152..158>,<160..180>,<190..210>}" );
  a += CRange ( -3, 170 );
  a += CRange ( -30, 1000 );
  assert ( toString ( a ) == "{<-30..1000>}" );
  b = CRange ( -500, -300 ) + CRange ( 2000, 3000 ) + CRange ( 700, 1001 );
  a += b;
  assert ( toString ( a ) == "{<-500..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( -400, -400 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..1001>,<2000..3000>}" );
  a -= CRange ( 10, 20 ) + CRange ( 900, 2500 ) + CRange ( 30, 40 ) + CRange ( 10000, 20000 );
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  try
  {
    a += CRange ( 15, 18 ) + CRange ( 10, 0 ) + CRange ( 35, 38 );
    assert ( "Exception not thrown" == nullptr );
  }
  catch ( const std::logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown" == nullptr );
  }
// Here done
  assert ( toString ( a ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  b = a;
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 2600, 2700 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,<21..29>,<41..899>,<2501..3000>}" );
  assert ( a == b );
  assert ( !( a != b ) );
  b += CRange ( 15, 15 );
  assert ( toString ( b ) == "{<-500..-401>,<-399..-300>,<-30..9>,15,<21..29>,<41..899>,<2501..3000>}" );
  assert ( !( a == b ) );
  assert ( a != b );
  assert ( b . includes ( 15 ) );
  assert ( b . includes ( 2900 ) );
  assert ( b . includes ( CRange ( 15, 15 ) ) );
  assert ( b . includes ( CRange ( -350, -350 ) ) );
  assert ( b . includes ( CRange ( 100, 200 ) ) );
  assert ( !b . includes ( CRange ( 800, 900 ) ) );
  assert ( !b . includes ( CRange ( -1000, -450 ) ) );
  assert ( !b . includes ( CRange ( 0, 500 ) ) );
  a += CRange ( -10000, 10000 ) + CRange ( 10000000, 1000000000 );
  assert ( toString ( a ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b += a;
  assert ( toString ( b ) == "{<-10000..10000>,<10000000..1000000000>}" );
  b -= a;
  assert ( toString ( b ) == "{}" );
  b += CRange ( 0, 100 ) + CRange ( 200, 300 ) - CRange ( 150, 250 ) + CRange ( 160, 180 ) - CRange ( 170, 170 );
  assert ( toString ( b ) == "{<0..100>,<160..169>,<171..180>,<251..300>}" );
  b -= CRange ( 10, 90 ) - CRange ( 20, 30 ) - CRange ( 40, 50 ) - CRange ( 60, 90 ) + CRange ( 70, 80 );
  assert ( toString ( b ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );

	CRangeList c = b;
	assert ( toString ( c ) == "{<0..9>,<20..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );

	c += CRange(10, 19);
	assert ( toString ( c ) == "{<0..30>,<40..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    c += CRange(39, 45);
    assert ( toString ( c ) == "{<0..30>,<39..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    c += CRange(35, 46);
    assert ( toString ( c ) == "{<0..30>,<35..50>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    c -= CRange(35, 50) - CRange(35, 49);
    assert ( toString ( c ) == "{<0..30>,<35..49>,<60..69>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    c += CRange(170, LLONG_MAX) - CRange(600, 700);
    assert ( toString ( c ) == "{<0..30>,<35..49>,<60..69>,<81..100>,<160..599>,<701..9223372036854775807>}" );
    c -= CRange(768, LLONG_MAX);
    assert ( toString ( c ) == "{<0..30>,<35..49>,<60..69>,<81..100>,<160..599>,<701..767>}" );
    c += CRange(-4673, -333) + CRange(999, 1011);
    assert ( toString ( c ) == "{<-4673..-333>,<0..30>,<35..49>,<60..69>,<81..100>,<160..599>,<701..767>,<999..1011>}" );
    assert ( !c . includes( CRange(LLONG_MIN, 767) ));
    assert ( !c . includes( CRange(999, LLONG_MAX) ));
    assert ( !c . includes( CRange(100, 701) ));
    assert ( c . includes( CRange(100, 100) ));
    assert ( !c . includes( CRange(100, 101) ));
    assert ( !c . includes( CRange(80, 100) ));
    assert ( !c . includes( CRange(80, 80) ));
    c -= CRange(25, 88);
    assert ( toString ( c ) == "{<-4673..-333>,<0..24>,<89..100>,<160..599>,<701..767>,<999..1011>}" );
    c += CRange(13, 789);
    assert ( toString ( c ) == "{<-4673..-333>,<0..789>,<999..1011>}" );
    c -= CRange(170, 5990);
    assert ( toString ( c ) == "{<-4673..-333>,<0..169>}" );
    c += CRange(-5, -2) + CRange(-1, -1);
    assert ( toString ( c ) == "{<-4673..-333>,<-5..169>}" );
    c -= CRange(LLONG_MIN, LLONG_MIN);
    assert ( toString ( c ) == "{<-4673..-333>,<-5..169>}" );
    c -= CRange(LLONG_MAX, LLONG_MAX);
    assert ( toString ( c ) == "{<-4673..-333>,<-5..169>}" );
    c -= CRange(LLONG_MIN, LLONG_MAX);
    assert ( toString ( c ) == "{}" );

	b += CRange(10, 70);
	assert ( toString ( b ) == "{<0..70>,<81..100>,<160..169>,<171..180>,<251..300>}" );
    assert (b.includes(CRange(172, 175)));
    assert (!b.includes(CRange(251, 301)));
    assert (!b.includes(CRange(250, 301)));
    assert (!b.includes(CRange(170, 180)));
    assert (b.includes(CRange(171, 180)));
    assert (!b.includes(CRange(251, LLONG_MAX)));

	b -= CRange(45, 301);
	assert ( toString ( b ) == "{<0..44>}" );

    CRangeList d;
    d += CRange(0, 1) + CRange(3, 4) + CRange(6, 7) + CRange(9, 10);
    assert ( toString ( d ) == "{<0..1>,<3..4>,<6..7>,<9..10>}" );
    assert ( d . includes ( 10 ) );
    assert ( !d . includes ( 15 ) );
    assert ( !d . includes ( 8 ) );
    assert ( d . includes ( 4 ) );
    assert ( !d . includes ( LLONG_MAX ) );
    assert ( !d . includes ( LLONG_MIN ) );
    d += CRange(2, 2);
    assert ( toString ( d ) == "{<0..4>,<6..7>,<9..10>}" );
    d -= CRange(2, 2);
    assert ( toString ( d ) == "{<0..1>,<3..4>,<6..7>,<9..10>}" );
    d += CRange(8, 8);
    assert ( toString ( d ) == "{<0..1>,<3..4>,<6..10>}" );
    d += CRange(5, 5);
    assert ( toString ( d ) == "{<0..1>,<3..10>}" );
    d += CRange(2, 2);
    assert ( toString ( d ) == "{<0..10>}" );
    d -= CRange(LLONG_MIN, -673);
    d -= CRange(-1331, -133);
    assert ( toString ( d ) == "{<0..10>}" );
    d += CRange(LLONG_MAX, LLONG_MAX);
    d -= CRange(78976, LLONG_MAX);
    assert ( toString ( d ) == "{<0..10>}" );
    d += CRange(20, 30);
    d -= CRange(5, 20);
    assert ( toString ( d ) == "{<0..4>,<21..30>}" );
    d -= CRange(4, 21);
    assert ( toString ( d ) == "{<0..3>,<22..30>}" );
    // d -= CRange(4, 21);
    // assert ( toString ( d ) == "{<0..3>,<22..30>}" );

/* DELETE EVERYTHING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

    CRangeList f;
    f += CRange(1,10);
    CRange mek(10,10);
    assert(f.includes(mek));

    CRangeList l;
  l += CRange(-10000, 10000);
  l += CRange(10000000, 1000000000);
  l += CRange(LLONG_MAX - 1, LLONG_MAX);
  assert(toString(l) == "{<-10000..10000>,<10000000..1000000000>,<9223372036854775806..9223372036854775807>}");
  l += CRange(LLONG_MAX, LLONG_MAX) + CRange(LLONG_MIN, LLONG_MAX);
  assert(toString(l) == "{<-9223372036854775808..9223372036854775807>}");
  l -= l;
  assert(toString(l) == "{}");
//   l += CRange(-10000, 10000) + CRange(10000000, 1000000000) + CRange(LLONG_MIN, LLONG_MIN + 1);
//   assert(toString(l) == "{<-9223372036854775808..-9223372036854775807>,<-10000..10000>,<10000000..1000000000>}");
//   l += CRange(LLONG_MIN, LLONG_MAX);
//   assert(toString(l) == "{<-9223372036854775808..9223372036854775807>}");

    a -= a;
  a = CRange(10, 10) + CRange(20, 20) + CRange(12, 12) + CRange(18, 18);
  assert ( toString ( a ) == "{10,12,18,20}" );
//   a -= CRange(11, 19);
//   assert ( toString ( a ) == "{10,20}" );
  a = CRange(10, 10) + CRange(20, 20) + CRange(12, 12) + CRange(18, 18);
  assert ( toString ( a ) == "{10,12,18,20}" );
  a -= CRange(10, 20);
  assert ( toString ( a ) == "{}" );
  a = CRange(10, 100);
  a -= CRange(20, 80);
  assert(toString(a) == "{<10..19>,<81..100>}");
  a = CRange(10, 100);
  a -= CRange(11, 99);
  assert(toString(a) == "{10,100}");
  a = CRange(10, 100);
  a -= CRange(11, 101);
  assert(toString(a) == "{10}");
  a = CRange(10, 100);
  a -= CRange(50, 150);
  assert(toString(a) == "{<10..49>}");
  a = CRange(10, 100);
  a -= CRange(0, 50);
  assert(toString(a) == "{<51..100>}");
  a = CRange(10, 100);
  a -= CRange(0, 99);
  assert(toString(a) == "{100}");
  a = CRange(10, 100);
  a -= CRange(10, 100);
  assert(toString(a) == "{}");
  a = CRange(10, 100);
  a -= CRange(0, 80);
  assert(toString(a) == "{<81..100>}");
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
//   a -= CRange(11, 29);
//   assert(toString(a) == "{<0..10>,<30..40>}");
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a -= CRange(10, 30);
  assert(toString(a) == "{<0..9>,<31..40>}");
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(0, 40);
  assert ( toString ( a ) == "{<-10..-5>,<50..60>}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(1, 39);
  assert ( toString ( a ) == "{<-10..-5>,0,40,<50..60>}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(2, 38);
  assert ( toString ( a ) == "{<-10..-5>,<0..1>,<39..40>,<50..60>}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(-5, 50);
  assert ( toString ( a ) == "{<-10..-6>,<51..60>}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(-8, 55);
  assert ( toString ( a ) == "{<-10..-9>,<56..60>}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(-9, 59);
  assert ( toString ( a ) == "{-10,60}" );
  a = CRange(0, 10);
  a += CRange(12, 28);
  a += CRange(30, 40);
  a += CRange(-10, -5);
  a += CRange(50, 60);
  a -= CRange(-10, 60);
  assert ( toString ( a ) == "{}" );
  a = CRange(2, 2);
  a += CRange(4, 4);
  a -= CRange(2, 2);
  a -= CRange(4, 4);
  assert ( toString ( a ) == "{}" );
  a = CRange(2, 2);
  a += CRange(4, 4);
  a += CRange(0, 0);
  a += CRange(6, 6);
  a -= CRange(2, 2);
  a -= CRange(4, 4);
  assert ( toString ( a ) == "{0,6}" );
  a = CRange(0, 0);
  a += CRange(10, 10);
  a += CRange(2, 8);
  a -= CRange(2, 2);
  a -= CRange(8, 8);
  assert ( toString ( a ) == "{0,<3..7>,10}" );
  a = CRange(0, 0);
  a += CRange(10, 10);
  a += CRange(5, 5);
  a -= CRange(5, 5);
  assert ( toString ( a ) == "{0,10}" );
  a = CRange(0, 10);
  a -= CRange(0, 0);
  a -= CRange(10, 10);
  assert ( toString ( a ) == "{<1..9>}" );
  a = CRange(0, 10);
  a += CRange(12, 20);
  a += CRange(22, 30);
  a -= CRange(11, 11);
//   a -= CRange(21, 21);
//   assert ( toString ( a ) == "{<0..10>,<12..20>,<22..30>}" );
  a = CRange(0, 10);
  a += CRange(12, 20);
  a += CRange(22, 30);
  a -= CRange(5, 5);
  a -= CRange(25, 25);
  assert ( toString ( a ) == "{<0..4>,<6..10>,<12..20>,<22..24>,<26..30>}" );
  a = CRange(0, 10);
  a += CRange(12, 20);
  a += CRange(22, 30);
//   a -= CRange(-1, -1);
//   a -= CRange(31, 31);
//   assert ( toString ( a ) == "{<0..10>,<12..20>,<22..30>}" );
//   a -= a;
//   for (int sf = 0; sf <= 60; sf++) {
//       if (sf % 2 == 0) {
//           a -= CRange(sf, sf);
//       } else {
//           a += CRange(sf, sf);
//       }
//   }
//   assert ( toString ( a ) == "{1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33,35,37,39,41,43,45,47,49,51,53,55,57,59}" );
//   a -= CRange(2, 58);
//   assert ( toString ( a ) == "{1,59}" );
//   a = CRange(10, 50);
//   a += CRange(0, 8);
//   a += CRange(52, 60);
//   a += CRange(9, 51);
//   assert ( toString ( a ) == "{<0..60>}" );

    a -= a;
  a = CRange(LLONG_MIN+1, LLONG_MAX-1);
  assert(toString(a) == "{<" + to_string(LLONG_MIN+1) + ".." + to_string(LLONG_MAX-1) + ">}");
  a += CRange(LLONG_MIN, LLONG_MAX);
  assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
  a -= CRange(LLONG_MIN, LLONG_MIN);
  a -= CRange(LLONG_MAX, LLONG_MAX);
  assert(toString(a) == "{<" + to_string(LLONG_MIN+1) + ".." + to_string(LLONG_MAX-1) + ">}");
  a += CRange(LLONG_MIN, LLONG_MIN);
  a += CRange(LLONG_MAX, LLONG_MAX);
  assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
  a -= CRange(0, 0);
  assert(toString(a) == "{<" + to_string(LLONG_MIN) + "..-1>,<1.." + to_string(LLONG_MAX) + ">}");
  a += CRange(0, 0);
  assert(toString(a) == "{<" + to_string(LLONG_MIN) + ".." + to_string(LLONG_MAX) + ">}");
  a -= CRange(LLONG_MIN+1, LLONG_MAX-1);
  assert(toString(a) == "{" + to_string(LLONG_MIN) + "," + to_string(LLONG_MAX) + "}");

  a -= a;
  a = CRange(LLONG_MIN, LLONG_MAX);
  a -= CRange(10, 12);
  a += a;

a -= a;
assert(toString(CRange(-1595,0) + CRange(0,LLONG_MAX) + CRange(LLONG_MAX,LLONG_MAX)) ==
         "{<-1595..9223372036854775807>}");

a = CRange(10, 20) + CRange(0, 9) + CRange(21, 30);
cout << "a: " << a << endl;
assert ( toString ( a ) == "{<0..30>}" );
cout << "TEST 4.1 OK" << endl << endl << endl;
a = CRange(10, 20) + CRange(0, 8) + CRange(22, 30);
cout << "a: " << a << endl;
assert ( toString ( a ) == "{<0..8>,<10..20>,<22..30>}" );
cout << "TEST 4.2 OK" << endl << endl << endl;
a = CRange(10, 20) + CRange(22, 30) + CRange(0, 50);
cout << "a: " << a << endl;
assert ( toString ( a ) == "{<0..50>}" );
cout << "TEST 4.3 OK" << endl << endl << endl;
a -= CRange(-5, 5) + CRange(25, 35) + CRange(45, 55);
cout << "a: " << a << endl;
assert ( toString ( a ) == "{<6..24>,<36..44>}" );
cout << "TEST 4.4 OK" << endl << endl << endl;
b = CRange(0, 0);
b -= CRange(0, 0);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{}" );
cout << "TEST 4.5 OK" << endl << endl << endl;
b -= CRange(0, 10);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{}" );
cout << "TEST 4.6 OK" << endl << endl << endl;
b += CRange ( 25, 100 );
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<25..100>}" );
cout << "TEST 4.7 OK" << endl << endl << endl;
b -= CRange(25, 25);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<26..100>}" );
cout << "TEST 4.7 OK" << endl << endl << endl;
b += CRange(1000, 1200);
b -= CRange(1000, 1000);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<26..100>,<1001..1200>}" );
cout << "TEST 4.8 OK" << endl << endl << endl;
b -= CRange(1200, 1200);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<26..100>,<1001..1199>}" );
cout << "TEST 4.9 OK" << endl << endl << endl;
b += CRange(30, 1100);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<26..1199>}" );
cout << "TEST 4.10 OK" << endl << endl << endl;
b -= CRange(30, 1100);
cout << "b: " << b << endl;
assert ( toString ( b ) == "{<26..29>,<1101..1199>}" );
cout << "TEST 4.11 OK" << endl << endl << endl;

a -= a;
assert(!a.includes(5));
cout << "TEST 5.0 OK" << endl << endl << endl;
assert(!a.includes(CRange(5, 10)));
cout << "TEST 5.1 OK" << endl << endl << endl;
a = CRange(5, 10);
assert(a.includes(5));
cout << "TEST 5.2 OK" << endl << endl << endl;
assert(a.includes(10));
cout << "TEST 5.3 OK" << endl << endl << endl;
assert(!a.includes(4));
cout << "TEST 5.4 OK" << endl << endl << endl;
assert(!a.includes(11));
cout << "TEST 5.5 OK" << endl << endl << endl;
assert(a.includes(CRange(5, 10)));
cout << "TEST 5.4 OK" << endl << endl << endl;
assert(!a.includes(CRange(4, 10)));
cout << "TEST 5.5 OK" << endl << endl << endl;
assert(!a.includes(CRange(5, 11)));
cout << "TEST 5.6 OK" << endl << endl << endl;

 
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
assert(!a.includes(LLONG_MIN));
assert(!a.includes(LLONG_MAX));
assert(a.includes(0));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MIN) + CRange(0,0);
assert(!a.includes(LLONG_MIN));
assert(!a.includes(0));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MAX, LLONG_MAX) + CRange(0,0);
assert(a.includes(LLONG_MIN));
assert(!a.includes(0));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, 0);
assert(!a.includes(LLONG_MIN));
assert(!a.includes(0));
assert(!a.includes(-100));
assert(a.includes(100));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, 0);
assert(!a.includes(LLONG_MIN));
assert(!a.includes(0));
assert(!a.includes(-100));
assert(a.includes(100));
assert(a.includes(LLONG_MAX));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(0, LLONG_MAX);
assert(!a.includes(LLONG_MAX));
assert(!a.includes(0));
assert(a.includes(-100));
assert(!a.includes(100));
assert(a.includes(LLONG_MIN));
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MIN, LLONG_MIN);
a += CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MIN);
assert(toString(a) == "{}");
a = CRange(LLONG_MAX, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MIN);
a -= CRange(LLONG_MAX, LLONG_MAX);
//a -= CRange(LLONG_MIN, LLONG_MIN);
//assert(toString(a) == "{}");
a = CRange(LLONG_MIN, LLONG_MIN);
a -= CRange(LLONG_MIN, LLONG_MIN);
assert(toString(a) == "{}");
a = CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MAX, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MAX, LLONG_MAX);
a += CRange(LLONG_MAX, LLONG_MAX);
assert(toString(a) == "{9223372036854775807}");
a = CRange(LLONG_MIN, LLONG_MIN);
a += CRange(LLONG_MIN, LLONG_MIN);
assert(toString(a) == "{-9223372036854775808}");
a = CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MIN, LLONG_MAX);
a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
assert(toString(a) == "{-9223372036854775808,9223372036854775807}");
a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
a += CRange(LLONG_MIN + 1, LLONG_MAX - 1);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
// a += CRange(LLONG_MIN + 2, LLONG_MAX - 2);
// assert(toString(a) == "{-9223372036854775808,<-9223372036854775806..9223372036854775805>,9223372036854775807}");
a = CRange(LLONG_MAX, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX - 1);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MIN);
a += CRange(LLONG_MIN + 1, LLONG_MAX);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MIN, LLONG_MIN);
a -= CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MIN, LLONG_MAX);
assert(toString(a) == "{}");
a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
// a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
// assert(toString(a) == "{-9223372036854775808,9223372036854775807}");
a = CRange(LLONG_MIN, LLONG_MIN) + CRange(LLONG_MAX, LLONG_MAX);
a += CRange(LLONG_MIN + 1, LLONG_MAX - 1);
assert(toString(a) == "{<-9223372036854775808..9223372036854775807>}");
a = CRange(LLONG_MIN, LLONG_MIN);
// a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
// assert(toString(a) == "{-9223372036854775808}");
a = CRange(LLONG_MAX, LLONG_MAX);
a -= CRange(LLONG_MIN + 1, LLONG_MAX - 1);
assert(toString(a) == "{9223372036854775807}");

a -= a;
b -= b;
a -= CRange(5, 10);
assert(!a.includes(5));
cout << "TEST 2.1 OK" << endl << endl << endl;
assert(!a.includes( CRange(5, 10) ));
cout << "TEST 2.2 OK" << endl << endl << endl;
a += b;
a = CRange(LLONG_MIN, LLONG_MAX);
assert(a.includes(5));
assert(a.includes(LLONG_MIN));
assert(a.includes(LLONG_MAX));
cout << "TEST 2.5 OK" << endl << endl << endl;
cout << a << endl;
a -= CRange(LLONG_MIN, 0);
cout << a << endl;
assert(a.includes(5));
assert(!a.includes(-5));
cout << "TEST 2.7 OK" << endl << endl << endl;
a = CRange(LLONG_MIN, LLONG_MAX);
a += CRange(LLONG_MIN, LLONG_MAX);
assert(a.includes(LLONG_MIN));
assert(a.includes(LLONG_MAX));
cout << "TEST 2.9 OK" << endl << endl << endl;
cout << "odebrání LLONG_MIN" << endl;
cout << a << endl;
a -= CRange(LLONG_MIN, LLONG_MIN);
cout << a << endl;
assert(!a.includes(LLONG_MIN));
cout << "TEST 2.10 OK" << endl << endl << endl;
a += CRange(LLONG_MIN, LLONG_MIN);
cout << a << endl;
assert(a.includes(LLONG_MIN));
cout << "TEST 2.11 OK" << endl << endl << endl;
a -= CRange(LLONG_MAX, LLONG_MAX);
cout << a << endl;
assert(!a.includes(LLONG_MAX));
cout << "TEST 2.12 OK" << endl << endl << endl;
a += CRange(LLONG_MAX, LLONG_MAX);
cout << a << endl;
assert(a.includes(LLONG_MAX));
cout << "TEST 2.13 OK" << endl << endl << endl;

#ifdef EXTENDED_SYNTAX
  CRangeList x { { 5, 20 }, { 150, 200 }, { -9, 12 }, { 48, 93 } };
  assert ( toString ( x ) == "{<-9..20>,<48..93>,<150..200>}" );
  ostringstream oss;
  oss << setfill ( '=' ) << hex << left;
  for ( const auto & v : x + CRange ( -100, -100 ) )
    oss << v << endl;
  oss << setw ( 10 ) << 1024;
  assert ( oss . str () == "-100\n<-9..20>\n<48..93>\n<150..200>\n400=======" );
#endif /* EXTENDED_SYNTAX */
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
