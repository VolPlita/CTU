#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>
using namespace std;

class CRect
{
public:
    CRect ( double x, double y, double w, double h ) :
            m_X ( x ), m_Y ( y ), m_W ( w ), m_H ( h )
    {}
    friend ostream & operator << ( ostream & os, const CRect & x )
    {
        return os << '(' << x . m_X << ',' << x . m_Y << ',' << x . m_W << ',' << x . m_H << ')';
    }
    double m_X;
    double m_Y;
    double m_W;
    double m_H;
};
#endif /* __PROGTEST__ */

class CObject
{
public:
    int m_id;
    CRect m_rect;
    CRect m_relative;

    CObject () = delete;
    CObject (int id, const CRect & relPos) : m_id(id), m_rect(relPos), m_relative(relPos) {}
    CObject (int id, const CRect & rec, const CRect & rel) : m_id(id), m_rect(rec), m_relative(rel) {}
    // Copy constructor
    CObject (const CObject & another) : m_id(another.m_id), m_rect(another.m_rect), m_relative(another.m_relative) {}
    virtual ~CObject () = default;

    double get_rel_x() const {return m_relative.m_X;}
    double get_rel_y() const {return m_relative.m_Y;}
    double get_rel_w() const {return m_relative.m_W;}
    double get_rel_h() const {return m_relative.m_H;}
    void set_rect(const CRect & newrect) {m_rect = newrect;}

    virtual void print(ostream & os, int depth) const = 0;
    virtual void print_in_middle(ostream & os, int depth) const = 0;

    virtual shared_ptr<CObject> dup() const = 0;

    friend ostream & operator << ( ostream & os, const CObject & x )
    {
        x.print(os, -1);
        return os;
    }
};

class CTextObject : public CObject
{
public:
    string m_text;

    CTextObject () = delete;
    CTextObject (int id, const CRect & relPos, const string & text) : CObject(id, relPos), m_text(text) {}
    CTextObject (int id, const CRect & rec, const CRect & rel, const string & text) : CObject(id, rec, rel), m_text(text) {}
    // Copy constructor 
    CTextObject (const CTextObject & another) : CObject(another.m_id, another.m_rect), m_text(another.m_text) {}
    virtual ~CTextObject () = default;
    virtual void print(ostream & os, int depth) const override = 0;
    virtual shared_ptr<CObject> dup() const override = 0;
};

class CButton : public CTextObject
{
public:
    CButton ( int id, const CRect & relPos, const string & name ) : CTextObject (id, relPos, name) {}
    CButton (const CButton & another) : CTextObject(another.m_id, another.m_rect, another.m_relative, another.m_text) {}
    void print(ostream & os, int depth) const override
    {
        for (int i = 0; i < depth; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Button " << "\"" << m_text << "\" " << m_rect << endl;
    }
    void print_in_middle(ostream & os, int depth) const override
    {
        if (depth > 0) os << "|  ";
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Button " << "\"" << m_text << "\" " << m_rect << endl;
    }

    shared_ptr<CObject> dup() const override {return make_shared<CButton>(*this);}
};

class CInput : public CTextObject
{
public:
    CInput ( int id, const CRect & relPos, const string & value ) : CTextObject (id, relPos, value) {}
    CInput (const CInput & another) : CTextObject(another.m_id, another.m_rect, another.m_relative, another.m_text) {}
    // setValue
    CInput & setValue(const string & x)
    {
        m_text = x;
        return *this;
    }
    // getValue
    string getValue() const {return m_text;}

    void print(ostream & os, int depth) const override
    {
        for (int i = 0; i < depth; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Input " << "\"" << m_text << "\" " << m_rect << endl;
    }
    void print_in_middle(ostream & os, int depth) const override
    {
        if (depth > 0) os << "|  ";
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Input " << "\"" << m_text << "\" " << m_rect << endl;
    }

    shared_ptr<CObject> dup() const override {return make_shared<CInput>(*this);}
};
class CLabel :public CTextObject
{
public:
    CLabel ( int id, const CRect & relPos, const string & label ) : CTextObject (id, relPos, label) {}
    CLabel (const CLabel & another) : CTextObject(another.m_id, another.m_rect, another.m_relative, another.m_text) {}
    void print(ostream & os, int depth) const override
    {
        for (int i = 0; i < depth; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Label " << "\"" << m_text << "\" " << m_rect << endl;
    }
    void print_in_middle(ostream & os, int depth) const override
    {
        if (depth > 0) os << "|  ";
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Label " << "\"" << m_text << "\" " << m_rect << endl;
    }

    shared_ptr<CObject> dup() const override {return make_shared<CLabel>(*this);}
};

class CComboBox : public CObject
{
    vector<string> m_options;
    int m_selected;
public:
    CComboBox ( int id, const CRect & relPos ) : CObject (id, relPos), m_selected(0) {}
    CComboBox ( const CComboBox & another) : CObject(another.m_id, another.m_rect, another.m_relative) 
    {
        m_options = another.m_options;
        m_selected = another.m_selected;
    }
    // ADD
    CComboBox & add (const string & str)
    {
        m_options.push_back(str);
        return *this;
    }
    // setSelected
    CComboBox & setSelected(int x)
    {
        m_selected = x;
        return *this;
    }
    // getSelected
    int getSelected() const {return m_selected;}

    void print(ostream & os, int depth) const override
    {
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] ComboBox " << m_rect << endl;
        for (size_t i = 0, l = m_options.size(); i < l; ++i)
        {
            for (int j = 0, k = depth + 1; j < k; ++j)
                os << "   ";
            if ((int)i == m_selected)
			{
				os << "+->";
				os << m_options[i];
				os << "<" << endl;
			} 
			else 
			{
				os << "+- ";
				os << m_options[i] << endl;
			}
        }
    }
    void print_in_middle(ostream & os, int depth) const override
    {
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] ComboBox " << m_rect << endl;
        for (size_t i = 0, l = m_options.size(); i < l; ++i)
        {
            os << "|  ";
            for (int j = 0; j < depth; ++j)
                os << "   ";
            if (depth >= 0)
			{
                if ((int)i == m_selected)
				{
					os << "+->";
					os << m_options[i];
					os << "<" << endl;
				} 
				else 
				{
					os << "+- ";
					os << m_options[i] << endl;
				}
			}
        }
    }

    shared_ptr<CObject> dup() const override {return make_shared<CComboBox>(*this);}
};

class CWindow : public CTextObject
{
    multimap<int, shared_ptr<CObject>> m_components;
public:
    CWindow ( int id, const string & title, const CRect & absPos ) : CTextObject(id, absPos, title) {}
    CWindow ( const CWindow & another ) : CTextObject(another.m_id, another.m_rect, another.m_relative, another.m_text)
    {
        for (auto comp : another.m_components)
        {
            auto new_ptr = comp.second->dup();
            m_components.emplace(new_ptr->m_id, new_ptr);
        }
    }
    //Add functions
    template<typename T>
    CWindow & add(const T & obj)
    {
        auto obj_ptr = make_shared<T>(obj);
        obj_ptr->set_rect(CRect ( m_rect.m_W * obj_ptr->get_rel_x() + m_rect.m_X,
                                  m_rect.m_H * obj_ptr->get_rel_y() + m_rect.m_Y,
                                  m_rect.m_W * obj_ptr->get_rel_w(),
                                  m_rect.m_H * obj_ptr->get_rel_h() ));
        m_components.emplace(obj.m_id, obj_ptr);
        return *this;
    }
    // /Add functions

    // search
    CObject * search(int id) const
    {
        auto it = m_components.find(id);
        if (it == m_components.end()) return nullptr;
        else return const_cast<CObject *>(&(*it->second));
    }
    // setPosition
    void setPosition(const CRect & newPos)
    {
        m_rect = newPos;
        for (auto it = m_components.begin(); it != m_components.end(); ++it)
        {
            it->second->set_rect(CRect (m_rect.m_W * it->second->get_rel_x() + m_rect.m_X,
                                        m_rect.m_H * it->second->get_rel_y() + m_rect.m_Y,
                                        m_rect.m_W * it->second->get_rel_w(),
                                        m_rect.m_H * it->second->get_rel_h() ));
        }
    }

    void print(ostream & os, int depth) const override
    {
        for (int i = 0; i < depth; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Window " << "\"" << m_text << "\" " << m_rect << endl;
        ++depth;
        for (auto it = m_components.begin(); it != m_components.end(); ++it)
        {
            if (it == --m_components.end())
                it->second->print(os, depth);
            else
                it->second->print_in_middle(os, depth);
        }
    }
    void print_in_middle(ostream & os, int depth) const override
    {
        if (depth > 0) os << "|  ";
        for (int i = 0, l = depth - 1; i < l; ++i)
            os << "   ";
        if (depth >= 0) os << "+- ";
        os << "[" << m_id << "] Window " << "\"" << m_text << "\" " << m_rect << endl;
        ++depth;
        for (auto it = m_components.begin(); it != m_components.end(); ++it)
        {
            if (it == --m_components.end())
                it->second->print(os, depth);
            else
                it->second->print_in_middle(os, depth);
        }
    }

    shared_ptr<CObject> dup() const override {return make_shared<CWindow>(*this);}
};

#ifndef __PROGTEST__
template <typename _T>
string toString ( const _T & x )
{
    ostringstream oss;
    oss << x;
    return oss . str ();
}

int main ( void )
{
    assert ( sizeof ( CButton ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CInput ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    assert ( sizeof ( CLabel ) - sizeof ( string ) < sizeof ( CComboBox ) - sizeof ( vector<string> ) );
    CWindow a ( 0, "Sample window", CRect ( 10, 10, 600, 480 ) );
    a . add ( CButton ( 1, CRect ( 0.1, 0.8, 0.3, 0.1 ), "Ok" ) ) . add ( CButton ( 2, CRect ( 0.6, 0.8, 0.3, 0.1 ), "Cancel" ) );
    a . add ( CLabel ( 10, CRect ( 0.1, 0.1, 0.2, 0.1 ), "Username:" ) );
    a . add ( CInput ( 11, CRect ( 0.4, 0.1, 0.5, 0.1 ), "chucknorris" ) );
    a . add ( CComboBox ( 20, CRect ( 0.1, 0.3, 0.8, 0.1 ) ) . add ( "Karate" ) . add ( "Judo" ) . add ( "Box" ) . add ( "Progtest" ) );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    CWindow b = a;
    assert ( toString ( *b . search ( 20 ) ) ==
             "[20] ComboBox (70,154,480,48)\n"
             "+->Karate<\n"
             "+- Judo\n"
             "+- Box\n"
             "+- Progtest\n" );
    assert ( dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . getSelected () == 0 );
    dynamic_cast<CComboBox &> ( *b . search ( 20 ) ) . setSelected ( 3 );
    assert ( dynamic_cast<CInput &> ( *b . search ( 11 ) ) . getValue () == "chucknorris" );
    dynamic_cast<CInput &> ( *b . search ( 11 ) ) . setValue ( "chucknorris@fit.cvut.cz" );
    b . add ( CComboBox ( 21, CRect ( 0.1, 0.5, 0.8, 0.1 ) ) . add ( "PA2" ) . add ( "OSY" ) . add ( "Both" ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (70,250,480,48)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    assert ( toString ( a ) ==
             "[0] Window \"Sample window\" (10,10,600,480)\n"
             "+- [1] Button \"Ok\" (70,394,180,48)\n"
             "+- [2] Button \"Cancel\" (370,394,180,48)\n"
             "+- [10] Label \"Username:\" (70,58,120,48)\n"
             "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
             "+- [20] ComboBox (70,154,480,48)\n"
             "   +->Karate<\n"
             "   +- Judo\n"
             "   +- Box\n"
             "   +- Progtest\n" );
    b . setPosition ( CRect ( 20, 30, 640, 520 ) );
    assert ( toString ( b ) ==
             "[0] Window \"Sample window\" (20,30,640,520)\n"
             "+- [1] Button \"Ok\" (84,446,192,52)\n"
             "+- [2] Button \"Cancel\" (404,446,192,52)\n"
             "+- [10] Label \"Username:\" (84,82,128,52)\n"
             "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
             "+- [20] ComboBox (84,186,512,52)\n"
             "|  +- Karate\n"
             "|  +- Judo\n"
             "|  +- Box\n"
             "|  +->Progtest<\n"
             "+- [21] ComboBox (84,290,512,52)\n"
             "   +->PA2<\n"
             "   +- OSY\n"
             "   +- Both\n" );
    return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
