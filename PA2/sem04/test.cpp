#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <array>
#include <cassert>
using namespace std;
#endif /* __PROGTEST__ */

class CTimeStamp
{
    int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
	int m_second;
public:
	CTimeStamp (int y = 0, int m = 0, int d = 0, int h = 0, int mi = 0, int s = 0) 
	{ 
		m_year = y;
		m_month = m;
		m_day = d;
		m_hour = h;
		m_minute = mi;
		m_second = s;
	}

	int get_year() const {return m_year;}
	int get_month() const {return m_month;}
	int get_day() const {return m_day;}
	int get_hour() const {return m_hour;}
	int get_minute() const {return m_minute;}
	int get_second() const {return m_second;}

	bool is_earlier (CTimeStamp another) const
	{
		if (m_year < another.get_year()) return true;
		else if (m_year == another.get_year())
		{
			if (m_month < another.get_month()) return true;
			else if (m_month == another.get_month())
			{
				if (m_day < another.get_day()) return true;
				else if (m_day == another.get_day())
				{
					if (m_hour < another.get_hour()) return true;
					else if (m_hour == another.get_hour())
					{
						if (m_minute < another.get_minute()) return true;
						else if (m_minute == another.get_minute())
						{
							if (m_second < another.get_second()) return true;
							else return false;
						}
						else return false;
					}
					else return false;
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
};

class CContact
{
	int m_phone_A;
	int m_phone_B;
	CTimeStamp m_time;
public:
	CContact (CTimeStamp t, int a, int b)
	{
		m_phone_A = a;
		m_phone_B = b;
		m_time = t;
	}

	CTimeStamp get_time() const {return m_time;}
	int get_A() const {return m_phone_A;}
	int get_B() const {return m_phone_B;}

	bool is_earlier (CContact another)
	{
		return (m_time.is_earlier(another.get_time()));
	}
};

class CEFaceMask
{
	vector<CContact> m_list;
public:
	CEFaceMask & addContact(CContact con)
	{
		// size_t len = m_list.size();

		// bool inserted = false;
		// for (size_t i = 0; i < len; i++)
		// {
		// 	if (con.is_earlier(m_list[i]))
		// 	{
		// 		m_list.insert(m_list.begin() + i, con);
		// 		inserted = true;
		// 		return *this;
		// 	}
		// }
		// if (!inserted) m_list.push_back(con);
		// return *this;
		m_list.push_back(con);
		return *this;
	}
	vector<int> listContacts(int phone) const
	{
		vector<int> result;
		size_t len = m_list.size();
		for (size_t i = 0; i < len; i++)
		{
			if ( m_list[i].get_A() != m_list[i].get_B() )
			{
				if (phone == m_list[i].get_A())
				{
					size_t len_r = result.size();
					bool found = false;
					for (size_t j = 0; j < len_r; j++)
					{
						if (m_list[i].get_B() == result[j])
						{
							found = true;
							break;
						}
					}
					if (found) continue;

					result.push_back(m_list[i].get_B());
				}
				else if (phone == m_list[i].get_B())
				{
					size_t len_r = result.size();
					bool found = false;
					for (size_t j = 0; j < len_r; j++)
					{
						if (m_list[i].get_A() == result[j])
						{
							found = true;
							break;
						}
					}
					if (found) continue;

					result.push_back(m_list[i].get_A());
				}
			}
		}
		return result;
	}
	vector<int> listContacts(int phone, CTimeStamp start, CTimeStamp finish) const
	{
		vector<int> result;
		size_t len = m_list.size();
		for (size_t i = 0; i < len; i++)
		{	
			if ( (m_list[i].get_A() != m_list[i].get_B()) && (m_list[i].get_time().is_earlier(finish)) && (!m_list[i].get_time().is_earlier(start)))
			{
				if (phone == m_list[i].get_A())
				{
					size_t len_r = result.size();
					bool found = false;
					for (size_t j = 0; j < len_r; j++)
					{
						if (m_list[i].get_B() == result[j])
						{
							found = true;
							break;
						}
					}
					if (found) continue;

					result.push_back(m_list[i].get_B());
				}
				else if (phone == m_list[i].get_B())
				{
					size_t len_r = result.size();
					bool found = false;
					for (size_t j = 0; j < len_r; j++)
					{
						if (m_list[i].get_A() == result[j])
						{
							found = true;
							break;
						}
					}
					if (found) continue;

					result.push_back(m_list[i].get_A());
				}
			}
		}
		return result;
	}
};

#ifndef __PROGTEST__
int main ()
{
  CEFaceMask test;

  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
       . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
  test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
  vector<int> a = test.listContacts ( 123456789 );
  assert ( test . listContacts ( 123456789 ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 999888777 ) == (vector<int> {123456789, 555000222}) );
  assert ( test . listContacts ( 191919191 ) == (vector<int> {}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), CTimeStamp ( 2021, 2, 21, 18, 0, 0 ) ) == (vector<int> {999888777, 111222333, 456456456}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 5, 18, 0, 1 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {999888777, 111222333}) );
  assert ( test . listContacts ( 123456789, CTimeStamp ( 2021, 1, 10, 12, 41, 9 ), CTimeStamp ( 2021, 2, 21, 17, 59, 59 ) ) == (vector<int> {111222333}) );
  return 0;

  //_________________

//   test . addContact ( CContact ( CTimeStamp ( 2021, 1, 5, 18, 0, 0 ), 123456789, 456456456 ) );
//   test . addContact ( CContact ( CTimeStamp ( 2021, 1, 10, 12, 40, 10 ), 123456789, 999888777 ) );
//   test . addContact ( CContact ( CTimeStamp ( 2021, 1, 12, 12, 40, 10 ), 123456789, 111222333 ) )
//   test . addContact ( CContact ( CTimeStamp ( 2021, 2, 1, 0, 0, 0 ), 123456789, 123456789 ) );
//        . addContact ( CContact ( CTimeStamp ( 2021, 2, 5, 15, 30, 28 ), 999888777, 555000222 ) );
//   test . addContact ( CContact ( CTimeStamp ( 2021, 2, 21, 18, 0, 0 ), 123456789, 999888777 ) );
}
#endif /* __PROGTEST__ */
