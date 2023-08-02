#ifndef __PROGTEST__
#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>
using namespace std;

class CTimeTester;
#endif /* __PROGTEST__ */

class CTime
{
private:
    int m_Hour;
    int m_Minute;
    int m_Second;

public:
    int get_h() const {return m_Hour;}
    int get_m() const {return m_Minute;}
    int get_s() const {return m_Second;}

    void set_h(int a) {m_Hour = a;}
    void set_m(int a) {m_Minute = a;}
    void set_s(int a) {m_Second = a;}

    int conv_seconds() const
    {
        return (m_Hour * 3600 + m_Minute * 60 + m_Second);
    }

    // constructor, destructor
    CTime() {m_Hour = 0, m_Minute = 0, m_Second = 0;} 
    CTime(int h, int m) 
    {
        if (h >= 24 || m >= 60 || h < 0 || m < 0)
        {
            throw invalid_argument("Oh shit!");
        }
        m_Hour = h;
        m_Minute = m;
        m_Second = 0;
    }
    CTime(int h, int m, int s) 
    {
        if (h >= 24 || m >= 60 || s >= 60 || h < 0 || m < 0 || s < 0)
        {
            throw invalid_argument("Oh shit!");
        }
        m_Hour = h;
        m_Minute = m; 
        m_Second = s;
    }
    CTime(const CTime &a) {m_Hour = a.get_h(), m_Minute = a.get_m(), m_Second = a.get_s();}
    CTime& operator = (const CTime &a) {this->set_h(a.get_h()), this->set_m(a.get_m()), this->set_s(a.get_s()); return *this;}

    // arithmetic operators
    // + with time
    CTime operator + (const CTime& b) const
    {
        CTime result;
        int seconds = this->conv_seconds() + b.conv_seconds();

        result.set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        result.set_m((seconds / 60 % 60));
        seconds -= ((seconds / 60) * 60);
        result.set_s(seconds % 60);

        return result;
    }
    CTime& operator += (const CTime& b) 
    {
        int seconds = this->conv_seconds() + b.conv_seconds();
        this->set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        this->set_m((seconds / 60) % 60);
        seconds -= ((seconds / 60) * 60);
        this->set_s(seconds % 60);

        return *this;
    }

    // + with seconds 
    CTime operator + (int b) const
    {
        CTime result;
        int seconds = this->conv_seconds() + b;

        result.set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        result.set_m((seconds / 60 % 60));
        seconds -= ((seconds / 60) * 60);
        result.set_s(seconds % 60);

        return result;
    }
    CTime& operator += (int b)
    {
        int seconds = this->conv_seconds() + b;
        this->set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        this->set_m((seconds / 60) % 60);
        seconds -= ((seconds / 60) * 60);
        this->set_s(seconds % 60);

        return *this;
    }

    // - with time
    int operator - (const CTime& b) const
    {
        //CTime result;
        int seconds = this->conv_seconds() - b.conv_seconds();
        if (seconds < 0) seconds = (0 - seconds);
        // result.set_h((seconds / 3600) % 24);
        // seconds -= ((seconds / 3600) * 3600);
        // result.set_m((seconds / 60 % 60));
        // seconds -= ((seconds / 60) * 60);
        // result.set_s(seconds % 60);

        // return result;
        return seconds;
    }
    
    CTime& operator -= (const CTime& b)
    {
        int seconds = this->conv_seconds() - b.conv_seconds();
        if (seconds < 0) seconds = (0 - seconds);
        this->set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        this->set_m((seconds / 60) % 60);
        seconds -= ((seconds / 60) * 60);
        this->set_s(seconds % 60);

        return *this;
    }

    // - with seconds 
    CTime operator - (int b) const
    {
        CTime result;
        int seconds = this->conv_seconds() - b;
        if (seconds < 0) seconds = (0 - seconds);
        result.set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        result.set_m((seconds / 60 % 60));
        seconds -= ((seconds / 60) * 60);
        result.set_s(seconds % 60);

        return result;
    }
    CTime& operator -= (int b)
    {
        int seconds = this->conv_seconds() - b;
        if (seconds < 0) seconds = (0 - seconds);
        this->set_h((seconds / 3600) % 24);
        seconds -= ((seconds / 3600) * 3600);
        this->set_m((seconds / 60) % 60);
        seconds -= ((seconds / 60) * 60);
        this->set_s(seconds % 60);

        return *this;
    }

    CTime& operator ++ ()
    {
        *this += 1;
        return *this;
    }

    CTime& operator -- ()
    {
        *this -= 1;
        return *this;
    }

    CTime operator ++ ( int )
    {
        CTime res(this->get_h(), this->get_m(), this->get_s());
        *this += 1;
        return res;
    }

    CTime operator -- ( int )
    {
        CTime res(this->get_h(), this->get_m(), this->get_s());
        *this -= 1;
        return res;
    }

    // comparison operators
    bool operator == (CTime a) const
    {
        return ( ( this->get_h() == a.get_h() ) &&
                 ( this->get_m() == a.get_m() ) &&
                 ( this->get_s() == a.get_s() ));
    }
    bool operator == (int a) const
    {
        int b = this->conv_seconds();
        return (b == a);
    }
    bool operator != (CTime a) const
    {
        return !( ( this->get_h() == a.get_h() ) &&
                 ( this->get_m() == a.get_m() ) &&
                 ( this->get_s() == a.get_s() ));
    }
    bool operator != (int a) const
    {
        int b = this->conv_seconds();
        return !(b == a);
    }
    // <
    bool operator < (CTime b) const
    {
        int seca = this->conv_seconds();
        int secb = b.conv_seconds();
        return (seca < secb);
    }
    bool operator < (int b) const
    {
        int seca = this->conv_seconds();
        return (seca < b);
    }
    // >
    bool operator > (CTime b) const
    {
        int seca = this->conv_seconds();
        int secb = b.conv_seconds();
        return (seca > secb);
    }
    bool operator > (int b) const
    {
        int seca = this->conv_seconds();
        return (seca > b);
    }
    //  <=
    bool operator <= (CTime b) const
    {
        int seca = this->conv_seconds();
        int secb = b.conv_seconds();
        return (seca <= secb);
    }
    bool operator <= (int b) const
    {
        int seca = this->conv_seconds();
        return (seca <= b);
    }
    // >=
    bool operator >= (CTime b) const
    {
        int seca = this->conv_seconds();
        int secb = b.conv_seconds();
        return (seca >= secb);
    }
    bool operator >= (int b) const
    {
        int seca = this->conv_seconds();
        return (seca >= b);
    }
    // output operator
    void operator << (ostream& out) const
    {
        out << setw(2) << setfill(' ') << this->get_h() << ':' << setfill('0') << this->get_m() << ':' << this->get_s();
    }

    friend class ::CTimeTester;
};

ostream& operator << (ostream& out, const CTime &a)
{
    out << setw(2) << setfill(' ') << a.get_h() << ':' << setw(2) << setfill('0') << a.get_m() << ':' << setw(2) << setfill('0') << a.get_s();
    return out;
}

CTime operator + (int a, CTime &b)
{
    CTime result;
    int seconds = b.conv_seconds() + a;

    result.set_h((seconds / 3600) % 24);
    seconds -= ((seconds / 3600) * 3600);
    result.set_m((seconds / 60 % 60));
    seconds -= ((seconds / 60) * 60);
    result.set_s(seconds % 60);

    return result;
}

#ifndef __PROGTEST__
struct CTimeTester
{
    static bool test ( const CTime & time, int hour, int minute, int second )
    {
        return time.m_Hour == hour 
            && time.m_Minute == minute
            && time.m_Second == second;
    }
};

#include <cassert>
#include <sstream>

int main ()
{
    CTime a { 12, 30 };
    assert( CTimeTester::test( a, 12, 30, 0 ) );

    CTime b { 13, 30 };
    assert( CTimeTester::test( b, 13, 30, 0 ) );

    assert( b - a == 3600 );

    assert( CTimeTester::test( a + 60, 12, 31, 0 ) );
    assert( CTimeTester::test( a - 60, 12, 29, 0 ) );

    assert( a < b );
    assert( a <= b );
    assert( a != b );
    assert( !( a > b ) );
    assert( !( a >= b ) );
    assert( !( a == b ) );

    while ( ++a != b );
    assert( a == b );

    std::ostringstream output;
    assert( static_cast<bool>( output << a ) );
    assert( output.str() == "13:30:00" );

    assert( a++ == b++ );
    assert( a == b );

    assert( --a == --b );
    assert( a == b );

    assert( a-- == b-- );
    assert( a == b );

    return 0;
}
#endif /* __PROGTEST__ */
