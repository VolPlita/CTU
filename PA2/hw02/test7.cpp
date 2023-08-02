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
    CSalary * lesser;
    CSalary * bigger;
    CSalary(unsigned salary)
    {
        m_salary = salary;
        m_amount = 1;
        lesser = nullptr;
        bigger = nullptr;
    }
};

class CEmployee
{
	string m_name;
	string m_surname;
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

struct CNode
{
    shared_ptr<CEmployee> Employee;
    CNode * lesser;
    CNode * bigger;
    CNode(shared_ptr<CEmployee>& a)
    {
        lesser = nullptr;
        bigger = nullptr;
        Employee = a;
    }
};

class CPersonalAgenda
{
    CNode * root_name;
    CNode * root_email;
    CSalary * root_salary;
public:
	CPersonalAgenda  ( void ) { root_name = nullptr, root_email = nullptr, root_salary= nullptr;}
	~CPersonalAgenda ( void ) { delete_tree(root_name), delete_tree(root_email), delete_tree(root_salary) ;}

	// Array is going to be sorted in an alphabetical order from A to Z
	bool add ( const string & name, const string & surname, const string & email, unsigned int salary )
	{
        CEmployee to_be_checked(name, surname, email, 0);
        if (find_name(root_name, to_be_checked) != nullptr || find_email(root_email, to_be_checked) != nullptr) return false;
        auto new_employee = make_shared<CEmployee>(name, surname, email, salary);
        root_name = insert_node_name(root_name, new_employee);
        root_email = insert_node_email(root_email, new_employee);
        root_salary = insert_node_salary(root_salary, salary);

        return true;
	}
    bool del ( const string & name, const string & surname )
    {
        CEmployee to_be_found(name, surname, "", 0);
        CNode * tmp;
		if ((tmp = find_name(root_name, to_be_found)) == nullptr) return false;
        to_be_found.set_email(tmp->Employee->get_email());
        to_be_found.set_salary(tmp->Employee->get_salary());

        root_name = delete_node_name(root_name, to_be_found);
        root_email = delete_node_email(root_email, to_be_found);
        root_salary = remove_node_salary(root_salary, to_be_found.get_salary());
        
        return true;
    }
    bool del ( const string & email )
    {
        CEmployee to_be_found("", "", email, 0);
        CNode * tmp;
		if ((tmp = find_email(root_email, to_be_found)) == nullptr) return false;
        to_be_found.set_name(tmp->Employee->get_name());
        to_be_found.set_surname(tmp->Employee->get_surname());
        to_be_found.set_salary(tmp->Employee->get_salary());

        root_name = delete_node_name(root_name, to_be_found);
        root_email = delete_node_email(root_email, to_be_found);
        root_salary = remove_node_salary(root_salary, to_be_found.get_salary());
        
        return true;
    }
    bool changeName ( const string & email, const string & newName, const string & newSurname )
    {
        CEmployee to_be_found(newName, newSurname, email, 0);
        CNode * tmp;
        //Check
		if ((tmp = find_email(root_email, to_be_found)) == nullptr ||
             find_name(root_name, to_be_found) != nullptr) return false;
        
        CEmployee to_be_found2(tmp->Employee->get_name(), tmp->Employee->get_surname(), email, 0);
        root_name = delete_node_name(root_name, to_be_found2);
        tmp->Employee->set_name(newName);
        tmp->Employee->set_surname(newSurname);
        root_name = insert_node_name(root_name, tmp->Employee);
        
        return true;
    }
    bool changeEmail ( const string & name, const string & surname, const string & newEmail )
    {
        CEmployee to_be_found(name, surname, newEmail, 0);
        CNode * tmp;
        //Check
		if ((tmp = find_name(root_name, to_be_found)) == nullptr ||
             find_email(root_email, to_be_found) != nullptr) return false;

        CEmployee to_be_found2(name, surname, tmp->Employee->get_email(), 0);
        root_email = delete_node_email(root_email, to_be_found2);
        tmp->Employee->set_email(newEmail);
        root_email = insert_node_email(root_email, tmp->Employee);
        
        return true;
    }
    bool setSalary ( const string & name, const string & surname, unsigned int salary )
	{
        CEmployee to_be_found(name, surname, "", 0);
		CNode * look = find_name(root_name, to_be_found);
		if (look == nullptr) return false;
        unsigned old_salary = look->Employee->get_salary();
        look->Employee->set_salary(salary);
        root_salary = remove_node_salary(root_salary, old_salary);
        root_salary = insert_node_salary(root_salary, salary); //-

        return true;
	}
    bool setSalary ( const string & email, unsigned int salary )
	{
        CEmployee to_be_found("", "", email, 0);
		CNode * look = find_email(root_email, to_be_found);
		if (look == nullptr) return false;
        unsigned old_salary = look->Employee->get_salary();
        look->Employee->set_salary(salary);
        root_salary = remove_node_salary(root_salary, old_salary);
        root_salary = insert_node_salary(root_salary, salary); //-

        return true;
	}
    unsigned int getSalary ( const string & name, const string & surname ) const
	{
        CEmployee to_be_found(name, surname, "", 0);
		CNode * look = find_name(root_name, to_be_found);
		if (look == nullptr) return 0;
        return look->Employee->get_salary();
	}
    unsigned int getSalary ( const string & email ) const
	{
        CEmployee to_be_found("", "", email, 0);
		CNode * look = find_email(root_email, to_be_found);
		if (look == nullptr) return 0;
        return look->Employee->get_salary();
	}

    //Rank
    bool getRank ( const string & name, const string & surname, int & rankMin, int & rankMax ) const
    {
        CEmployee to_be_found(name, surname, "", 0);
		CNode * look = find_name(root_name, to_be_found);
		if (look == nullptr) return false;

        unsigned sal_to_look = look->Employee->get_salary();
        unsigned rm = 0;
        CSalary * tmp_root = root_salary;
        while(sal_to_look != tmp_root->m_salary) // If problem comes up here, then smth else is incorrect
        {
            if (sal_to_look < tmp_root->m_salary)
                tmp_root = tmp_root->lesser;
            else
            {
                rm += tmp_root->m_amount;
                rm += sum_sal_nodes(tmp_root->lesser);
                tmp_root = tmp_root->bigger;
            }
        }
        // Get sum of all salaries amounts lesser than searched
        rm += sum_sal_nodes(tmp_root->lesser);
        rankMin = rm;
        rankMax = rm + tmp_root->m_amount - 1;

        return true;
    }
    bool getRank ( const string & email, int & rankMin, int & rankMax ) const
    {
        CEmployee to_be_found("", "", email, 0);
		CNode * look = find_email(root_email, to_be_found);
		if (look == nullptr) return false;

        unsigned sal_to_look = look->Employee->get_salary();
        unsigned rm = 0;
        CSalary * tmp_root = root_salary;
        // Get dow to the searched salary
        while(sal_to_look != tmp_root->m_salary) // If problem comes up here, then smth else is incorrect
        {
            if (sal_to_look < tmp_root->m_salary)
                tmp_root = tmp_root->lesser;
            else
            {
                rm += tmp_root->m_amount;
                rm += sum_sal_nodes(tmp_root->lesser);
                tmp_root = tmp_root->bigger;
            }
        }
        // Get sum of all salaries amounts lesser than searched
        // unsigned rm = 0;
        // sum_until(root_salary, sal_to_look, rm);
        rm += sum_sal_nodes(tmp_root->lesser);
        rankMin = rm;
        rankMax = rm + tmp_root->m_amount - 1;

        return true;
    }
    bool getFirst ( string & outName, string & outSurname ) const
	{
        if (root_name == nullptr) return false;
        CNode * look = root_name;
        while (look->lesser != nullptr) look = look->lesser;
        outName = look->Employee->get_name();
        outSurname = look->Employee->get_surname();
        return true;
	}
    bool getNext ( const string & name, const string & surname, string & outName, string & outSurname ) const
	{
        CEmployee to_be_found(name, surname, "", 0);
		CNode * look = find_name(root_name, to_be_found);
        if (look == nullptr) return false;
        CNode * succ = find_successor(root_name, look);
        if (succ == nullptr) return false;
        outName = succ->Employee->get_name();
        outSurname = succ->Employee->get_surname();
        return true;
	}
private:
    CNode * find_name(CNode * root, CEmployee& to_be_found) const // Create an object to_be_found before using
    {
        if (root == nullptr)
            return nullptr; // or root
        int res = to_be_found.relative_name_pos(*root->Employee);
        if (res == 0)
            return root;
        else if (res > 0)
            return find_name(root->lesser, to_be_found);
        else if (res < 0) 
            return find_name(root->bigger, to_be_found);
        else
            return nullptr;
    }
    CNode * find_email(CNode * root, CEmployee& to_be_found) const // Create an object to_be_found before using
    {
        if (root == nullptr)
            return root;
        int res = to_be_found.relative_email_pos(*root->Employee);
        if (res == 0)
            return root;
        else if (res > 0)
            return find_email(root->lesser, to_be_found);
        else if (res < 0) 
            return find_email(root->bigger, to_be_found);
        else 
            return nullptr;
    }
    CSalary * find_salary(CSalary * root, unsigned key) const
    {
        if (root == nullptr || root->m_salary == key)
            return root;
        if (key < root->m_salary)
            return find_salary(root->lesser, key);
        return find_salary(root->bigger, key);
    }
    CNode * insert_node_name(CNode * root, shared_ptr<CEmployee>& key) // Create a ptr before using
    {
        if (root == nullptr)
        {
            CNode * ret = new CNode(key);
            return ret;
        }
        int res = key->relative_name_pos(*root->Employee);
        // How to correctly check for res == 0???? maybe add some output indicator?
        if (res > 0)
            root->lesser = insert_node_name(root->lesser, key);
        else if (res < 0)
            root->bigger = insert_node_name(root->bigger, key);

        return root;
    }
    CNode * find_successor(CNode * root, CNode * child) const // Create an object to_be_found before using
    {
        // 1. If bigger subtree exists
        if (child->bigger != nullptr)
            return min_node(child->bigger);
        
        CNode * succ = nullptr;
        // 2. Start from root and search down
        while (root->Employee != child->Employee)
        {
            if (child->Employee->relative_name_pos(*root->Employee) > 0)
            {
                succ = root;
                root = root->lesser;
            }
            else 
                root = root->bigger;
        }
        return succ;
    }
    // void sum_until(CSalary * root, unsigned until, unsigned & res) const
    // {
    //     if (root == nullptr) return;
    //     sum_until(root->lesser, until, res);
    //     res += root->m_amount;
    //     if (root->m_salary == until) sum_until(root->bigger, until, res);
    // }
    CNode * insert_node_email(CNode * root, shared_ptr<CEmployee>& key) // Create a ptr before using
    {
        if (root == nullptr)
            return new CNode(key);
        int res = key->relative_email_pos(*root->Employee);
        // How to correctly check for res == 0???? maybe add some output indicator?
        if (res > 0)
            root->lesser = insert_node_email(root->lesser, key);
        else if (res < 0)
            root->bigger = insert_node_email(root->bigger, key);

        return root;
    }
    CSalary * insert_node_salary(CSalary * root, unsigned salary)
    {
        if (root == nullptr)
            return new CSalary(salary);
        if (salary == root->m_salary)
            root->m_amount += 1;
        else if (salary < root->m_salary)
            root->lesser = insert_node_salary(root->lesser, salary);
        else 
            root->bigger = insert_node_salary(root->bigger, salary);

        return root;
    }
    CSalary * remove_node_salary(CSalary * root, unsigned salary)
    {
        if (root == nullptr)
            return root;
        else if (salary < root->m_salary)
            root->lesser = remove_node_salary(root->lesser, salary);
        else if (salary > root->m_salary)
            root->bigger = remove_node_salary(root->bigger, salary);
        else
        {
            if (root->m_amount > 0) root->m_amount -= 1;
            if (root->m_amount < 1)
            {
                if (root->lesser == nullptr && root->bigger == nullptr)
                {
                    delete root;
                    root = nullptr;
                }
                else if (root->lesser == nullptr)
                {
                    CSalary * tmp = root;
                    root = root->bigger;
                    delete tmp;
                }
                else if (root->bigger == nullptr)
                {
                    CSalary * tmp = root;
                    root = root->lesser;
                    delete tmp;
                }
                else
                {
                    CSalary * tmp = min_sal_node(root->bigger);
                    root->m_amount = tmp->m_amount;
                    root->m_salary = tmp->m_salary;
                    tmp->m_amount = 0;
                    root->bigger = remove_node_salary(root->bigger, tmp->m_salary);
                }
            }
        }
        return root;
    }

    void delete_tree(CNode * root)
    {
        if (root == nullptr) return;

        delete_tree(root->lesser);
        delete_tree(root->bigger);

        delete root;
    }
    void delete_tree(CSalary * root)
    {
        if (root == nullptr) return;

        delete_tree(root->lesser);
        delete_tree(root->bigger);

        delete root;
    }
    //Utilities for deletion
    CNode * min_node(CNode * root) const
    {
        CNode * cur = root;
        while (cur != nullptr && cur->lesser != nullptr)
            cur = cur->lesser;
        return cur;
    }
    CSalary * min_sal_node(CSalary * root) const
    {
        CSalary * cur = root;
        while (cur != nullptr && cur->lesser != nullptr)
            cur = cur->lesser;
        return cur;
    }
    //
    CNode * delete_node_name(CNode * root, CEmployee & key) // Try to unify functions
    {
        if (root == nullptr)
            return root;
        else if (key.relative_name_pos(*root->Employee) > 0)
            root->lesser = delete_node_name(root->lesser, key);
        else if (key.relative_name_pos(*root->Employee) < 0)
            root->bigger = delete_node_name(root->bigger, key);
        else
        {
            if (root->lesser == nullptr && root->bigger == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else if (root->lesser == nullptr)
            {
                CNode * tmp = root;
                root = root->bigger;
                delete tmp;
            }
            else if (root->bigger == nullptr)
            {
                CNode * tmp = root;
                root = root->lesser;
                delete tmp;
            }
            else
            {
                CNode * tmp = min_node(root->bigger);
                root->Employee = tmp->Employee;
                root->bigger = delete_node_name(root->bigger, *tmp->Employee);
            }
        }
        return root;
    }
    CNode * delete_node_email(CNode * root, CEmployee & key)
    {
        if (root == nullptr)
            return root;
        else if (key.relative_email_pos(*root->Employee) > 0)
            root->lesser = delete_node_email(root->lesser, key);
        else if (key.relative_email_pos(*root->Employee) < 0)
            root->bigger = delete_node_email(root->bigger, key);
        else
        {
            if (root->lesser == nullptr && root->bigger == nullptr)
            {
                delete root;
                root = nullptr;
            }
            else if (root->lesser == nullptr)
            {
                CNode * tmp = root;
                root = root->bigger;
                delete tmp;
            }
            else if (root->bigger == nullptr)
            {
                CNode * tmp = root;
                root = root->lesser;
                delete tmp;
            }
            else
            {
                CNode * tmp = min_node(root->bigger);
                root->Employee = tmp->Employee;
                root->bigger = delete_node_email(root->bigger, *tmp->Employee);
            }
        }
        return root;
    }

    // Utility for summation
    unsigned sum_sal_nodes(CSalary * root) const
    {
        if (root == nullptr) return 0;
        if (root->lesser != nullptr && root->bigger != nullptr)
            return root->m_amount + sum_sal_nodes(root->lesser) + sum_sal_nodes(root->bigger);
        if (root->lesser != nullptr && root->bigger == nullptr)
            return root->m_amount + sum_sal_nodes(root->lesser);
        if (root->lesser == nullptr && root->bigger != nullptr)
            return root->m_amount + sum_sal_nodes(root->bigger);
        return root->m_amount;
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

  CPersonalAgenda b3;
  b3.add("a", "A", "a1", 1);
  b3.add("a", "B", "a2", 1);
  b3.add("a", "C", "a3", 1);
  b3.add("a", "D", "a4", 1);
  b3.add("a", "E", "a5", 1);
  b3.add("a", "F", "a6", 1);
  b3.add("a", "G", "a7", 1);

  b3.del("a6");

  b3.add("a", "F", "a8", 1);
  b3.add("a", "H", "a9", 1);
  b3.add("a", "J", "a10", 1);
  b3.add("a", "K", "a11", 1);
  b3.del("a8");
  b3.add("a", "F", "a12", 1);
  b3.add("a", "L", "a13", 1);
  b3.add("a", "M", "a14", 1);
  b3.add("a", "N", "a15", 1);
  b3.add("a", "O", "a16", 1);
  b3.add("a", "P", "a17", 1);

  assert (b3.getNext("a", "A", outName, outSurname) && outName == "a" && outSurname == "B");
  assert (b3.getNext("a", "C", outName, outSurname) && outName == "a" && outSurname == "D");
  assert (b3.getNext("a", "D", outName, outSurname) && outName == "a" && outSurname == "E");

  CPersonalAgenda b4;
  b4.add("a", "a", "a1", 500);
  b4.add("a", "b", "a2", 500);
  b4.add("a", "c", "a3", 300);
  b4.add("a", "d", "a4", 300);
  b4.add("a", "e", "a5", 300);
  b4.add("a", "f", "a6", 700);
  b4.add("a", "g", "a7", 700);
  b4.add("a", "h", "a8", 700);
  b4.add("a", "j", "a9", 700);
  b4.add("a", "k", "a10", 200);
  b4.add("a", "l", "a11", 400);
  b4.add("a", "m", "a12", 600);
  b4.add("a", "n", "a13", 800);

  b4.getRank("a12", lo, hi);
  b4.getRank("a7", lo, hi);
  b4.getRank("a13", lo, hi);
  b4.getRank("a3", lo, hi);
  b4.getRank("a11", lo, hi);

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
