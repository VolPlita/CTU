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
	bool in_bounds(long long val) const // ?? inline
	{
		return (val >= (m_start - 1) && val <= (m_end + 1));
	}
	bool in_bounds(CRange interval) const // Shows if another is in bounds of this
	{
		return (interval.get_start() >= (m_start - 1) && interval.get_end() <= (m_end + 1));
	}
	int direction(CRange another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (another.get_end() < (m_start - 1)) return -1;
        if (another.get_start() > (m_end + 1)) return 1;
        else return 0;
    }
	int direction(long long another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (another < (m_start - 1)) return -1;
        if (another > (m_end + 1)) return 1;
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

private:
	long long m_start;
	long long m_end;
};
// struct compareRange // Returns true if a is strictly before b
// {
// 	bool operator ()(const CRange & a, const CRange & b) const
// 	{
// 		return (a.get_end() < (b.get_start() - 1));
// 	}
// };
struct compareVal // Returns true if a is strictly before b
{
	bool operator ()(const CRange a, const long long b) const
	{
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
		if (val >= ran->get_start() && val <= ran->get_end()) return true;
		return false;
	}
	bool includes (CRange interval) const
	{
		auto ran_s = lower_bound(m_list.begin(), m_list.end(), interval.get_start(), compareVal());
		//auto ran_e = lower_bound(m_list.begin(), m_list.end(), interval.get_end(), compareVal());
		if (interval.get_start() >= ran_s->get_start() && interval.get_end() <= ran_s->get_end())
			return true;
		return false;
	}
    // += range / range list
	CRangeList & operator += (CRange interval) // += shall return by reference
	{
		if (m_list.empty()) m_list.push_back(interval);
		auto bound_start = lower_bound(m_list.begin(), m_list.end(), interval.get_start(), compareVal());
		auto bound_end = lower_bound(m_list.begin(), m_list.end(), interval.get_end(), compareVal());
		if (bound_start == bound_end)
		{
			if (bound_start == m_list.end()) {m_list.push_back(interval); return *this;} // This seems very error prone
			int dir = bound_start->direction(interval);
			if (dir < 0) 
				m_list.insert(bound_start, interval);
			else if (!bound_start->in_bounds(interval)) // dir == 0
				bound_start->set_start(interval.get_start());
			else // Correction +-1
			{
				bound_start->set_start(min(bound_start->get_start(), interval.get_end()));
				bound_start->set_end(max(bound_start->get_end(), interval.get_end()));
			}
		}
		else
		{
			// To prevent iterator invalidation, modify first and only then erase unneeded
			if (bound_end == m_list.end()) bound_start->set_end(interval.get_end());// Special case
			if (bound_start->in_bounds(interval.get_start()))
			{
				//if (bound_end == m_list.end()) bound_start->set_end(interval.get_end());// Special case
				if (bound_end->in_bounds(interval.get_end())) bound_start->set_end(bound_end->get_end());
				else bound_start->set_end(interval.get_end());
			}
			else
			{
				bound_start->set_start(interval.get_start());
				//if (bound_end == m_list.end()) bound_start->set_end(interval.get_end());// Special case
				if (bound_end->in_bounds(interval.get_end())) bound_start->set_end(bound_end->get_end());
				else bound_start->set_end(interval.get_end());
			}
			// Erase all intervals in between
			auto it = ++bound_start;
			while (it != m_list.end() && it->direction(interval.get_end()) >= 0) // Perhaps checking for m_list.end() is buggy
				m_list.erase(it);
		}
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
		auto bound_end = lower_bound(m_list.begin(), m_list.end(), interval.get_end(), compareVal());
		if (bound_start == bound_end)
		{
			if (bound_start == m_list.end()) return *this;
			if(interval.get_start() < bound_start->get_start() && interval.get_end() >= bound_start->get_start())
			{
				bound_start->set_start(interval.get_end() + 1);	
			}
			else
			{
				if (interval.get_start() == bound_start->get_start() && interval.get_end() == bound_start->get_end())
					m_list.erase(bound_start);
				else if (interval.get_start() == bound_start->get_start()) bound_start->set_start(interval.get_end() + 1);
				else if (interval.get_end() == bound_start->get_end()) bound_start->set_end(interval.get_start() - 1);
				else
				{
					CRange a = *bound_start;
					a.set_end(interval.get_start() - 1);
					bound_start->set_start(interval.get_end() + 1);
					m_list.insert(bound_start, a);
				}
			}
		}
		else
		{
			if (bound_end == m_list.end()) bound_start->set_end(interval.get_start() - 1);
			else if (bound_start->in_bounds(interval.get_start()))
			{
				bound_start->set_end(interval.get_start() - 1);
				if (bound_end->in_bounds(interval.get_end())) bound_end->set_start(interval.get_end() + 1);
				// Erase all in between
				auto it = ++bound_start;
				while (it != m_list.end() && it->direction(interval.get_end()) > 0) // Perhaps checking for m_list.end() is buggy
					m_list.erase(it);
			}
			else
			{
				if (bound_end->in_bounds(interval.get_end())) bound_end->set_start(interval.get_end() + 1);
				// Erase all in between
				auto it = bound_start;
				while (it != m_list.end() && it->direction(interval.get_end()) > 0) // Perhaps checking for m_list.end() is buggy
					m_list.erase(it);
			}
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
    // CRangeList & operator = (const CRangeList& list)
	// {
	// 	m_list.clear();
	// 	for (auto i : list.m_list)
	// 	{
	// 		m_list.push_back(i);
	// 	}
	// 	return *this;
	// }
    // operator ==
	bool operator == (CRangeList & list) const
	{
		size_t len = list.m_list.size();
		if (len != m_list.size()) return false;
		for (size_t i = 0; i < len; ++i)
		{
			if (list.m_list[i] != m_list[i]) return false;
		}
		return true;
	}
    // operator !=
	bool operator != (CRangeList & list) const
	{
		size_t len = list.m_list.size();
		if (len != m_list.size()) return true;
		for (size_t i = 0; i < len; ++i)
		{
			if (list.m_list[i] != m_list[i]) return true;
		}
		return false;
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
