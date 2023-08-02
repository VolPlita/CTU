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

class TContact {
public:
    string m_name;
    string m_surname;
    string m_phone;
    TContact(string name, string surname, string phone) {
        m_name = name;
        m_surname = surname;
        m_phone = phone;
    }
};

bool isphone(string a)
{
    int k = a.size();
    if(k != 9 || a[0] == '0') return false;
    for (int i = 0; i < k; ++i) {
        if(!isdigit(a[i])) return false;
    }
    return true;
}

bool report ( const string & fileName, ostream & out )
{
  //Open a file:
  ifstream read_from;
  read_from.open(fileName);
  if(!read_from.is_open()) return false;

  //Reading contact list from file
  vector<TContact> list;
  string buf;
  while (true)
  {
      getline(read_from, buf);
      if(buf == "") break;
      stringstream read_line(buf);
      string name, surname, phone, check = "";
      read_line >> name >> surname >> phone >> check;

      if(!isphone(phone) || check != "") return false;

      TContact tmp(name, surname, phone);
      list.push_back(tmp);
  }

  //Searching
  while(!read_from.eof())
  {
      string search;
      int n = 0;
      getline(read_from, search);
      if(search == "") continue;
      //cout << "Search for: " << search << ";" << endl;
      for(int i = 0; i < (int)list.size(); ++i)
      {
          if(search == list[i].m_surname || search == list[i].m_name)
          {
              out << list[i].m_name << " " << list[i].m_surname << " " << list[i].m_phone << endl;
              n++;
          }
      }
      out << "-> " << n << endl;
  }

  //read_from.close();
  return true;
}

#ifndef __PROGTEST__
int main ()
{
  ostringstream oss;
  oss . str ( "" );
  assert ( report( "tests/test0_in.txt", oss ) == true );
  assert ( oss . str () ==
    "John Christescu 258452362\n"
    "John Harmson 861647702\n"
    "-> 2\n"
    "-> 0\n"
    "Josh Dakhov 264112084\n"
    "Dakhov Speechley 865216101\n"
    "-> 2\n"
    "John Harmson 861647702\n"
    "-> 1\n" );
  oss . str ( "" );
  assert ( report( "tests/test1_in.txt", oss ) == false );
  return 0;
}
#endif /* __PROGTEST__ */
