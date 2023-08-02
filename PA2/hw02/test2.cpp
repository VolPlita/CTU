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
    unsigned m_same_sal;
public:
	CEmployee(string name = nullptr, string surname = nullptr, string email = nullptr, unsigned salary = 0)
	{
		m_name = name;
		m_surname = surname;
		m_email = email;
		m_salary = salary;
        m_same_sal = 1;
	}
	string get_name() const {return m_name;}
	string get_surname() const {return m_surname;}
	string get_email() const {return m_email;}
	unsigned get_salary() const {return m_salary;}
    unsigned get_same_sal() const {return m_same_sal;}

	void set_name(string name) {m_name = name;}
	void set_surname(string surname) {m_surname = surname;}
	void set_email(string email) {m_email = email;}
	void set_salary(unsigned salary) {m_salary = salary;}
    void set_same_sal(unsigned a) {m_same_sal = a;}

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

    int relative_sal_pos(CEmployee& another) const
    {
        if (m_salary < another.get_salary()) return 1;
		else if (m_salary == another.get_salary()) return 0;
		else return -1;
    }
};

class CPersonalAgenda
{
	vector<shared_ptr<CEmployee>> m_byName;
    vector<shared_ptr<CEmployee>> m_byEmail;
    vector<shared_ptr<CEmployee>> m_bySalary;

public:
	CPersonalAgenda  ( void ) {};
	~CPersonalAgenda ( void ) {};

	// Array is going to be sorted in an alphabetical order from A to Z
	bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
	{
        auto new_employee = make_shared<CEmployee>(name, surname, email, salary);

        bool inserted_n = false;
        bool inserted_e = false;
        bool inserted_s = false;
        size_t pos_n = 0;
        size_t pos_e = 0;
        size_t pos_s = 0;
        //Iterate through three arrays at one time
        for (size_t i = 0, k = m_byName.size(); i < k; ++i)
        {
            // Validate
            int res_n = new_employee->relative_name_pos(*m_byName[i]);
            int res_e = new_employee->relative_email_pos(*m_byEmail[i]);
            if (res_n == 0) return false;
            if (res_e == 0) return false;
            // Find place for name
            if(inserted_n == false && res_n > 0)
            {
                inserted_n = true;
                pos_n = i;
            }
            // Find place for email
            if (inserted_e == false && res_e > 0)
            {
                inserted_e = true;
                pos_e = i;
            }
            // Find place for salary
            int res_s = new_employee->relative_sal_pos(*m_bySalary[i]);
			if (inserted_s == false && res_s >= 0)
			{
                if(res_s > 0)
				{
				    inserted_s = true;
				    pos_s = i;
				}
				else if (res_s == 0)
				{
					inserted_s = true;
					pos_s = i;
					new_employee->set_same_sal(m_bySalary[i]->get_same_sal() + 1);
                    //m_bySalary[i]->set_same_sal(m_bySalary[i]->get_same_sal() + 1);
				}
			}
        }
        
        if (inserted_n == true) m_byName.insert(m_byName.begin()+pos_n, new_employee);
		else m_byName.push_back(new_employee);
		if (inserted_e == true) m_byEmail.insert(m_byEmail.begin()+pos_e, new_employee);
		else m_byEmail.push_back(new_employee);
		if (inserted_s == true) m_bySalary.insert(m_bySalary.begin()+pos_s, new_employee);
		else m_bySalary.push_back(new_employee);
        return true;
	}

    bool del ( const string & name, const string & surname )
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return false;
        for (size_t i = 0, k = m_byEmail.size(); i < k; ++i)
        {
            if (m_byEmail[i] == m_byName[pos]) 
            {
                m_byEmail.erase(m_byEmail.begin()+i);
                break;
            }
        }
        for (size_t i = 0, k = m_bySalary.size(); i < k; ++i)
        {
            if (m_bySalary[i] == m_byName[pos]) 
            {
                m_bySalary.erase(m_bySalary.begin()+i);
                break;
            }
        }
        m_byName.erase(m_byName.begin()+pos);
        return true;
    }
    bool del ( const string & email )
    {
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return false;
        for (size_t i = 0, k = m_byName.size(); i < k; ++i)
        {
            if (m_byName[i] == m_byEmail[pos]) 
            {
                m_byName.erase(m_byName.begin()+i);
                break;
            }
        }
        for (size_t i = 0, k = m_bySalary.size(); i < k; ++i)
        {
            if (m_bySalary[i] == m_byEmail[pos]) 
            {
                m_bySalary.erase(m_bySalary.begin()+i);
                break;
            }
        }
        m_byEmail.erase(m_byEmail.begin()+pos);
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
    
        CEmployee to_be_found2("", "", "", m_byEmail[pos_e]->get_salary());
		int pos_s = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1); // Found in salary
		if (pos_s < 0) return false;

        CEmployee to_be_found3(save_name, save_surname, "", 0);
		int pos_n = bsearch_name(to_be_found3, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        m_bySalary.erase(m_bySalary.begin()+pos_s);
        if(!this->add(newName, newSurname, email, save_salary)) 
        {
            this->add(save_name, save_surname, email, save_salary);
            return false;
        }
        return true;
    }
    bool changeEmail ( const string & name, const string & surname, const string & newEmail )
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos_n = bsearch_name(to_be_found, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;
        //Save for later 
        string save_email = m_byName[pos_n]->get_email();
        unsigned save_salary = m_byName[pos_n]->get_salary();

        if (save_email == newEmail) return false;
        
        CEmployee to_be_found2("", "", "", m_byName[pos_n]->get_salary());
		int pos_s = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1); // Found in salary
		if (pos_s < 0) return false;

        CEmployee to_be_found3("", "", save_email, 0);
		int pos_e = bsearch_email(to_be_found3, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        m_bySalary.erase(m_bySalary.begin()+pos_s);
        if(!this->add(name, surname, newEmail, save_salary)) 
        {
            this->add(name, surname, save_email, save_salary);
            return false;
        }
        return true;
    }
    bool setSalary ( const string & name, const string & surname, unsigned int salary )
	{
        CEmployee to_be_found(name, surname, "", 0);
		int pos_n = bsearch_name(to_be_found, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;
        //Save for later 
        string save_email = m_byName[pos_n]->get_email();
        unsigned old_sal = m_byName[pos_n]->get_salary();
        
        CEmployee to_be_found2("", "", "", old_sal);
		int pos_s = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1); // Found in salary
		if (pos_s < 0) return false;

        CEmployee to_be_found3("", "", save_email, 0);
		int pos_e = bsearch_email(to_be_found3, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        m_bySalary.erase(m_bySalary.begin()+pos_s);
        if(!this->add(name, surname, save_email, salary)) 
        {
            this->add(name, surname, save_email, old_sal);
            return false;
        }
        return true;
	}
    bool setSalary ( const string & email, unsigned int salary )
	{
        CEmployee to_be_found("", "", email, 0);
		int pos_e = bsearch_email(to_be_found, 0, m_byEmail.size() - 1); // Found in email
		if (pos_e < 0) return false;
        //Save for later
        string save_name = m_byEmail[pos_e]->get_name();
        string save_surname = m_byEmail[pos_e]->get_surname();
        unsigned old_sal = m_byEmail[pos_e]->get_salary();

        CEmployee to_be_found2("", "", "", old_sal);
		int pos_s = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1); // Found in salary
		if (pos_s < 0) return false;

        CEmployee to_be_found3(save_name, save_surname, "", 0);
		int pos_n = bsearch_name(to_be_found3, 0, m_byName.size() - 1); // Found in names
		if (pos_n < 0) return false;

        //Erase all pointers
        m_byName.erase(m_byName.begin()+pos_n);
        m_byEmail.erase(m_byEmail.begin()+pos_e);
        m_bySalary.erase(m_bySalary.begin()+pos_s);
        if(!this->add(save_name, save_surname, email, salary)) 
        {
            //! If change fails here, pointers above should not be erased
            //Add them back
            this->add(save_name, save_surname, email, old_sal);
            return false; 
        }
        return true;
	}
    unsigned int getSalary ( const string & name, const string & surname ) const
	{
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return false;
        return m_byName[pos]->get_salary();
	}
    unsigned int getSalary ( const string & email ) const
	{
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return false;
        return m_byEmail[pos]->get_salary();
	}

    //Rank
    bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
    {
        CEmployee to_be_found(name, surname, "", 0);
		int pos = bsearch_name(to_be_found, 0, m_byName.size() - 1);
		if (pos < 0) return false;

        CEmployee to_be_found2("", "", "", m_byName[pos]->get_salary());
        size_t i = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1);
        //Get to the max
        // vector<(*m_bySalary)>::iterator ptr;
        // advance(ptr, pos);
        while (  i != (m_bySalary.size() - 1) && m_bySalary[i]->get_salary() == m_bySalary[i+1]->get_salary()) {i+=1;}
        rankMax = i;
        while (i != 0 && m_bySalary[i]->get_salary() == m_bySalary[i-1]->get_salary()) {i-=1;}
        rankMin = i;
        //rankMin = rankMax - m_bySalary[pos]->get_same_sal() + 1;
        return true;
    }
    bool getRank ( const string & email, int & rankMin, int & rankMax ) const
    {
        CEmployee to_be_found("", "", email, 0);
		int pos = bsearch_email(to_be_found, 0, m_byEmail.size() - 1);
		if (pos < 0) return false;

        CEmployee to_be_found2("", "", "", m_byEmail[pos]->get_salary());
        size_t i = bsearch_salary(to_be_found2, 0, m_bySalary.size() - 1);
        //Get to the max
        while ( i != (m_bySalary.size() - 1) && m_bySalary[i]->get_salary() == m_bySalary[i+1]->get_salary()) {i+=1;}
        rankMax = i;
        while ( i != 0 && m_bySalary[i]->get_salary() == m_bySalary[i-1]->get_salary()) {i-=1;}
        rankMin = i;
        //rankMin = rankMax - m_bySalary[pos]->get_same_sal() + 1;
        return true;
    }
    bool getFirst ( string & outName, string & outSurname ) const
	{
        if (m_byName.size() == 0) return false;
        outName = m_byName[0]->get_name();
        outSurname = m_byName[0]->get_surname();
        return true;
	}
    bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
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
    int bsearch_name(CEmployee key, int start_pos, int end_pos) const // This function returns a position in an array
	{
		while (start_pos != end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.relative_name_pos(*m_byName[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (key.relative_name_pos(*m_byName[start_pos]) == 0) return start_pos;
		return -1;
	}

    int bsearch_email(CEmployee key, int start_pos, int end_pos) const // This function returns a position in an array
	{
		while (start_pos != end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.relative_email_pos(*m_byEmail[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (key.relative_email_pos(*m_byEmail[start_pos]) == 0) return start_pos;
		return -1;
	}

    int bsearch_salary(CEmployee key, int start_pos, int end_pos) const // This function returns a position in an array
	{
		while (start_pos != end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = key.relative_sal_pos(*m_bySalary[middle]);
			if (res == 0) return middle;
			else if (res < 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		if (key.relative_sal_pos(*m_bySalary[start_pos]) == 0) return start_pos;
		return -1;
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
  //b1 . getRank ( "John", "Smith", lo, hi );
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
// b2.add ( "Fucker", "Bond", "fuch", 70000 );
// b2.add ( "Travnicek", "Bond", "travnja", 70000 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
