#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <iterator>
#include <functional>
using namespace std;

class CDate
{
public:
    //---------------------------------------------------------------------------------------------
    CDate (int y, int m, int d) : 
        m_Year ( y ),
        m_Month ( m ),
        m_Day ( d )
    {}
    //---------------------------------------------------------------------------------------------
    int compare (const CDate & x) const
    {
		if ( m_Year != x . m_Year )
			return m_Year - x . m_Year;
      	if ( m_Month != x . m_Month )
        	return m_Month - x . m_Month;
      	return m_Day - x . m_Day;
    }
    int year ( void ) const {return m_Year;}
    int month ( void ) const {return m_Month;}
    int day ( void ) const {return m_Day;}
    //---------------------------------------------------------------------------------------------
    friend ostream & operator << (ostream & os, const CDate & x)
    {
      char oldFill = os.fill();
      return os << setfill ( '0' ) << setw ( 4 ) << x . m_Year << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Month ) << "-" 
                                   << setw ( 2 ) << static_cast<int> ( x . m_Day ) 
                << setfill ( oldFill );
    }
    //---------------------------------------------------------------------------------------------
private:
    int16_t m_Year;
    int8_t m_Month;
    int8_t m_Day;
};
#endif /* __PROGTEST__ */

unsigned regorder = 0;

string to_normal(string oldstr)// Don't rely on
{
	string str = oldstr; // Do I need it??
    if (str.empty()) return str;
    // remove spaces from beginning of string
    while (str[0] == ' ') str.erase(0, 1);
    // remove spaces from end of string
    while (str[str.size() - 1] == ' ') str.erase(str.size() - 1, 1);
    // squeeze consecutive spaces into one
    str[0] = tolower(str[0]);
    for (size_t i = 1; i < str.size(); i++) 
    {
		str[i] = tolower(str[i]);
        if (str[i] == ' ' && str[i - 1] == ' ') 
        {
            str.erase(i, 1);
            i--;
        }
    }
	return str;
}

class CInvoice
{
public:
	CInvoice (const CDate & date, const string & seller, const string & buyer, unsigned int amount, double vat) : 
		m_date(date), 
		m_buyer(buyer),  
		m_seller(seller),
		m_norm_buyer(to_normal(buyer)), 
		m_norm_seller(to_normal(seller)), 
		m_amount(amount), 
		m_vat(vat), 
		m_issued(false), 
		m_accepted(false),
		m_regorder(0)
		{}
    CDate date( void ) const {return m_date;}
    string buyer( void ) const {return m_buyer;}
    string seller( void ) const {return m_seller;}
	string norm_buyer( void ) const {return m_norm_buyer;}
    string norm_seller( void ) const {return m_norm_seller;}
    unsigned int amount( void ) const {return m_amount;}
    double vat( void ) const {return m_vat;}
	void set_issued(bool x) {m_issued = x;}
	void set_accepted(bool x) {m_accepted = x;}
	void set_regorder(int x) {m_regorder = x;}
	void set_buyer(string x) {m_buyer = x;}
	void set_seller(string x) {m_seller = x;}
	bool issued() const {return m_issued;}
	bool accepted() const {return m_accepted;}
	unsigned regorder() const {return m_regorder;}
  private:
    CDate m_date;
	string m_buyer;
	string m_seller;
	string m_norm_buyer;
	string m_norm_seller;
	unsigned int m_amount;
	double m_vat;
	bool m_issued;
	bool m_accepted;
	unsigned m_regorder;
};

typedef int (*CompareInvoice)(const CInvoice&, const CInvoice&, bool);

class CSortComparator
{
public:
	static CompareInvoice compset[5]; // Set of functions for comparison
	static int compNum(unsigned a, unsigned b) {return (b < a) - (a < b);}
	static int compNum(double a, double b) {return (b < a) - (a < b);}
	static int compString(const string & a, const string & b)
	{
		int res = strcasecmp(a.c_str(), b.c_str()); //strncasecmp??
		if (res == 0 && a.size() != b.size()) 
		{
			if (a.size() > b.size()) return 1;
			else return -1;
		}
		return res;
	}
	static int by_date(const CInvoice & inv1, const CInvoice & inv2, bool ascending)
	{
		if (ascending) return inv1.date().compare(inv2.date());
		else return inv2.date().compare(inv1.date());
	}
	static int by_buyer(const CInvoice & inv1, const CInvoice & inv2, bool ascending)
	{
		if (ascending) return compString(inv1.buyer(), inv2.buyer());
		else return compString(inv2.buyer(), inv1.buyer());
	}
	static int by_seller(const CInvoice & inv1, const CInvoice & inv2, bool ascending)
	{
		if (ascending) return compString(inv1.seller(), inv2.seller());
		else return compString(inv2.seller(), inv1.seller());
	}
	static int by_amount(const CInvoice & inv1, const CInvoice & inv2, bool ascending)
	{
		if (ascending) return compNum(inv1.amount(), inv2.amount());
		else return compNum(inv2.amount(), inv1.amount());
	}
	static int by_vat(const CInvoice & inv1, const CInvoice & inv2, bool ascending)
	{
		if (ascending) return compNum(inv1.vat(), inv2.vat());
		else return compNum(inv2.vat(), inv1.vat());
	}
};

CompareInvoice CSortComparator::compset[5] = {&by_date, &by_buyer, &by_seller, &by_amount, &by_vat};

class CSortOpt
{
public:
    static const int BY_DATE = 0;
    static const int BY_BUYER = 1;
    static const int BY_SELLER = 2;
    static const int BY_AMOUNT = 3;
    static const int BY_VAT = 4;
	CSortOpt ( void ) = default;
    CSortOpt & addKey (int key, bool ascending = true )
	{
		opts.emplace_back(key, ascending);
		return *this;
	}
	bool operator () (const CInvoice & a, const CInvoice & b) const
	{
		for (auto option : opts)
		{
			int res = CSortComparator::compset[option.first](a, b, option.second);
			if (res != 0) return res < 0;
		}
		return a.regorder() < b.regorder();
	}
private:
	vector<pair<int, bool>> opts;
};

/* ----- Hash functions ----- */
size_t hash_string(const string & str)
{
	size_t hash = 5381;
	for (char c : str) 
		hash = ((hash << 5) - hash) + static_cast<size_t>(c);
	return hash;
}
size_t hash_seller(const string & str)
{
	size_t hash = 5387;
	for (char c : str) 
		hash = ((hash << 5) + hash) + static_cast<size_t>(c);
	return hash;
}
size_t hash_buyer(const string & str)
{
	size_t hash = 5393;
	for (char c : str) 
		hash = ((hash << 5) + hash) + static_cast<size_t>(c);
	return hash;
}
size_t hash_num(const unsigned int n)
{
	return hash_string(to_string(n));
}
size_t hash_num(const double n)
{
	return hash_string(to_string(n));
}
size_t hash_date(const CDate date)
{
	return hash_string(to_string(date.year())) ^ 
		   hash_string(to_string(date.month())) ^ 
		   hash_string(to_string(date.day()));
}
/* ----- Structs for set/map ----- */
struct ComparCompName
{
	bool operator () (const string & str1, const string & str2) const 
	{
		if (strcmp(to_normal(str1).c_str(), to_normal(str2).c_str()) == 0) return true;
		return false;
	}
};
struct StringHash
{
	size_t operator () (const string & str) const 
	{
		return hash_string(to_normal(str));
	}
};
struct ComparInvoice
{
	bool operator () (const shared_ptr<CInvoice> & inv1, const shared_ptr<CInvoice> & inv2) const
	{
		if (inv1 == nullptr || inv2 == nullptr) return false;
		return inv1->date().compare(inv2->date()) == 0 &&
			   inv1->norm_seller() == inv2->norm_seller() &&
			   inv1->norm_buyer() == inv2->norm_buyer() &&
			   inv1->amount() == inv2->amount() &&
			   inv1->vat() == inv2->vat();
	}
};
struct InvoiceHash
{
	size_t operator () (const shared_ptr<CInvoice> & inv) const
	{
		return hash_buyer(inv->norm_buyer()) ^ hash_seller(inv->norm_seller()) ^
			   hash_num(inv->amount()) ^ hash_num(inv->vat()) ^ hash_date(inv->date());
	}
};

struct CCompInvoiceList
{
	unordered_set<shared_ptr<CInvoice>, InvoiceHash, ComparInvoice> m_data;
};
/* ----- Initial interface -----*/
class CVATRegister
{
public:
	CVATRegister ( void ) = default;
	~CVATRegister ( void )
	{
		for (auto & it : m_register)
		{
			delete (it.second);
		}
	}
    bool registerCompany( const string & name )
	{
		if (m_register.find(name) != m_register.end()) return false;
		m_register[name] = new CCompInvoiceList;
		return true;
	}
    bool addIssued( const CInvoice & x )
	{
		auto buy = m_register.find(x.buyer());
		auto sell = m_register.find(x.seller());
		if (x.norm_buyer() == x.norm_seller() || buy == m_register.end() || sell == m_register.end()) return false;

		auto ptr_x = make_shared<CInvoice>(x);
		ptr_x->set_buyer(buy->first);
		ptr_x->set_seller(sell->first);

		auto found_inv_buy = m_register[ptr_x->buyer()]->m_data.find(ptr_x);
		if (found_inv_buy == buy->second->m_data.end())
		{
			ptr_x->set_issued(true);
			ptr_x->set_regorder(regorder);
			regorder++;
			buy->second->m_data.emplace(ptr_x);
			sell->second->m_data.emplace(ptr_x);
		}
		else
		{
			if ((*found_inv_buy)->issued() == false) (*found_inv_buy)->set_issued(true);
			else return false; 
		}
		return true;
	}
    bool addAccepted( const CInvoice & x )
	{
		auto buy = m_register.find(x.buyer());
		auto sell = m_register.find(x.seller());
		if (x.norm_buyer() == x.norm_seller() || buy == m_register.end() || sell == m_register.end()) return false;

		auto ptr_x = make_shared<CInvoice>(x);
		ptr_x->set_buyer(buy->first);
		ptr_x->set_seller(sell->first);

		auto found_inv_buy = m_register[ptr_x->buyer()]->m_data.find(ptr_x);
		if (found_inv_buy == buy->second->m_data.end())
		{
			ptr_x->set_accepted(true);
			ptr_x->set_regorder(regorder);
			regorder++;
			m_register[ptr_x->buyer()]->m_data.emplace(ptr_x); // Better this way
			m_register[ptr_x->seller()]->m_data.emplace(ptr_x);
		}
		else
		{
			if ((*found_inv_buy)->accepted() == false) (*found_inv_buy)->set_accepted(true);
			else return false; 
		}
		return true;
	}
    bool delIssued( const CInvoice & x )
	{
		// Find the wanted InvoiceLists of buyer and seller
		auto buy = m_register.find(x.buyer());
		auto sell = m_register.find(x.seller());
		// Error if seller and buyer are same or either of them is not registered
		if (x.norm_buyer() == x.norm_seller() || buy == m_register.end() || sell == m_register.end()) return false;
		
		// Create ptr just search for invoice if it exists in InvoiceLists
		auto ptr_x = make_shared<CInvoice>(x);
		auto found_inv_buy = m_register[x.buyer()]->m_data.find(ptr_x);
		auto found_inv_sell = m_register[x.seller()]->m_data.find(ptr_x);
		// If invoice doesn't exist in InvoiceList, return error
		if (found_inv_buy == buy->second->m_data.end()) return false;
		if (found_inv_sell == sell->second->m_data.end()) return false;
		else // Set issued to false if it is true, if it if false, return error
		{
			if ((*found_inv_buy)->issued() == true) (*found_inv_buy)->set_issued(false);
			else return false; 
		}
		// If both issued and accepted are false, delete invoice (both from seller and from the buyer)
		if ((*found_inv_buy)->issued() == false && (*found_inv_buy)->accepted() == false)
		{
			buy->second->m_data.erase(found_inv_buy);
			sell->second->m_data.erase(found_inv_sell);
		}
		return true;
	}
    bool delAccepted( const CInvoice & x )
	{
		// Find the wanted InvoiceLists of buyer and seller
		auto buy = m_register.find(x.buyer());
		auto sell = m_register.find(x.seller());
		// Error if seller and buyer are same or either of them is not registered
		if (x.norm_buyer() == x.norm_seller() || buy == m_register.end() || sell == m_register.end()) return false;
		
		// Create ptr just search for invoice if it exists in InvoiceLists
		auto ptr_x = make_shared<CInvoice>(x);
		auto found_inv_buy = m_register[x.buyer()]->m_data.find(ptr_x);
		auto found_inv_sell = m_register[x.seller()]->m_data.find(ptr_x);
		// If invoice doesn't exist in InvoiceList, return error
		if (found_inv_buy == buy->second->m_data.end()) return false;
		if (found_inv_sell == sell->second->m_data.end()) return false;
		else // Set accepted to false if it is true, if it if false, return error
		{
			if ((*found_inv_buy)->accepted() == true) (*found_inv_buy)->set_accepted(false);
			else return false; 
		}
		// If both issued and accepted are false, delete invoice (both from seller and from the buyer)
		if ((*found_inv_buy)->issued() == false && (*found_inv_buy)->accepted() == false)
		{
			buy->second->m_data.erase(found_inv_buy);
			sell->second->m_data.erase(found_inv_sell);
		}
		return true;
	}
    list<CInvoice> unmatched( const string & company, const CSortOpt & sortBy ) const
	{
		list<CInvoice> res;
		auto comp = m_register.find(company);
		if (comp == m_register.end()) return res;
		for (const auto & it : comp->second->m_data)
		{
			if (it->accepted() != it->issued())
				res.push_back(*it);
		}
		res.sort(sortBy);
		return res;
	}
private:
	unordered_map< string, CCompInvoiceList* , StringHash , ComparCompName > m_register;
};
	
#ifndef __PROGTEST__
bool equalLists ( const list<CInvoice> & a, const list<CInvoice> & b )
{
	if (a.size() != b.size()) return false;
	auto it1 = a.begin();
	auto it2 = b.begin();
	while (it1 != a.end() || it2 != b.end())
	{
		if ( it1->date().compare(it2->date()) != 0 ||
			 it1->seller() != it2->seller() ||
			 it1->buyer() != it2->buyer() ||
			 it1->amount() != it2->amount() ||
			 it1->vat() != it2->vat() )
			 return false;
		++it1;
		++it2;
	}
	return true;
}

int main ( void )
{
  CVATRegister r;
  assert ( r . registerCompany ( "first Company" ) );
  assert ( r . registerCompany ( "Second     Company" ) );
  assert ( !r . registerCompany ( "     second company        " ) );
  assert ( r . registerCompany ( "ThirdCompany, Ltd." ) );
  assert ( r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( "tHIRd CompANy,            Ltd." ) );
  assert ( !r . registerCompany ( "Third Company, Ltd." ) );
  assert ( !r . registerCompany ( " Third  Company,  Ltd.  " ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 20 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "FirSt Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 100, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Second Company ", "First Company", 300, 30 ) ) );
  assert ( r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "Third  Company,  Ltd.", "  Second    COMPANY ", 400, 34 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company ", 300, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "First Company", "First   Company", 200, 30 ) ) );
  assert ( !r . addIssued ( CInvoice ( CDate ( 2000, 1, 4 ), "Another Company", "First   Company", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, false ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),//
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, true ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "second company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Third Company, Ltd.", "Second     Company", 400, 34.000000 )
           } ) );
  assert ( equalLists ( r . unmatched ( "last company", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 2 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( r . addAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "Second company ", "First Company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2001, 1, 1 ), "First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "A First Company", "Second Company ", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Hand", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 1200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", "Second Company", 200, 130 ) ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "first          Company", "Second      company", 200, 30 ) ) );
  assert ( !r . delIssued ( CInvoice ( CDate ( 2000, 1, 2 ), "SecondCompany", "First company", 300, 32 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delAccepted ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Third Company, Ltd.", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
  assert ( r . delIssued ( CInvoice ( CDate ( 2000, 1, 1 ), "First Company", " Third  Company,  Ltd.   ", 200, 30 ) ) );
  assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) . addKey ( CSortOpt::BY_DATE, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );

	// My tests

	assert ( r.addAccepted(CInvoice ( CDate ( 2000, 1, 1 ), "first company", "Second Company", 300, 30.000000 )));
	assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey ( CSortOpt::BY_DATE, false ) . addKey ( CSortOpt::BY_SELLER, true ) . addKey ( CSortOpt::BY_BUYER, true ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
	assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey( CSortOpt::BY_AMOUNT, true ) . addKey( CSortOpt::BY_VAT, true ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );
	assert ( r. addAccepted( CInvoice ( CDate(2000, 1, 3), "first Company", "SecoNd Company", 200, 30.0 ) ) );
	assert ( equalLists ( r . unmatched ( "First Company", CSortOpt () . addKey( CSortOpt::BY_AMOUNT, true ) . addKey ( CSortOpt::BY_DATE, false ) ),
           list<CInvoice>
           {
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 20.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "first Company", "Second     Company", 100, 30.000000 ),
			 CInvoice ( CDate ( 2000, 1, 3 ), "first Company", "Second     Company", 200, 30 ),
             CInvoice ( CDate ( 2000, 1, 2 ), "first Company", "Second     Company", 200, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 30.000000 ),
             CInvoice ( CDate ( 2000, 1, 1 ), "Second     Company", "first Company", 300, 32.000000 )
           } ) );

	assert ( r . registerCompany ( "a" ) );
	assert ( r . registerCompany ( "b" ) );
	assert ( r . registerCompany ( "c" ) );
	assert ( r . registerCompany ( "d" ) );
	assert ( r . registerCompany ( "e" ) );
	assert ( !r . registerCompany ( "C" ) );
	assert ( !r . registerCompany ( "D" ) );
	assert ( r . addIssued( CInvoice(CDate ( 2000, 1, 1 ), "a", "b", 100, 10.000000)) );
	assert ( r . addAccepted( CInvoice(CDate ( 2000, 1, 1 ), "a", "b", 100, 10.000000)) );
	assert ( equalLists ( r . unmatched ( "a", CSortOpt () . addKey ( CSortOpt::BY_VAT, true ) ),
           list<CInvoice>
           {
           } ) );
	assert ( r . delIssued( CInvoice(CDate ( 2000, 1, 1 ), "a", "b", 100, 10.000000)) );
	assert ( r . delAccepted( CInvoice(CDate ( 2000, 1, 1 ), "a", "b", 100, 10.000000)) );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
