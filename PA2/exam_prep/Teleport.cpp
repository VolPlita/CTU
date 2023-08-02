//The task is to implement the CTeleport class,
// which will make it possible to define a system of teleports between cities
// and search for the best routes in time.
//
//Teleport is always one-way, leading from one city to another,
// and has a defined time in which we must teleport from the first city,
// and then the time in which we appear in the second city.
// There can be multiple teleport connections between two cities that differ by specified times,
// then they form a "timetable", we can use any of them.
// There are also teleports that can take passengers back in time,
// so they appear in the second city before they teleported in the first city.
// Transfers between teleports are immediate, if, for example, we arrive in the city at time 5,
// we can immediately use the teleport, which also leaves at time 5.
//
//The interface of the CTeleport class is as follows:
//
//implicit constructor
//initializes an empty instance of the class, so there are no teleports between cities.
//
//destructor
//frees resources allocated by an instance of the class.
//
//Add(from, to, fromTime, toTime)
//
//adds a teleport entry from city from to city to.
//
// From the from city, the traveler must teleport at fromTime and it will then appear in the city at toTime.
// There may be multiple teleports between two cities (these may have different teleportation times).
// Calls to this method can be chained one after the other (so-called fluent interface).
//
//optimize()
//
//this helper method is always called between entering new teleports and finding the best routes.
// You can use it to possibly optimize your internal data representation.
// Calls to this method can be chained together just like calls to the Add method.
//
//FindWay(from, to, time)
//
//finds the time-best sequence of teleports from city from to city to starting at time.
// The result is the time in which the traveler can first appear in the city.
// If there is no sequence of teleports that would allow the traveler to get from the from city to the to city,
// the method throws an invalid_argument exception.
//
//Submit the source code with the implementation of the required class.
// In the attached archive you will find the declaration of the required class and the main function with a basic test.
// When implementing, you can (or must) use structures from the STL.
// The solution should be reasonably efficient. The naive vector-only approach fails the speed test.
// Failure to pass the speed test will result in a point penalty.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <list>
#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <memory>
using namespace std;

struct CEdge
{
    unsigned m_fromTime;
    unsigned m_toTime;
    string m_src;
    string m_dest;
    CEdge() {}
    CEdge(int from, int to, string src, string dest) : m_fromTime(from), m_toTime(to), m_src(std::move(src)), m_dest(std::move(dest)) {}
    bool operator == (const CEdge & another) const
    {
        return m_fromTime == another.m_fromTime &&
               m_toTime == another.m_toTime &&
               m_dest == another.m_dest;
    }
};

class HashFun
{
    size_t operator() (const CEdge & var)
    {
        hash<string> hasher;
        return hasher(var.m_src) ^
               (hasher(var.m_dest) << 1) ^
               ((size_t)var.m_fromTime << 2) ^
               ((size_t)var.m_toTime << 2);
    }
};

class CTeleport
{
    // graph
    unordered_map<string, vector<CEdge>> graph;

public:

    // ctor
    // dtor

    CTeleport & Add ( const string & from,
                      const string & to,
                      unsigned fromTime,
                      unsigned toTime )
    {
        graph[from].emplace_back(fromTime, toTime, from, to);
        return *this;
    }

    CTeleport & Optimize ( void )
    {
        for(auto & p : graph)
        {
            std::sort(p.second.begin(), p.second.end(), [](const CEdge& a, const CEdge& b)
            {
                return a.m_fromTime < b.m_fromTime;
            });
        }
        return *this;
    }

    unsigned FindWay ( const string & from,
                       const string & to,
                       unsigned time )
    {
        queue<CEdge> Q;
        vector<CEdge> visited;

        // Find start edge
        for (const CEdge& e : graph[from] ) {
            if (e.m_fromTime >= time)
            {
                Q.push(e);
                visited.push_back(e);
            }
        }

        unsigned result = -1;

        while(!Q.empty())
        {
            CEdge currentEdge = Q.front();
            Q.pop();
            unsigned arriveTime = currentEdge.m_toTime;
            if (to == currentEdge.m_dest && arriveTime < result) result = arriveTime;

            for(const CEdge& neighbor : graph[currentEdge.m_dest])
            {
                auto it = find(visited.begin(), visited.end(), neighbor);
                if (neighbor.m_fromTime >= arriveTime && it == visited.end())
                {
                    Q.push(neighbor);
                    visited.push_back(neighbor);
                }
            }
        }
        if (result == -1) throw invalid_argument("Path not found");
        return result;
    }
};

void test1() {
    CTeleport t;
    t.Add("A", "B", 0, 5)
            .Add("B", "C", 6, 10)
            .Add("C", "D", 11, 15)
            .Add("A", "C", 2, 10)
            .Add("B", "D", 7, 12)
            .Optimize();

    // Shortest path from A to D is A -> B -> D, total time = 12
    assert(t.FindWay("A", "D", 0) == 12);
    // Shortest path from A to C is A -> C, total time = 10
    assert(t.FindWay("A", "C", 0) == 10);
    // Shortest path from B to D is B -> D, total time = 12
    assert(t.FindWay("B", "D", 0) == 12);
    // No path from D to A, should throw an exception
    try {
        t.FindWay("D", "A", 0);
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument &e) {
    }
}

void test2() {
    CTeleport t;
    t.Add("A", "B", 0, 5)
            .Add("B", "C", 6, 10)
            .Add("C", "D", 11, 15)
            .Add("D", "A", 12, 18)
            .Optimize();

    // Shortest path from A to D is A -> B -> C -> D, total time = 15
    assert(t.FindWay("A", "D", 0) == 15);
    // Shortest path from D to A is D -> A, total time = 18
    assert(t.FindWay("D", "A", 0) == 18);
    // No path from C to B, should throw an exception
    try {
        t.FindWay("C", "B", 0);
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument &e) {
    }
}

void test3() {
    CTeleport t;
    t.Add("A", "B", 0, 5)
            .Add("B", "C", 6, 10)
            .Add("C", "D", 11, 15)
            .Add("D", "A", 16, 20)
            .Optimize();

    // Shortest path from A to D is A -> B -> C -> D, total time = 15
    assert(t.FindWay("A", "D", 0) == 15);
    // Shortest path from D to A is D -> A, total time = 20
    assert(t.FindWay("D", "A", 0) == 20);
    // No path from B to D, should throw an exception
    assert(t.FindWay("B", "D", 0) == 15);
}
void test5() {
    CTeleport t;
    t.Add("A", "B", 0, 5)
            .Add("B", "C", 6, 10)
            .Add("C", "D", 11, 15)
            .Add("D", "E", 16, 20)
            .Add("E", "F", 21, 25)
            .Add("F", "G", 26, 30)
            .Add("G", "H", 31, 35)
            .Add("H", "I", 36, 40)
            .Add("I", "J", 41, 45)
            .Add("J", "K", 46, 50)
            .Add("K", "L", 51, 55)
            .Add("L", "M", 56, 60)
            .Add("M", "N", 61, 65)
            .Add("N", "O", 66, 70)
            .Add("O", "P", 71, 75)
            .Add("P", "Q", 76, 80)
            .Add("Q", "R", 81, 85)
            .Add("R", "S", 86, 90)
            .Add("S", "T", 91, 95)
            .Add("T", "U", 96, 100)
            .Add("U", "V", 101, 105)
            .Add("V", "W", 106, 110)
            .Add("W", "X", 111, 115)
            .Add("X", "Y", 116, 120)
            .Add("Y", "Z", 121, 125)
            .Add("A", "Z", 3, 123)
            .Optimize();

    // Shortest path from A to Z is A -> Z, total time = 123
    assert(t.FindWay("A", "Z", 0) == 123);
    // Shortest path from B to V is B -> C -> D -> E -> F -> G -> H -> I -> J -> K
    // -> L -> M -> N -> O -> P -> Q -> R -> S -> T -> U -> V, total time = 105
    assert(t.FindWay("B", "V", 0) == 105);
    // No path from V to A, should throw an exception
    try {
        t.FindWay("V", "A", 0);
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument &e) {
    }
}

void test6() {
    CTeleport t;
    t.Add("A", "B", 0, 5)
            .Add("B", "C", 6, 10)
            .Add("B", "C", 7, 11)
            .Add("B", "C", 5, 10)
            .Add("B", "C", 6, 10)
            .Add("B", "C", 6, 10)
            .Add("B", "C", 7, 32)
            .Add("B", "C", 6, 1)
            .Add("B", "C", 6, 90)
            .Add("C", "D", 11, 15)
            .Add("D", "E", 16, 20)
            .Add("E", "F", 21, 25)
            .Add("F", "G", 26, 30)
            .Add("G", "H", 31, 35)
            .Add("H", "I", 36, 40)
            .Add("I", "J", 41, 45)
            .Add("J", "K", 46, 50)
            .Add("K", "L", 51, 55)
            .Add("L", "M", 56, 60)
            .Add("M", "N", 61, 65)
            .Add("N", "O", 66, 70)
            .Add("O", "P", 71, 75)
            .Add("P", "Q", 76, 80)
            .Add("Q", "R", 81, 85)
            .Add("R", "S", 86, 90)
            .Add("S", "T", 91, 95)
            .Add("T", "U", 96, 100)
            .Add("U", "V", 101, 105)
            .Add("V", "W", 106, 110)
            .Add("W", "X", 111, 115)
            .Add("X", "Y", 116, 120)
            .Add("Y", "Z", 121, 125)
            .Add("A", "Z", 3, 123)
            .Add("A", "M", 2, 58)
            .Add("B", "L", 7, 59)
            .Add("C", "K", 12, 56)
            .Add("D", "J", 17, 54)
            .Add("E", "I", 22, 52)
            .Add("F", "H", 27, 50)
            .Add("G", "G", 32, 48)
            .Add("H", "F", 37, 46)
            .Add("I", "E", 42, 44)
            .Add("J", "D", 47, 42)
            .Add("K", "C", 52, 40)
            .Add("L", "B", 57, 38)
            .Add("M", "A", 62, 36)
            .Optimize();

    // Shortest path from A to Z is A -> Z, total time = 123
    assert(t.FindWay("A", "Z", 0) == 123);
    // Shortest path from B to V is B -> C -> D -> E -> F -> G -> H -> I -> J -> K
    // -> L -> M -> N -> O -> P -> Q -> R -> S -> T -> U -> V, total time = 105
    assert(t.FindWay("B", "V", 0) == 105);
    // No path from V to A, should throw an exception
    try {
        t.FindWay("V", "A", 0);
        assert("Missing exception" == nullptr);
    } catch (const std::invalid_argument &e) {
    }
}

int main ( void )
{
    CTeleport t;
    t . Add ( "Prague", "Vienna", 0, 7 )
            . Add ( "Vienna", "Berlin", 9, 260 )
            . Add ( "Vienna", "London", 8, 120 )
            . Add ( "Vienna", "Chicago", 4, 3 )
            . Add ( "Prague", "Vienna", 10, 10 ) . Optimize ( );

    assert ( t . FindWay ( "Prague", "Vienna", 0 ) == 7 );
    assert ( t . FindWay ( "Prague", "Vienna", 1 ) == 10 );
    assert ( t . FindWay ( "Prague", "London", 0 ) == 120 );
    assert ( t . FindWay ( "Vienna", "Chicago", 4 ) == 3 );

    try { t . FindWay ( "Prague", "London", 2 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }
    try { t . FindWay ( "Prague", "Chicago", 0 ); assert ( "Missing exception" == nullptr ); }
    catch ( const invalid_argument & e ) { }
    catch ( ... ) { assert ( "Invalid exception" == nullptr ); }

    t . Add ( "Dallas", "Atlanta", 150, 30 )
            . Add ( "Berlin", "Helsinki", 1080, 2560 )
            . Add ( "Chicago", "Frankfurt", 50, 0 )
            . Add ( "Helsinki", "Vienna", 3200, 3 )
            . Add ( "Chicago", "London", 10, 12 )
            . Add ( "London", "Atlanta", 20, 40 )
            . Add ( "Vienna", "Atlanta", 10, 50 )
            . Add ( "Prague", "Vienna", 1, 6 )
            . Add ( "Berlin", "Helsinki", 265, 265 )
            . Add ( "Berlin", "London", 259, 0 ) . Optimize ( );

    assert ( t . FindWay ( "Prague", "Frankfurt", 0 ) == 0 );
    cout << t . FindWay ( "Prague", "Atlanta", 0 ) << endl;
            assert ( t . FindWay ( "Prague", "Atlanta", 0 ) == 40 );
    assert ( t . FindWay ( "Prague", "Atlanta", 10 ) == 50 );

    test1();
    test2();
    test3();
    test5();
    test6();

    return EXIT_SUCCESS;
}
