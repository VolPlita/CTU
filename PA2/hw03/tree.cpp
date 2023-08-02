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

// I stopped on finding inorder predecessor

// uncomment if your code implements initializer lists
// #define EXTENDED_SYNTAX

class CRange
{
public:
	CRange (long long s = NULL, long long e = NULL)
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

	bool operator == (CRange & another)
	{
		return m_start == another.get_start() && m_end == another.get_end();
	}
	bool operator != (CRange & another)
	{
		return m_start != another.get_start() || m_end != another.get_end();
	}

	bool in_bounds(long long val) const // Shows if value val is in bounds of this range
	{
		return (val >= m_start && val <= m_end);
	}
	bool in_bounds(CRange & interval) const // Shows if interval is in bounds of this range
	{
		return (interval.get_start() >= m_start && interval.get_end() <= m_end);
	}
    bool is_overlap(CRange & another) const // Shows if another overlaps with this
    {
        return ((another.get_start() <= (m_end + 1) && another.get_start() >= m_start)      ||
                (another.get_end()   <=  m_end      && another.get_end()   >= (m_start - 1) ));
    }
    int direction(CRange & another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (another.get_end() < (m_start - 1)) return -1;
        if (another.get_start() > (m_end + 1)) return 1;
        else return 0;
    }
	int direction(long long & another) const // -1: [another]<-[this] | 1: [this]->[another] | 0: overlap
    {
        if (another < (m_start - 1)) return -1;
        if (another > (m_end + 1)) return 1;
        else return 0;
    }

private:
	long long m_start;
	long long m_end;
};

struct CNode
{
    CRange range;
    CNode * left;
    CNode * right;
    CNode (CRange val)
    {
        range = val;
        left = nullptr;
        right = nullptr;
    }
};

class CRangeList
{
  public:
    // constructor / destructor
	CRangeList () {m_root = nullptr;}
	~CRangeList () {delete_tree(m_root);}
    // includes long long / range
	bool includes(long long val) const
	{
	}
	bool includes (CRange interval) const
	{
	}
    // += range / range list
	CRangeList * operator += (CRange interval)
	{

	}
    // -= range / range list
	CRangeList * operator -= (CRange interval)
	{
	}
    // = range / range list
    // operator ==
    // operator !=
    // operator <<
  private:
    CNode * m_root;

	// Insert a single node at a correct place - maybe I don't need this
	CNode * insert_node(CNode * root, CRange & key)
	{
		if (root == nullptr) return new CNode(key);
		int dir = root->range.direction(key);
		if (dir < 0) return insert_node(root->left, key);
		else if (dir > 0) return insert_node(root->right, key);
		else /* dir == 0 - overlap */
		{
			if (root->range.get_end() < key.get_end()) // Right overlap
			{
				// 1) Find the biggest node that is smaller than the bound or includes it
				CNode * bound_node = find_max_for(root, key.get_end());
				// Remembering the right child
				CNode * bound_node_right = bound_node->right;
				bound_node_right = nullptr;
				// 2) Find inorder successor of bound_node
				CNode * inorder_suc = find_inorder_suc(root, bound_node);
				// 3) Remove range from bound_node to inorder_suc
				root = remove_range(root, bound_node->range, inorder_suc->range);
				// 4) Insert bound_node_right as a left subtree of inorder_suc
				inorder_suc->left = bound_node_right;
				// 5) Fix value of bound_node
				bound_node->range.set_end(key.get_end());
			}
			if (root->range.get_start() > key.get_start()) // Left overlap
			{
				// 1) Find the smallest node that is bigger than the bound or includes it
				CNode * bound_node = find_min_for(root, key.get_start());
				// Remembering the left child
				CNode * bound_node_left = bound_node->left;
				bound_node_left = nullptr;
				// 2) Find inorder predecessor of bound_node
				CNode * inorder_pre = find_inorder_pre(root, bound_node);
			}
			/* If neither, then in bounds, do nothing */
		}
	}

	// Delete a single node
	CNode * delete_node(CNode * root, CNode * node)
	{
		if (node->left == nullptr)
		{
			CNode * child = node->right;
			delete node;
			node = nullptr;
			return child;
		}
		else if (node->right == nullptr)
		{
			CNode * child = node->left;
			delete node;
			node = nullptr;
			return child;
		}
		CNode * suc = find_inorder_suc(root, node);
		node->range = suc->range;
		node->right = delete_node(root, node->right);
		return root;
	}

	// Delete all nodes in range from low to high, not including bounds
	CNode * remove_range(CNode * node, CRange & low, CRange & high) 
	{
		if (node == nullptr) return node;
		node->left = remove_range(node->left, low, high);
		node->right = remove_range(node->right, low, high);
		if (node->range.direction(low) < 0 && node->range.direction(high) > 0)
			return delete_node(m_root, node);
		return node;
	}

	// Search for a node that overlaps
	CNode * search(CNode * root, CRange key) const
	{
		if (root == nullptr || root->range.is_overlap(key)) return root;
		int dir = root->range.direction(key);
		if (dir < 0) return search(root->left, key);
		return search(root->right, key);
	}
	CNode * search(CNode * root, long long key) const
	{
		return root;
	}

	// Deletes a node and all it's children
    void delete_tree(CNode * root)
    {
        if (root == nullptr) return;

        delete_tree(root->left);
        delete_tree(root->right);

        delete root;
    }

	// Support functions:
	CNode * find_max_for(CNode * root, long long bound) const
	{
		if (root == nullptr) return root;
		int dir = root->range.direction(bound);
		if (dir == 0) return root;
		else if (dir > 0)
		{
			CNode * res = find_max_for(root->right, bound);
			if (res == nullptr) return root;
			else return res;
		}
		else return find_max_for(root->left, bound);
	}
	CNode * find_min_for(CNode * root, long long bound) const // ???
	{
		if (root == nullptr) return root;
		int dir = root->range.direction(bound);
		if (dir == 0) return root;
		else if (dir < 0)
		{
			CNode * res = find_min_for(root->right, bound);
			if (res == nullptr) return root;
			else return res;
		}
		else return find_min_for(root->left, bound);
	}
	CNode * find_inorder_suc(CNode * root, CNode * node) const
	{
		if (node->right != nullptr) return find_min_val(node->right);
		CNode * suc = nullptr;
		while (root->range != node->range)
		{
			int dir = root->range.direction(node->range);
			if (dir < 0)
			{
				suc = root;
				root = root->left;
			}
			else root = root->right;
		}
		return suc;
	}
	CNode * find_inorder_pre(CNode * root, CNode * node) const
	{
		if ()
	}
	CNode * find_min_val(CNode * root) const
	{
		CNode * cur = root;
		while (cur->left != nullptr) {cur = cur->left;}
		return cur;
	}
};

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
