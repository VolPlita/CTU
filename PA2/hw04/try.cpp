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

// String holder
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
	if (--m_Data->m_RefCnt == 0) 
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

class CMailIterator 
{
public:
    explicit operator bool ( void ) const;
    bool operator ! ( void ) const;
    const CMail & operator * ( void ) const;
    CMailIterator & operator ++ ( void );
private:
    // todo
};


class CMailServer 
{
public:
    CMailServer ( void );
    CMailServer ( const CMailServer & src );
    CMailServer & operator = ( const CMailServer & src );
    ~CMailServer ( void );
    void sendMail ( const CMail & m );
    CMailIterator outbox ( const char * email ) const;
    CMailIterator inbox ( const char * email ) const;
private:
    // todo
};

#ifndef __PROGTEST__
// bool                         matchOutput                   ( const CMail     & m,
//                                                              const char      * str )
// {
//   ostringstream oss;
//   oss << m;
//   return oss . str () == str;
// }

int main ( void )
{
  char from[100], to[100], body[1024];
  CMail new_mail( "john", "peter", "progtest deadline" );
  cout << new_mail;
  CMail anothermail( "john", "peter", "progtest deadline" );
  cout << (new_mail == anothermail) << endl;
  cout << endl;
  cout << (const char *)new_mail.get_from() << endl;
  cout << *new_mail.get_from() << endl;
  cout << (const char *)anothermail.get_from() << endl;
  cout << *anothermail.get_from() << endl;
  //assert ( CMail ( "john", "peter", "progtest deadline" ) == CMail ( "john", "peter", "progtest deadline" ) );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
