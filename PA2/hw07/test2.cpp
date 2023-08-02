#ifndef __PROGTEST__
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <stack>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <functional>
#include <iterator>
#include <stdexcept>
using namespace std;
#endif /* __PROGTEST__ */

template <typename M_>
struct CGame
{
	CGame(string f, string s, M_ r) : first(f), second(s), result(r) {}
	string first;
	string second;
	M_ result;
};
template <typename M_>
class CContest
{
public:
    // default constructor
    // CContest() : m_constructed(false) {}
    // destructor
    // addMatch ( contestant1, contestant2, result )
	CContest & addMatch(const string & contestant1, const string & contestant2, M_ result)
	{
        for (size_t i = 0, l = m_games.size(); i < l; ++i)
		{
			if ((contestant1 == m_games[i].first && contestant2 == m_games[i].second) ||
                (contestant2 == m_games[i].first && contestant1 == m_games[i].second))
                throw logic_error("Similar match already registered");
        }
        m_games.push_back(CGame<M_>(contestant1, contestant2, result));
        //m_constructed = false;
		return *this;
	}
    // isOrdered ( comparator )
    bool isOrdered(function<int(const M_ &)> comparator) const
    {
        // Adjacency list
	    unordered_map<string, vector<string>> graph;
        // Storing the number of wins for each contestant
        unordered_map<string, int> winCount;
        construct(comparator, graph, winCount);
		for (const pair<string, vector<string>> & node : graph)
        {
            winCount[node.first] = bfs(node.first, graph);
        }
        // Check if the scoreboard is unambiguos
        unordered_set<int> unique_values;
        for (const pair<string, int> & p : winCount)
        {
            if (unique_values.find(p.second) != unique_values.end())
                return false;
            unique_values.insert(p.second);
        }
        return true;
    }
    // results ( comparator )
    list<string> results(function<int(const M_ &)> comparator) const
    {
        unordered_map<string, vector<string>> graph;
        unordered_map<string, int> winCount;
        // if (!isOrdered(comparator))
        // {
        //     throw logic_error("List is ambiguous");
        // }
        construct(comparator, graph, winCount);
        for (const pair<string, vector<string>> & node : graph)
        {
            winCount[node.first] = bfs(node.first, graph);
        }
        // Check if the scoreboard is unambiguos
        unordered_set<int> unique_values;
        for (const pair<string, int> & p : winCount)
        {
            if (unique_values.find(p.second) != unique_values.end())
                throw logic_error("List is ambiguous");
            unique_values.insert(p.second);
        }

        vector<pair<string, int>> vec(winCount.begin(), winCount.end());
        sort(vec.begin(), vec.end(), 
            [](const pair<string, int> & a, const pair<string, int> & b)
            {return a.second > b.second;});

        list<string> ret_list;
        for (const pair<string, int> & elem : vec)
            ret_list.push_back(elem.first);

        return ret_list;
    }
private:
	// Vector to store matches
	vector<CGame<M_>> m_games;

    // bool m_constructed;

	// Construction of a graph
	void construct(function<int(const M_ &)> comparator, unordered_map<string, vector<string>> & graph, unordered_map<string, int> & winCount) const
	{
		graph.clear(); // Paranoic
		for (const CGame<M_> & game : m_games)
        {
			// Set win count to 0 for everyone
			winCount[game.first] = 0;
			winCount[game.second] = 0;
            int res = comparator(game.result);
            if (res > 0)
			{
                graph[game.first].push_back(game.second);
				graph[game.second];
			}
            else if (res < 0)
			{
                graph[game.second].push_back(game.first);
				graph[game.first];
			}
            // Else - there is a draw
        }
	}
	// Fill the m_winCount cell for a particular node
    int bfs(const string & node, unordered_map<string, vector<string>> & graph) const
    {
        queue<string> q;
        set<string> v;
        q.push(node);
        v.insert(node);

        int count = 0;

        while (!q.empty())
        {
            string current_node = q.front();
            q.pop();

            ++count;

            for (const string & adjacent : graph[current_node])
            {
                if (v.find(adjacent) == v.end())
                {
                    v.insert(adjacent);
                    q.push(adjacent);
                }
            }
        }
        return count;
    }
	
};

#ifndef __PROGTEST__
struct CMatch
{
public:
	CMatch ( int a, int b ) : m_A ( a ), m_B ( b ) {}
    int m_A;
    int m_B;
};

class HigherScoreThreshold
{
public:
	HigherScoreThreshold ( int diffAtLeast ): m_DiffAtLeast ( diffAtLeast ){}
    int operator () ( const CMatch & x ) const
    {
    	return ( x.m_A > x.m_B + m_DiffAtLeast ) - ( x.m_B > x.m_A + m_DiffAtLeast );
    }
private:
    int m_DiffAtLeast;    
};

int HigherScore ( const CMatch & x )
{
  return ( x.m_A > x.m_B ) - ( x.m_B > x.m_A );
}

int main ( void )
{
  CContest<CMatch>  x;
  
  x . addMatch ( "C++", "Pascal", CMatch ( 10, 3 ) )
    . addMatch ( "C++", "Java", CMatch ( 8, 1 ) )
    . addMatch ( "Pascal", "Basic", CMatch ( 40, 0 ) )
    . addMatch ( "Java", "PHP", CMatch ( 6, 2 ) )
    . addMatch ( "Java", "Pascal", CMatch ( 7, 3 ) )
    . addMatch ( "PHP", "Basic", CMatch ( 10, 0 ) );
    
  
  assert ( ! x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }

  x . addMatch ( "PHP", "Pascal", CMatch ( 3, 6 ) ); 

  assert ( x . isOrdered ( HigherScore ) );
  try
  {
    list<string> res = x . results ( HigherScore );
    assert ( ( res == list<string>{ "C++", "Java", "Pascal", "PHP", "Basic" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }

  
  assert ( ! x . isOrdered ( HigherScoreThreshold ( 3 ) ) );
  try
  {
    list<string> res = x . results ( HigherScoreThreshold ( 3 ) );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  assert ( x . isOrdered ( [] ( const CMatch & x )
  {
    return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
  } ) );
  try
  {
    list<string> res = x . results ( [] ( const CMatch & x )
    {
      return ( x . m_A < x . m_B ) - ( x . m_B < x . m_A ); 
    } );
    assert ( ( res == list<string>{ "Basic", "PHP", "Pascal", "Java", "C++" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
  
  CContest<bool>  y;
  
  y . addMatch ( "Python", "PHP", true )
    . addMatch ( "PHP", "Perl", true )
    . addMatch ( "Perl", "Bash", true )
    . addMatch ( "Bash", "JavaScript", true )
    . addMatch ( "JavaScript", "VBScript", true );
  
  assert ( y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    });
    assert ( ( res == list<string>{ "Python", "PHP", "Perl", "Bash", "JavaScript", "VBScript" } ) );
  }
  catch ( ... )
  {
    assert ( "Unexpected exception!" == nullptr );
  }
    
  y . addMatch ( "PHP", "JavaScript", false );
  assert ( !y . isOrdered ( [] ( bool v )
  {
    return v ? 10 : - 10;
  } ) );
  try
  {
    list<string> res = y . results ( [] ( bool v )
    {
      return v ? 10 : - 10;
    } );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "PHP", "JavaScript", false );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  
  try
  {
    y . addMatch ( "JavaScript", "PHP", true );
    assert ( "Exception missing!" == nullptr );
  }
  catch ( const logic_error & e )
  {
  }
  catch ( ... )
  {
    assert ( "Invalid exception thrown!" == nullptr );
  }
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
