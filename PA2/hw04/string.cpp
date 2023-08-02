#include <cstring>
#include <cstdio>
#include <iostream>
using namespace std;

/* String implementation, counted references. The strings use shared holder objects (TData).
 * These shared data are copied only when needed - i.e. if the contents is to be modified or
 * if the last CStrRef releases the string (copy-on-write).
 */

class CStrRef
{
public:
    CStrRef (const char * str = ""); /*Constructor of new instance of string with new data*/
    explicit CStrRef (double x);								// ??
    explicit CStrRef (int x);									// ?? 
 	CStrRef (const CStrRef & x);    /*Constructor of new instance of string with reference to old data*/
 	~CStrRef ();
    CStrRef & operator = (const CStrRef & x);	/*Copying with reference*/ 
    int get_length() const;
    CStrRef & operator += (const CStrRef & x);				// NOT NEEDED 
    CStrRef & operator += (const char * x);					// NOT NEEDED 
    CStrRef & operator += (int x);								// NOT NEEDED 
    CStrRef & operator += (double x);							// NOT NEEDED 
	operator const char * () const;
    char operator [] (int idx) const;
//    void                     SetCharAt           ( int idx, char X );

private:
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

    void append ( const char * src, int srclen );		// NOT NEEDED
    void detach ( int newMaxSize );						// NOT NEEDED

    friend CStrRef operator + (const CStrRef & a, const CStrRef & b);
    friend std::ostream & operator << (std::ostream & os, const CStrRef & x);
};

CStrRef operator + (const CStrRef & a, const CStrRef & b);
std::ostream & operator << (std::ostream & os, const CStrRef & x);
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
CStrRef::CStrRef (double x) :
    m_Data (new TData (20))
{
	operator += (x);
}
//-------------------------------------------------------------------------------------------------
CStrRef::CStrRef (int x) : 
	m_Data (new TData(20))
{
	operator += ( x );
}
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
CStrRef & CStrRef::operator += (const CStrRef & x)
{
	append (x.m_Data->m_Str, x.m_Data->m_Len);
	return *this;
}
//-------------------------------------------------------------------------------------------------
CStrRef         & CStrRef::operator +=         ( const char * x )
{
  append ( x, strlen ( x ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrRef         & CStrRef::operator +=         ( int x )
{
  char tmp [20];

  snprintf ( tmp, sizeof ( tmp ), "%d", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
CStrRef         & CStrRef::operator +=         ( double x )
{
  char tmp [20];

  snprintf ( tmp, sizeof ( tmp ), "%f", x );
  append ( tmp, strlen ( tmp ) );
  return *this;
}
//-------------------------------------------------------------------------------------------------
                   CStrRef::operator const char * ( void ) const
{
  return m_Data -> m_Str;
}
//-------------------------------------------------------------------------------------------------
char               CStrRef::operator []         ( int idx ) const
{
  if ( idx < 0 || idx >= m_Data -> m_Len )
    throw "index out of range";

  return m_Data -> m_Str[idx];
}
//-------------------------------------------------------------------------------------------------
// void               CStrRef::SetCharAt           ( int idx, char x )
// {
//   if ( idx < 0 || idx >= m_Data -> m_Len )
//     throw "index out of range";

//   detach ( m_Data -> m_Max );
//   m_Data -> m_Str[idx] = x;
// }
//-------------------------------------------------------------------------------------------------
void               CStrRef::append              ( const char * src, int srclen )
{
  detach ( m_Data -> m_Len + srclen + 1 );

  if ( m_Data -> m_Len + srclen + 1 > m_Data -> m_Max )
  {
    m_Data -> m_Max = m_Data -> m_Len + srclen + 1;
    char * tmp = new char [m_Data -> m_Max];
    memcpy ( tmp, m_Data -> m_Str, m_Data -> m_Len );
    memcpy ( tmp + m_Data -> m_Len, src, srclen );
    delete [] m_Data -> m_Str;  // safe even if m_Data == src
    m_Data -> m_Str = tmp;
  }
  else
    memcpy ( m_Data -> m_Str + m_Data -> m_Len, src, srclen );
  m_Data -> m_Len += srclen;
  m_Data -> m_Str [ m_Data -> m_Len ] = 0;
}
//-------------------------------------------------------------------------------------------------
void               CStrRef::detach              ( int newMaxSize )
{
  if ( m_Data -> m_RefCnt > 1 )
  { // data block is shared among more than one instance of CStrRef. create own block
    // that can be modified without affecting the other CStrRef instances.

    TData * tmpD = new TData ( newMaxSize );
    memcpy ( tmpD -> m_Str, m_Data -> m_Str, m_Data -> m_Len + 1 ); // + terminating zero
       // tmpD is a separate data block for use in our instance. disconnect from the source

    m_Data -> m_RefCnt --;
    m_Data = tmpD;
  }
}
//-------------------------------------------------------------------------------------------------
CStrRef            operator +                   ( const CStrRef & a, const CStrRef & b )
{
  CStrRef res ( a );
  res += b;
  return res;
}
//-------------------------------------------------------------------------------------------------
std::ostream     & operator <<                   ( std::ostream & os, const CStrRef & x )
{
  os << x . m_Data -> m_Str;
  return os;
}
//=================================================================================================
int                main                          ( int argc, char * argv [] )
{
  CStrRef      a ( 5 ), b = "Long test string";
  int       i;

  a += " test ";

  CStrRef      c ( a );                       // copy constructor

  a += 12.5;                               // op +=
  a += b;

  cout << "a = " << a << endl <<
          "b = " << b << endl <<
          "c = " << c << endl;

  for ( i = a . get_length  () - 1; i >= 0; i -- )  // test op []
   cout << a[i];
  cout << endl;

  c = b;

  const char * pokus = (const char*)  c;       // typecast op test

  cout << pokus << endl;

  c = a + b;                                // concatenation

  cout << "a = " << a << endl <<
          "b = " << b << endl <<
          "c = " << c << endl;


  c = a + "pokus";                               // user conversion constructor

  b = a + CStrRef ( 5 ); // ok
  b = a + CStrRef ( 5.0 ); // ok

  return 0;
}
//-------------------------------------------------------------------------------------------------
