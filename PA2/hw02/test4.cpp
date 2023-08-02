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

struct CSalary
{
    unsigned m_salary;
    unsigned m_amount;
};

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

    /* Returns position of surname+name in alphabet
        If before than another = 1
        If later than another = -1
        If same = 0
    */
    int relative_name_pos(CEmployee& another) const
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

    int relative_email_pos(CEmployee& another) const
    {
        if (m_email < another.get_email()) return 1;
		else if (m_email == another.get_email()) return 0;
		else return -1;
    }
};

struct compName
{
    bool operator ()(const shared_ptr<CEmployee> a, const shared_ptr<CEmployee> b)
    {
        return ( (a->get_surname() < b->get_surname()) || ((a->get_surname() == b->get_surname()) && (a->get_name() < b->get_name())) );
    }
};
struct compEmail
{
    bool operator ()(const shared_ptr<CEmployee> a, const shared_ptr<CEmployee> b)
    {
        return ( a->get_email() < b->get_email() );
    }
};

class CPersonalAgenda
{
	vector<shared_ptr<CEmployee>> m_byName;
    vector<shared_ptr<CEmployee>> m_byEmail;
    vector<CSalary> m_Salaries;
public:
	CPersonalAgenda  ( void ) {};
	~CPersonalAgenda ( void ) {};

	// Array is going to be sorted in an alphabetical order from A to Z
	bool add ( const string & name, const string & surname, const string & email, unsigned int salary ) // 4n + log(n) - old; new - 2n + 5log(n)
	{
        auto new_employee = make_shared<CEmployee>(name, surname, email, salary);
        CEmployee to_be_checked(name, surname, email, 0);
        if ( bsearch_name(to_be_checked, 0, m_byName.size()-1) >= 0 || bsearch_email(to_be_checked, 0, m_byEmail.size()-1) >= 0 ) return false;
        m_byName.insert(lower_bound(m_byName.begin(), m_byName.end(), new_employee, compName()), new_employee);
        m_byEmail.insert(lower_bound(m_byEmail.begin(), m_byEmail.end(), new_employee, compEmail()), new_employee);

        if (m_Salaries.size() < 1) 
        {
            CSalary a;
            a.m_amount = 1;
            a.m_salary = salary;
            m_Salaries.push_back(a);
        }
        else bsearch_puts(salary, 0, m_Salaries.size()-1);

        return true;
	}

    bool del ( const string & name, const string & surname )
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return false;
        unsigned save_salary = m_byName[pos]->get_salary();
        string save_email = m_byName[pos]->get_email();

        CEmployee to_be_found2("", "", save_email, 0);
        int pos_e = bsearch_email(to_be_found2, 0, m_byEmail.size() - 1);
        if (pos_e < 0) return false;

        m_byEmail.erase(m_byEmail.begin()+pos_e);
        m_byName.erase(m_byName.begin()+pos);
        bsearch_removes(save_salary, 0, m_Salaries.size()-1);
        return true;
    }
    bool del ( const string & email )
    {
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return false;
        unsigned save_salary = m_byEmail[pos]->get_salary();
        string save_name = m_byEmail[pos]->get_name();
        string save_surname = m_byEmail[pos]->get_surname();

        CEmployee to_be_found2(save_name, save_surname, "", 0);
		int pos_n = bsearch_name(to_be_found2, 0, m_byName.size() - 1);
		if (pos_n < 0) return false;

        m_byEmail.erase(m_byEmail.begin()+pos);
        m_byName.erase(m_byName.begin()+pos_n);
        bsearch_removes(save_salary, 0, m_Salaries.size()-1);
        return true;
    }
    bool changeName ( const string & email, const string & newName, const string & newSurname )
    {
        CEmployee to_be_found("", "", email, 0);
		int pos_e = bsearch_email(to_be_found, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;
        //Save for later
        string save_name = m_byEmail[pos_e]->get_name();
        string save_surname = m_byEmail[pos_e]->get_surname();
        unsigned save_salary = m_byEmail[pos_e]->get_salary();

        if (save_name == newName && save_surname == newSurname) return false;

        CEmployee to_be_found3(save_name, save_surname, "", 0);
		int pos_n = bsearch_name(to_be_found3, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        bsearch_removes(save_salary, 0, m_Salaries.size()-1);
        if(!this->add(newName, newSurname, email, save_salary)) 
        {
            this->add(save_name, save_surname, email, save_salary);
            return false;
        }
        return true;
    }
    bool changeEmail ( const string & name, const string & surname, const string & newEmail ) // 6n + 4log(n) + (n)
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos_n = bsearch_name(to_be_found, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;
        //Save for later 
        string save_email = m_byName[pos_n]->get_email();
        unsigned save_salary = m_byName[pos_n]->get_salary();

        if (save_email == newEmail) return false;

        CEmployee to_be_found3("", "", save_email, 0);
		int pos_e = bsearch_email(to_be_found3, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        bsearch_removes(save_salary, 0, m_Salaries.size()-1);
        if(!this->add(name, surname, newEmail, save_salary)) 
        {
            this->add(name, surname, save_email, save_salary);
            return false;
        }
        return true;
    }
    bool setSalary ( const string & name, const string & surname, unsigned int salary ) // 3log(n) + (2n)in some cases
	{
        CEmployee to_be_found(name, surname, "", 0);
		int pos_n = bsearch_name(to_be_found, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;

        bsearch_removes(m_byName[pos_n]->get_salary(), 0, m_Salaries.size()-1);
        bsearch_puts(salary, 0, m_Salaries.size()-1);
        m_byName[pos_n]->set_salary(salary);

        return true;
	}
    bool setSalary ( const string & email, unsigned int salary ) // 3log(n) + (2n)in some cases
	{
        CEmployee to_be_found("", "", email, 0);
		int pos_e = bsearch_email(to_be_found, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;

        bsearch_removes(m_byEmail[pos_e]->get_salary(), 0, m_Salaries.size()-1);
        bsearch_puts(salary, 0, m_Salaries.size()-1);
        m_byEmail[pos_e]->set_salary(salary);

        return true;
	}
    unsigned int getSalary ( const string & name, const string & surname ) const // log(n)
	{
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return 0;
        return m_byName[pos]->get_salary();
	}
    unsigned int getSalary ( const string & email ) const // log(n)
	{
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return 0;
        return m_byEmail[pos]->get_salary();
	}

    //Rank
    bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const // log(n) + n
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return false;

        unsigned sal_to_look = m_byName[pos]->get_salary();
        int i = 0;
        int rm = 0;
        while(m_Salaries[i].m_salary < sal_to_look)
        {
            rm += m_Salaries[i].m_amount;
            i++;
        }
        rankMin = rm;
        rankMax = rm + m_Salaries[i].m_amount - 1;

        return true;
    }
    bool getRank ( const string & email, int & rankMin, int & rankMax ) const // log (n) + n
    {
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return false;

        unsigned sal_to_look = m_byEmail[pos]->get_salary();
        int i = 0;
        int rm = 0;
        while(m_Salaries[i].m_salary < sal_to_look)
        {
            rm += m_Salaries[i].m_amount;
            i++;
        }
        rankMin = rm;
        rankMax = rm + m_Salaries[i].m_amount - 1;

        return true;
    }
    bool getFirst ( string & outName, string & outSurname ) const // 1
	{
        if (m_byName.size() < 1) return false;
        outName = m_byName[0]->get_name();
        outSurname = m_byName[0]->get_surname();
        return true;
	}
    bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const // log(n)
	{
        CEmployee to_be_found(name, surname, "", 0); // How to avoid writing two arguments in the end?
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0 || pos >= (int)(m_byName.size() - 1)) return false;
		else
		{
			outName = m_byName[pos + 1]->get_name();
			outSurname = m_byName[pos + 1]->get_surname();
			return true;
		}
	}
private:
    int bsearch_name(CEmployee key, int start_pos, int end_pos) const // log(n)
	{
        if (start_pos > end_pos) return -1;
		while (start_pos < end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.relative_name_pos(*m_byName[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (start_pos == end_pos && key.relative_name_pos(*m_byName[start_pos]) == 0) return start_pos;
		return -1;
	}

    int bsearch_email(CEmployee key, int start_pos, int end_pos) const // log(n)
	{
        if (start_pos > end_pos) return -1;
		while (start_pos < end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.relative_email_pos(*m_byEmail[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (start_pos == end_pos && key.relative_email_pos(*m_byEmail[start_pos]) == 0) return start_pos;
		return -1;
	}

    void bsearch_removes(unsigned key, int start_pos, int end_pos) // log(n) OR log(n) + n
    {
        while (start_pos < end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
            if(key == m_Salaries[middle].m_salary) 
            {
                m_Salaries[middle].m_amount--;
                if(m_Salaries[middle].m_amount < 1) m_Salaries.erase(m_Salaries.begin()+middle);
                return;
            }
            else if (key < m_Salaries[middle].m_salary) end_pos = middle - 1;
            else start_pos = middle + 1;
		}
        if (key == m_Salaries[start_pos].m_salary) 
        {
            if(--m_Salaries[start_pos].m_amount) 
                m_Salaries.erase(m_Salaries.begin()+start_pos);
        }
    }

    void bsearch_puts(unsigned key, int start_pos, int end_pos) // log(n) OR log(n) + n depends on input
	{
		while (start_pos < end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
            if(key == m_Salaries[middle].m_salary) 
            {
                m_Salaries[middle].m_amount++;
                return;
            }
            else if (key < m_Salaries[middle].m_salary) end_pos = middle - 1;
            else start_pos = middle + 1;
		}
		if (key == m_Salaries[start_pos].m_salary) m_Salaries[start_pos].m_amount++;
        else if (key < m_Salaries[start_pos].m_salary)
        {
            CSalary a;
            a.m_amount = 1;
            a.m_salary = key;
            m_Salaries.insert(m_Salaries.begin()+start_pos, a);
        }
        else
        {
            CSalary a;
            a.m_amount = 1;
            a.m_salary = key;
            m_Salaries.push_back(a);
        }
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
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 1 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 2
           && hi == 2 );
  assert ( b1 . setSalary ( "John", "Smith", 35000 ) );
  assert ( b1 . getSalary ( "John", "Smith" ) ==  35000 );
  assert ( b1 . getSalary ( "john" ) ==  35000 );
  assert ( b1 . getRank ( "John", "Smith", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . getRank ( "peter", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getRank ( "johnm", lo, hi )
           && lo == 1
           && hi == 2 );
  assert ( b1 . changeName ( "peter", "James", "Bond" ) );
  assert ( b1 . getSalary ( "peter" ) ==  23000 );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "James"
           && outSurname == "Bond" );
  assert ( b1 . getNext ( "James", "Bond", outName, outSurname )
           && outName == "John"
           && outSurname == "Miller" );
  assert ( b1 . getNext ( "John", "Miller", outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . changeEmail ( "James", "Bond", "james" ) );
  assert ( b1 . getSalary ( "James", "Bond" ) ==  23000 );
  assert ( b1 . getSalary ( "james" ) ==  23000 );
  assert ( b1 . getSalary ( "peter" ) ==  0 );
  assert ( b1 . del ( "james" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 1 );
  assert ( b1 . del ( "John", "Miller" ) );
  assert ( b1 . getRank ( "john", lo, hi )
           && lo == 0
           && hi == 0 );
  assert ( b1 . getFirst ( outName, outSurname )
           && outName == "John"
           && outSurname == "Smith" );
  assert ( ! b1 . getNext ( "John", "Smith", outName, outSurname ) );
  assert ( b1 . del ( "john" ) );
  assert ( ! b1 . getFirst ( outName, outSurname ) );
  assert ( b1 . add ( "John", "Smith", "john", 31000 ) );
  assert ( b1 . add ( "john", "Smith", "joHn", 31000 ) );
  assert ( b1 . add ( "John", "smith", "jOhn", 31000 ) );

  CPersonalAgenda b2;
  assert ( ! b2 . getFirst ( outName, outSurname ) );
  assert ( b2 . add ( "James", "Bond", "james", 70000 ) );
  assert ( b2 . add ( "James", "Smith", "james2", 30000 ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( ! b2 . add ( "James", "Bond", "james3", 60000 ) );
  assert ( ! b2 . add ( "Peter", "Bond", "peter", 50000 ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . setSalary ( "Joe", "Black", 90000 ) );
  assert ( ! b2 . setSalary ( "joe", 90000 ) );
  assert ( b2 . getSalary ( "Joe", "Black" ) ==  0 );
  assert ( b2 . getSalary ( "joe" ) ==  0 );
  assert ( ! b2 . getRank ( "Joe", "Black", lo, hi ) );
  assert ( ! b2 . getRank ( "joe", lo, hi ) );
  assert ( ! b2 . changeName ( "joe", "Joe", "Black" ) );
  assert ( ! b2 . changeEmail ( "Joe", "Black", "joe" ) );
  assert ( ! b2 . del ( "Joe", "Black" ) );
  assert ( ! b2 . del ( "joe" ) );
  assert ( ! b2 . changeName ( "james2", "James", "Bond" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "james" ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter" ) );
  assert ( b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . changeEmail ( "Peter", "Smith", "peter2" ) );
  assert ( ! b2 . setSalary ( "Peter", "Smith", 35000 ) );
  assert ( b2 . getSalary ( "Peter", "Smith" ) ==  0 );
  assert ( ! b2 . getRank ( "Peter", "Smith", lo, hi ) );
  assert ( ! b2 . changeName ( "peter", "Peter", "Falcon" ) );
  assert ( ! b2 . setSalary ( "peter", 37000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  0 );
  assert ( ! b2 . getRank ( "peter", lo, hi ) );
  assert ( ! b2 . del ( "Peter", "Smith" ) );
  assert ( ! b2 . del ( "peter" ) );
  assert ( b2 . add ( "Peter", "Smith", "peter", 40000 ) );
  assert ( b2 . getSalary ( "peter" ) ==  40000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
