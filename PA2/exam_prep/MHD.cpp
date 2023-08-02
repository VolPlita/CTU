//The task was to implement the CPublicTransport class,
// which registers lines with stops and allows searching for the cheapest route from one stop to another.
//
//The lines are two-way and when passengers get on at any of the stops on the line,
// they pay the fee specified for that line, but it doesn't matter how many stops they pass on that line.
// If he transfers to a stop belonging to another line,
// he pays the fee associated with that line and again it does not matter how many stops he passes on that line.
//
//The CPublicTransport class contains the following interfaces:
//
//Default constructor to create an empty instance
//
//addLine(const, stops) - the method adds a line represented as a vector of stops, which has a fee cost
//
//optimize() - method optimizes internal structures
//
//findCheapest(from, to) - method finds the cheapest way to get from station from to station to.
// If no connection between from and to is found, the method throws an exception of type std::logic_error ,
// otherwise it returns the fee for the cheapest connection between from and to
//
//The line charges were small numbers, I think in the range of 1-10.
//
//I am writing the entry and the template from memory,
// so it may not be completely accurate, if you come across any mistakes, please correct them.

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

struct CLine
{
    unsigned m_cost;
    unordered_set<string> m_line;
    CLine(unsigned cost, const vector<string>& stops) : m_cost(cost)
    {
        for (size_t i = 0, l = stops.size(); i < l; ++i)
        {
            m_line.insert(stops[i]);
        }
    }
};

class CHash
{
public:
    size_t operator () (const pair<size_t, size_t> & x) const
    {
        return hash<size_t>()(x.first) ^ hash<size_t>()(x.second);
    }
};

class CPublicTransport {
    vector<CLine> mhd;
    vector<pair<size_t, size_t>> graph;

    unordered_set<size_t> V;

    unsigned DFS(size_t s, size_t f)
    {
        unsigned price = mhd[s].m_cost;
        if (s == f) return price;

        vector<size_t> neighbors;
        for (size_t i = 0, l = graph.size(); i < l; ++i) // Find neighbors
        {
            if (graph[i].first == s && V.find(graph[i].second) == V.end())
            {
                neighbors.push_back(graph[i].second);
            }
            else if (graph[i].second == s && V.find(graph[i].first) == V.end())
            {
                neighbors.push_back(graph[i].first);
            }
        }

        unsigned current_min = -1;
        for (size_t i = 0, l = neighbors.size(); i < l; ++i) // For all neighbors
        {
            V.insert(neighbors[i]);
            unsigned result = DFS(neighbors[i], f);
            if (result < current_min) current_min = result;
            V.erase(neighbors[i]);
        }
        price += current_min;
        return price;
    }

public:
    CPublicTransport()= default;

    CPublicTransport& addLine(unsigned cost, const vector<string>& stops)
    {
        mhd.emplace_back(cost, stops);
        size_t mhdlen = mhd.size() - 1;
        for (const string & station : mhd[mhdlen].m_line)
        {
            for (size_t i = 0; i < mhdlen; ++i)
            {
                if (mhd[i].m_line.find(station) != mhd[i].m_line.end()) // All except for the last one (just added)
                {
                    graph.emplace_back(i, mhdlen);
                }
            }
        }
        return *this;
    }

    CPublicTransport& optimize()
    {
        // Remove duplicates??
    }

    unsigned findCheapest(const string& from, const string& to)
    {
        if (from == to) return 0;
        vector<size_t> start;
        vector<size_t> finish;
        for (size_t i = 0, l = mhd.size(); i < l; ++i)
        {
            if (mhd[i].m_line.find(from) != mhd[i].m_line.end()) start.push_back(i);
            if (mhd[i].m_line.find(to) != mhd[i].m_line.end()) finish.push_back(i);
        }
        vector<pair<size_t, size_t>> start_finish;
        for (size_t i = 0, l = start.size(); i < l; ++i) {
            for (size_t j = 0, m = finish.size(); j < m; ++j) {
                start_finish.emplace_back(start[i], finish[j]);
            }
        }
        // Do DFS for each pair
        unsigned min_price = -1;
        for (size_t i = 0, l = start_finish.size(); i < l; ++i)
        {
            unsigned cur_price = DFS(start_finish[i].first, start_finish[i].second);
            if (cur_price < min_price) min_price = cur_price;
        }
        return min_price;
    }
};

int main() {

    CPublicTransport t;

    t.addLine(1, {"Staromestska", "Muzeum", "Mustek", "Namesti Miru", "Jiriho z Podebrad"}).
            addLine(3, {"Hlavni Nadrazi", "Orionka", "Baterie", "Petriny", "Jiriho z Podebrad"}).
            addLine(2, {"Kamenicka", "Letenske namesti", "Baterie", "Petriny", "Namesti Miru", "Malovanka"}).optimize();

    cout << t.findCheapest("Staromestska", "Baterie") << endl;
    assert(t.findCheapest("Staromestska", "Baterie") == 3);
    cout << t.findCheapest("Staromestska", "Staromestska") << endl;
    assert(t.findCheapest("Staromestska", "Staromestska") == 0);
    cout << t.findCheapest("Staromestska", "Namesti Miru") << endl;
    assert(t.findCheapest("Staromestska", "Namesti Miru") == 1);
    cout << t.findCheapest("Staromestska", "Hlavni Nadrazi") << endl;
    assert(t.findCheapest("Staromestska", "Hlavni Nadrazi") == 4);
    cout << t.findCheapest("Orionka", "Kamenicka") << endl;
    assert(t.findCheapest("Orionka", "Kamenicka") == 5);

    t.addLine(6, {"Vitezne namesti", "I P Pavlova", "Andel"});

    try {t.findCheapest("Orionka", "Andel"); }
    catch ( const logic_error & e ) { assert(e.what() != "");}

}

