#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <iostream> 
#include <iomanip> 
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <memory>
using namespace std;
#endif /* __PROGTEST__ */

class CEmployee
{
	string m_surname;
	string m_name;
	string m_email;
	unsigned m_salary;
public:
	CEmployee(string name = nullptr, string surname = nullptr, string email = nullptr, unsigned salary = 0)
	{
		m_name = name;
		m_surname = surname;
		m_email = email;
		m_salary = salary;
	}
	string get_name() const {return m_name;}
	string get_surname() const {return m_surname;}
	string get_email() const {return m_email;}
	unsigned get_salary() const {return m_salary;}

	void set_name(string name) {m_name = name;}
	void set_surname(string surname) {m_surname = surname;}
	void set_email(string email) {m_email = email;}
	void set_salary(unsigned salary) {m_salary = salary;}

	int get_alpha_pos(CEmployee another)
	{
		if (m_surname < another.get_surname()) return 1;
		else if (m_surname == another.get_surname())
		{
			if (m_name < another.get_name()) return 1;
			else if (m_name == another.get_name()) return 0;
			else return -1;
		}
		else return -1;
	}

	bool is_unique_email (CEmployee another)
	{
		if (m_email == another.get_email()) return false;
		return true;
	}

	int get_email_pos(CEmployee another)
	{
		if (m_email < another.get_email()) return 1;
		else if (m_email == another.get_email()) return 0;
		else return -1;
	}

	int get_salary_pos(CEmployee another)
	{
		if (m_salary < another.get_salary()) return 1;
		else if (m_salary < another.get_salary()) return 0;
		else return -1;
	}
};

class CPersonalAgenda
{
	vector<CEmployee> m_database;
	// State: 0 - sorted by names
	//		  1 - sorted by emails
	//		  2 - sorted by salaries 
	int m_state = 0;
public:
	// int set_state(int state) {m_state = state;}
	// int get_state() {return m_state;}

	CPersonalAgenda  ( void ) {};
	~CPersonalAgenda ( void ) {};

	// Array is going to be sorted in an alphabetical order from A to Z
	bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
	{
		CEmployee Worm(name, surname, email, salary);
		bool placed = false;
		size_t pos = 0;
		for (size_t i = 0, len = m_database.size(); i < len; ++i)
		{
			if (!Worm.is_unique_email(m_database[i])) return false;
			int res = Worm.get_alpha_pos(m_database[i]);
			if (res == 0) return false;
			else if (placed == false && res > 0) 
			{
				placed = true;
				pos = i;
			}
		}
		if (placed == true) m_database.insert(m_database.begin()+pos, Worm);
		else m_database.push_back(Worm);
		return true;
	}

    // bool del ( const string & name, const string & surname );
    // bool del ( const string    & email );
    // bool changeName ( const string & email, const string & newName, const string & newSurname );
    // bool changeEmail ( const string & name, const string & surname, const string & newEmail );
    bool setSalary ( const string & name, const string & surname, unsigned int salary )
	{
		if (m_state != 0)
		{
			sort(m_database.begin(), m_database.end(), compare_name);
			m_state = 1;
		}
		CEmployee to_be_found(name, surname, "", 0); // How to avoid writing two arguments in the end?
		int pos = bsearch_name(to_be_found, 0, m_database.size() - 1);
		if (pos < 0) return false;
		m_database[pos].set_salary(salary);
		return true;
	}
    bool setSalary ( const string & email, unsigned int salary )
	{
		if (m_state != 1)
		{
			sort(m_database.begin(), m_database.end(), compare_email);
			m_state = 1;
		}
		CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_database.size() - 1);
		if (pos < 0) return false;
		m_database[pos].set_salary(salary);
		return true;
	}
    unsigned int getSalary ( const string & name, const string & surname )
	{
		if (m_state != 0)
		{
			sort(m_database.begin(), m_database.end(), compare_name);
			m_state = 1;
		}
		CEmployee to_be_found(name, surname, "", 0); // How to avoid writing two arguments in the end?
		int pos = bsearch_name(to_be_found, 0, m_database.size() - 1);
		return m_database[pos].get_salary();
	}
    unsigned int getSalary ( const string & email )
	{
		if (m_state != 1)
		{
			sort(m_database.begin(), m_database.end(), compare_email);
			m_state = 1;
		}
		CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_database.size() - 1);
		return m_database[pos].get_salary();
	}
    // bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const;
    // bool getRank ( const string & email, int & rankMin, int & rankMax ) const;
    bool getFirst ( string & outName, string & outSurname )
	{
		if (m_state != 0)
		{
			sort(m_database.begin(), m_database.end(), compare_name);
			m_state = 1;
		}
		if(m_database.size() == 0) return false;
		outName = m_database[0].get_name();
		outSurname = m_database[0].get_surname();
		return true;
	}
    bool getNext ( const string & name, const string & surname, string & outName, string & outSurname )
	{
		if (m_state != 0)
		{
			sort(m_database.begin(), m_database.end(), compare_name);
			m_state = 1;
		}
		CEmployee to_be_found(name, surname, "", 0); // How to avoid writing two arguments in the end?
		int pos = bsearch_name(to_be_found, 0, m_database.size() - 1);
		if (pos < 0 || pos >= (int)(m_database.size() - 1)) return false;
		else
		{
			outName = m_database[pos + 1].get_name();
			outSurname = m_database[pos + 1].get_surname();
			return true;
		}
	}
private:
	int bsearch_name(CEmployee key, int start_pos, int end_pos) const // This function returns a position in an array
	{
		while (start_pos != end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.get_alpha_pos(m_database[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (key.get_alpha_pos(m_database[start_pos]) == 0) return start_pos;
		return -1;
	}

	int bsearch_email(CEmployee key, int start_pos, int end_pos) const // This function returns a position in an array
	{
		while (start_pos != end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.get_email_pos(m_database[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (key.get_email_pos(m_database[start_pos]) == 0) return start_pos;
		return -1;
	}

	static bool compare_email(CEmployee a, CEmployee b)
	{
		return (a.get_email_pos(b) > 0);
	}
	static bool compare_name(CEmployee a, CEmployee b)
	{
		return (a.get_alpha_pos(b) > 0);
	}
	static bool compare_salary(CEmployee a, CEmployee b) //true if a less than b
	{
		return (a.get_salary_pos(b) > 0);
	}
};

#ifndef __PROGTEST__
int main ( void )
{
  string outName, outSurname;
  int lo, hi;

  CPersonalAgenda b1;
  assert ( b1 . add ( "John", "Smith", "john", 30000 ) );
  assert ( b1 . add ( "John", "Miller", "johnm", 35000 ) );
  assert ( b1 . add ( "Peter", "Smith", "peter", 23000 ) );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( b1 . getNext ( "John", "Smith", outName, outSurname )
           && outName == "Peter"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "Peter", "Smith", outName, outSurname ) );
  assert ( b1 . setSalary ( "john", 32000 ) );
  assert ( b1 . getSalary ( "john" ) ==  32000 );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  32000 );
  // assert ( b1 . getRank ( "John", "Smith", lo, hi )
  //          && lo == 1
  //          && hi == 1 );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 1
  //          && hi == 1 );
  // assert ( b1 . getRank ( "peter", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getRank ( "johnm", lo, hi )
  //          && lo == 2
  //          && hi == 2 );
  // assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  // assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  // assert ( b1 . getSalary ( "john" ) ==  35000 );
  // assert ( b1 . getRank ( "John", "Smith", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  // assert ( b1 . getRank ( "peter", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getRank ( "johnm", lo, hi )
  //          && lo == 1
  //          && hi == 2 );
  // assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  // assert ( b1 . getSalary ( "peter" ) ==  23000 );
  // assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  // assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  // assert ( b1 . getFirst ( outName, outSurname )
  //          && outName == "James"
  //          && outSurname == "Bond" );
  // assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Miller" );
  // assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Smith" );
  // assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  // assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  // assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  // assert ( b1 . getSalary ( "james" ) ==  23000 );
  // assert ( b1 . getSalary ( "peter" ) ==  0 );
  // assert ( b1 . del ( "james" ) );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 0
  //          && hi == 1 );
  // assert ( b1 . del ( "John", "Miller" ) );
  // assert ( b1 . getRank ( "john", lo, hi )
  //          && lo == 0
  //          && hi == 0 );
  // assert ( b1 . getFirst ( outName, outSurname )
  //          && outName == "John"
  //          && outSurname == "Smith" );
  // assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  // assert ( b1 . del ( "john" ) );
  // assert ( ! b1 . getFirst ( outName, outSurname ) );
  // assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  // assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  // assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  // CPersonalAgenda b2;
  // assert ( ! b2 . getFirst ( outName, outSurname ) );
  // assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  // assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  // assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  // assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  // assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  // assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  // assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  // assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  // assert ( ! b2 . setSalary ( "joe", 90000 ) );
  // assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  // assert ( b2 . getSalary ( "joe" ) ==  0 );
  // assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  // assert ( ! b2 . getRank ( "joe", lo, hi ) );
  // assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  // assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  // assert ( ! b2 . del ( "Joe", "Black" ) );
  // assert ( ! b2 . del ( "joe" ) );
  // assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  // assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  // assert ( b2 . del ( "Peter", "Smith" ) );
  // assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  // assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  // assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  // assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  // assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  // assert ( ! b2 . setSalary ( "peter", 37000 ) );
  // assert ( b2 . getSalary ( "peter" ) ==  0 );
  // assert ( ! b2 . getRank ( "peter", lo, hi ) );
  // assert ( ! b2 . del ( "Peter", "Smith" ) );
  // assert ( ! b2 . del ( "peter" ) );
  // assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  // assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
