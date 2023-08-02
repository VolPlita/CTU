#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <cctype>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;
#endif /* __PROGTEST__ */
#define INIT_SIZE 10

/* ----- Shared pointer ----- */
template <typename T>
class shared_ptr {
public:
    // Default constructor
    shared_ptr() : ptr_(nullptr), ref_count_(nullptr) {}

    // Constructor with raw pointer
    explicit shared_ptr(T* ptr) : ptr_(ptr) {
        try {
            ref_count_ = new unsigned int(1);
        } catch (std::bad_alloc&) {
            delete ptr_;
            throw;
        }
    }

    // Copy constructor
    shared_ptr(const shared_ptr& other) : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        if (ref_count_) {
        	IncrementRefCount();
    	}
    }

    // Move constructor
    shared_ptr(shared_ptr&& other) noexcept : ptr_(other.ptr_), ref_count_(other.ref_count_) {
        other.ptr_ = nullptr;
        other.ref_count_ = nullptr;
    }

    // Copy assignment operator
    shared_ptr& operator=(const shared_ptr& other) {
        if (this != &other) {
            DecrementRefCount();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            IncrementRefCount();
        }
        return *this;
    }

    // Move assignment operator
    shared_ptr& operator=(shared_ptr&& other) noexcept {
        if (this != &other) {
            DecrementRefCount();
            ptr_ = other.ptr_;
            ref_count_ = other.ref_count_;
            other.ptr_ = nullptr;
            other.ref_count_ = nullptr;
        }
        return *this;
    }

    // Destructor
    ~shared_ptr() {
        DecrementRefCount();
    }

    // Dereference operator
    T& operator*() const {
        return *ptr_;
    }

    // Arrow operator
    T* operator->() const {
        return ptr_;
    }

private:
    T* ptr_;
    unsigned int* ref_count_;

    // Helper function to increment reference count
    void IncrementRefCount() const {
        if (ref_count_) {
            ++(*ref_count_);
        }
    }

    // Helper function to decrement reference count and clean up if necessary
    void DecrementRefCount() {
        if (ref_count_) {
            --(*ref_count_);
            if (*ref_count_ <= 0) {
                delete ptr_;
                delete ref_count_;
            }
			ptr_ = nullptr;
        	ref_count_ = nullptr; 
        }
    }
};

/* ----- Vector ----- */
template <typename T> 
class vector {
	T* arr;
	size_t capacity;
	size_t current;
public:
	vector()
	{
		arr = new T[INIT_SIZE];
		capacity = INIT_SIZE;
		current = 0;
	}
	~vector() {delete [] arr;}

	void push(T & data)
	{
		if (current >= capacity) {
			T* temp = new T[capacity * 2];

			for (size_t i = 0; i < capacity; i++)
				temp[i] = arr[i];

			delete[] arr;
			capacity *= 2;
			arr = temp;
		}
		arr[current] = data;
		++current;
	}

    void insert(size_t index, const T& data) {
        if (index > current) 
        {
            throw std::out_of_range("Index out of range");
        }

        if (current >= capacity) 
        {
            // Resize
            T* temp = new T[capacity * 2];
            for (size_t i = 0; i < current; ++i)
                temp[i] = arr[i];

            delete[] arr;
			capacity *= 2;
			arr = temp;
        }

        // Shift elements to the right
        for (size_t i = current; i > index; --i) {
            arr[i] = arr[i - 1];
        }

        arr[index] = data;
        ++current;
    }

    T& operator[](size_t index) 
    {
        if (index >= current) 
        {
            throw std::out_of_range("Index out of range");
        }
        return arr[index];
    }

	int size() { return current; }
};

/* ----- String holder with counted references ----- */
class CStrRef
{
public:
    CStrRef (const char * str = ""); /*Constructor of new instance of string with new data*/
	CStrRef (const CStrRef & x);    /*Constructor of new instance of string with reference to old data*/
 	~CStrRef ();
    CStrRef & operator = (const CStrRef & x);	/*Copying with reference*/ 
    int get_length() const;
	operator const char * () const;
    char operator [] (int idx) const;

    struct TData
    {
		TData (const char * str);
		TData (int Reserve);
        ~TData ();
    	int m_RefCnt;
    	int m_Len;
    	int m_Max;
    	char * m_Str;
    };

    TData * m_Data;

    friend std::ostream & operator << (std::ostream & os, const CStrRef & x);
};

CStrRef operator + (const CStrRef & a, const CStrRef & b);
ostream & operator << (std::ostream & os, const CStrRef & x);
//=================================================================================================
CStrRef::TData::TData (const char * str) :
	m_RefCnt (1),
	m_Len (strlen(str)),
	m_Max (m_Len + 1),
	m_Str (new char[m_Max])
{
	memcpy(m_Str, str, m_Len + 1);
}
//-------------------------------------------------------------------------------------------------
CStrRef::TData::TData (int reserve) :
	m_RefCnt ( 1 ),
	m_Len ( 0 ),
	m_Max ( reserve ),
	m_Str ( new char[m_Max] )

{
	m_Str[0] = 0;
}
//-------------------------------------------------------------------------------------------------
CStrRef::TData::~TData ()
{
	delete [] m_Str;
}
//=================================================================================================
CStrRef::CStrRef (const char * str) :
    m_Data (new TData (str))
{}
//-------------------------------------------------------------------------------------------------
CStrRef::CStrRef (const CStrRef & x) :
    m_Data (x.m_Data)
{
	m_Data->m_RefCnt++;
}
//-------------------------------------------------------------------------------------------------
CStrRef::~CStrRef()
{
	if (--m_Data->m_RefCnt <= 0) 
    	delete m_Data;
}
//-------------------------------------------------------------------------------------------------
CStrRef & CStrRef::operator = (const CStrRef & x)
{
	if (&x != this)
	{
    	if (--m_Data->m_RefCnt == 0)
      		delete m_Data;
    	m_Data = x.m_Data;
    	m_Data->m_RefCnt++;
	}
	return *this;
}
//-------------------------------------------------------------------------------------------------
int CStrRef::get_length() const
{
	return m_Data->m_Len;
}
//-------------------------------------------------------------------------------------------------
CStrRef::operator const char * () const
{
  return m_Data -> m_Str;
}
//-------------------------------------------------------------------------------------------------
char CStrRef::operator [] (int idx) const
{
  if ( idx < 0 || idx >= m_Data -> m_Len )
    throw "index out of range";

  return m_Data -> m_Str[idx];
}
//-------------------------------------------------------------------------------------------------
ostream & operator << (ostream & os, const CStrRef & x)
{
  os << x.m_Data->m_Str;
  return os;
}
//=======================================================================================================================================================================

class CMail
{
public:
    CMail (const char * from, const char * to, const char * body) :
        m_from(from) , m_to(to), m_body(body)
    {}
    bool operator == ( const CMail & x ) const
    {
        return !strncmp(m_from.m_Data->m_Str, x.m_from.m_Data->m_Str, m_from.get_length()) && 
               !strncmp(m_to.m_Data->m_Str, x.m_to.m_Data->m_Str, m_to.get_length()) && 
               !strncmp(m_body.m_Data->m_Str, x.m_body.m_Data->m_Str, m_body.get_length());
    }
    friend ostream & operator << ( ostream & os, const CMail & m );

    const CStrRef & get_from() const {return m_from;}
    const CStrRef & get_to() const {return m_to;}
    const CStrRef & get_body() const {return m_body;}
private:
    CStrRef m_from;
    CStrRef m_to;
    CStrRef m_body;
};
ostream & operator << ( ostream & os, const CMail & m )
{
    return os << m.m_from << " " << m.m_to << " " << m.m_body << endl;
}

struct CAccount
{
	CStrRef m_email;
	vector<shared_ptr<CMail>> m_inbox;
	vector<shared_ptr<CMail>> m_outbox;
	CAccount(){}
	CAccount(const CStrRef & addr) : m_email(addr)
	{}
	~CAccount(){}
};

class CMailIterator 
{
public:
	CMailIterator();
	CMailIterator(vector<CMail> & vec);
    explicit operator bool ( void ) const;
    bool operator ! ( void ) const;
    const CMail & operator * ( void ) const;
    CMailIterator & operator ++ ( void );
private:
    vector<CMail> & vec;
	size_t pos;
};


class CMailServer 
{
public:
    CMailServer ( void ) {};
    CMailServer ( const CMailServer & src );
    CMailServer & operator = ( const CMailServer & src );
    ~CMailServer ( void ) {};	// Maybe need to implement
    void sendMail ( const CMail & m );
    CMailIterator outbox ( const char * email ) const;
    CMailIterator inbox ( const char * email ) const;
private:
	vector<CAccount> m_accounts;

	int bsearch(CStrRef key, int start_pos, int end_pos) // This function returns a position in an array, -1 if not found
	{
		while (start_pos < end_pos)
		{
			int middle = ((start_pos + end_pos) / 2);
			int res = strcmp(key.m_Data->m_Str, m_accounts[middle].m_email.m_Data->m_Str);
			if (res == 0) return middle;
			else if (res > 0) start_pos = middle + 1;	
			else end_pos = middle - 1;
		}
		// if (strncmp(key, m_accounts[start_pos].m_email, key.get_length())) return start_pos;
		// return -1;
		return start_pos;
	}
};
void CMailServer::sendMail(const CMail & m)
{
	shared_ptr<CMail> newptr(new CMail(m));

	int pos_sender = bsearch(m.get_from(), 0, m_accounts.size() - 1);
	int cmp;
	if (m_accounts.size() == 0 || ((cmp = strncmp(m.get_from().m_Data->m_Str, m_accounts[pos_sender].m_email.m_Data->m_Str, m.get_from().get_length())) != 0))
	{
		if (m_accounts.size() > 0 && cmp > 0) 
		{
			m_accounts.insert(pos_sender + 1, CAccount(m.get_from()));
			m_accounts[pos_sender + 1].m_outbox.push(newptr);
		}
		else 
		{
			m_accounts.insert(pos_sender, CAccount(m.get_from()));
			m_accounts[pos_sender].m_outbox.push(newptr);
		}
	}
	else
	{
		m_accounts[pos_sender].m_outbox.push(newptr);
	}

	int pos_receiver = bsearch(m.get_to(), 0, m_accounts.size() - 1);
	if (m_accounts.size() == 0 || ((cmp = strncmp(m.get_to().m_Data->m_Str, m_accounts[pos_receiver].m_email.m_Data->m_Str, m.get_to().get_length())) != 0))
	{
		if (m_accounts.size() > 0 && cmp > 0) 
		{
			m_accounts.insert(pos_receiver + 1, CAccount(m.get_to()));
			m_accounts[pos_receiver + 1].m_inbox.push(newptr);
		}
		else 
		{
			m_accounts.insert(pos_receiver, CAccount(m.get_to()));
			m_accounts[pos_receiver].m_inbox.push(newptr);
		}
	}
	else
	{
		m_accounts[pos_receiver].m_inbox.push(newptr);
	}
}

#ifndef __PROGTEST__
bool matchOutput (const CMail & m, const char * str)
{
  ostringstream oss;
  oss << m;
  return oss . str () == str;
}

int main ( void )
{
  char from[100], to[100], body[1024];

  assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "progtest deadline", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "john", "progtest deadline" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "peter", "progtest deadline", "john" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "john", "peter" ) ) );
  assert ( !( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "progtest deadline", "peter", "john" ) ) );
  CMailServer s0;
  s0 . sendMail ( CMail ( "john", "peter", "some important mail" ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "thomas", sizeof ( to ) );
  strncpy ( body, "another important mail", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  strncpy ( from, "john", sizeof ( from ) );
  strncpy ( to, "alice", sizeof ( to ) );
  strncpy ( body, "deadline notice", sizeof ( body ) );
  s0 . sendMail ( CMail ( from, to, body ) );
  s0 . sendMail ( CMail ( "alice", "john", "deadline confirmation" ) );
  s0 . sendMail ( CMail ( "peter", "alice", "PR bullshit" ) );
  // CMailIterator i0 = s0 . inbox ( "alice" );
  // assert ( i0 && *i0 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i0,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i0 && *i0 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i0,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ! ++i0 );

  // CMailIterator i1 = s0 . inbox ( "john" );
  // assert ( i1 && *i1 == CMail ( "alice", "john", "deadline confirmation" ) );
  // assert ( matchOutput ( *i1,  "From: alice, To: john, Body: deadline confirmation" ) );
  // assert ( ! ++i1 );

  // CMailIterator i2 = s0 . outbox ( "john" );
  // assert ( i2 && *i2 == CMail ( "john", "peter", "some important mail" ) );
  // assert ( matchOutput ( *i2,  "From: john, To: peter, Body: some important mail" ) );
  // assert ( ++i2 && *i2 == CMail ( "john", "thomas", "another important mail" ) );
  // assert ( matchOutput ( *i2,  "From: john, To: thomas, Body: another important mail" ) );
  // assert ( ++i2 && *i2 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i2,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ! ++i2 );

  // CMailIterator i3 = s0 . outbox ( "thomas" );
  // assert ( ! i3 );

  // CMailIterator i4 = s0 . outbox ( "steve" );
  // assert ( ! i4 );

  // CMailIterator i5 = s0 . outbox ( "thomas" );
  // s0 . sendMail ( CMail ( "thomas", "boss", "daily report" ) );
  // assert ( ! i5 );

  // CMailIterator i6 = s0 . outbox ( "thomas" );
  // assert ( i6 && *i6 == CMail ( "thomas", "boss", "daily report" ) );
  // assert ( matchOutput ( *i6,  "From: thomas, To: boss, Body: daily report" ) );
  // assert ( ! ++i6 );

  // CMailIterator i7 = s0 . inbox ( "alice" );
  // s0 . sendMail ( CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( i7 && *i7 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i7,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i7 && *i7 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i7,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ! ++i7 );

  // CMailIterator i8 = s0 . inbox ( "alice" );
  // assert ( i8 && *i8 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i8,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i8 && *i8 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i8,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ++i8 && *i8 == CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( matchOutput ( *i8,  "From: thomas, To: alice, Body: meeting details" ) );
  // assert ( ! ++i8 );

  // CMailServer s1 ( s0 );
  // s0 . sendMail ( CMail ( "joe", "alice", "delivery details" ) );
  // s1 . sendMail ( CMail ( "sam", "alice", "order confirmation" ) );
  // CMailIterator i9 = s0 . inbox ( "alice" );
  // assert ( i9 && *i9 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i9,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i9 && *i9 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i9,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ++i9 && *i9 == CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( matchOutput ( *i9,  "From: thomas, To: alice, Body: meeting details" ) );
  // assert ( ++i9 && *i9 == CMail ( "joe", "alice", "delivery details" ) );
  // assert ( matchOutput ( *i9,  "From: joe, To: alice, Body: delivery details" ) );
  // assert ( ! ++i9 );

  // CMailIterator i10 = s1 . inbox ( "alice" );
  // assert ( i10 && *i10 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i10,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i10 && *i10 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i10,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ++i10 && *i10 == CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( matchOutput ( *i10,  "From: thomas, To: alice, Body: meeting details" ) );
  // assert ( ++i10 && *i10 == CMail ( "sam", "alice", "order confirmation" ) );
  // assert ( matchOutput ( *i10,  "From: sam, To: alice, Body: order confirmation" ) );
  // assert ( ! ++i10 );

  // CMailServer s2;
  // s2 . sendMail ( CMail ( "alice", "alice", "mailbox test" ) );
  // CMailIterator i11 = s2 . inbox ( "alice" );
  // assert ( i11 && *i11 == CMail ( "alice", "alice", "mailbox test" ) );
  // assert ( matchOutput ( *i11,  "From: alice, To: alice, Body: mailbox test" ) );
  // assert ( ! ++i11 );

  // s2 = s0;
  // s0 . sendMail ( CMail ( "steve", "alice", "newsletter" ) );
  // s2 . sendMail ( CMail ( "paul", "alice", "invalid invoice" ) );
  // CMailIterator i12 = s0 . inbox ( "alice" );
  // assert ( i12 && *i12 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i12,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i12 && *i12 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i12,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ++i12 && *i12 == CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( matchOutput ( *i12,  "From: thomas, To: alice, Body: meeting details" ) );
  // assert ( ++i12 && *i12 == CMail ( "joe", "alice", "delivery details" ) );
  // assert ( matchOutput ( *i12,  "From: joe, To: alice, Body: delivery details" ) );
  // assert ( ++i12 && *i12 == CMail ( "steve", "alice", "newsletter" ) );
  // assert ( matchOutput ( *i12,  "From: steve, To: alice, Body: newsletter" ) );
  // assert ( ! ++i12 );

  // CMailIterator i13 = s2 . inbox ( "alice" );
  // assert ( i13 && *i13 == CMail ( "john", "alice", "deadline notice" ) );
  // assert ( matchOutput ( *i13,  "From: john, To: alice, Body: deadline notice" ) );
  // assert ( ++i13 && *i13 == CMail ( "peter", "alice", "PR bullshit" ) );
  // assert ( matchOutput ( *i13,  "From: peter, To: alice, Body: PR bullshit" ) );
  // assert ( ++i13 && *i13 == CMail ( "thomas", "alice", "meeting details" ) );
  // assert ( matchOutput ( *i13,  "From: thomas, To: alice, Body: meeting details" ) );
  // assert ( ++i13 && *i13 == CMail ( "joe", "alice", "delivery details" ) );
  // assert ( matchOutput ( *i13,  "From: joe, To: alice, Body: delivery details" ) );
  // assert ( ++i13 && *i13 == CMail ( "paul", "alice", "invalid invoice" ) );
  // assert ( matchOutput ( *i13,  "From: paul, To: alice, Body: invalid invoice" ) );
  // assert ( ! ++i13 );

  // return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
