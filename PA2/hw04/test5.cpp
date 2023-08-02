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

template <typename T>
class shared_ptr 
{
public:
    shared_ptr(T * ptr = nullptr) : 
        m_ptr(ptr), m_ref_count(nullptr) 
    {
        if (m_ptr != nullptr)
            m_ref_count = new size_t(1);
    }

    // copy constructor
    shared_ptr(const shared_ptr<T> & another) : 
        m_ptr(another.m_ptr), m_ref_count(another.m_ref_count) 
    {
        if (m_ref_count != nullptr)
            ++(*m_ref_count);
    }
    ~shared_ptr() {cleanup();}

    shared_ptr<T> & operator = (const shared_ptr<T> & another) 
    {
        if (this != &another) 
        {
            cleanup();
            m_ptr = another.m_ptr;
            m_ref_count = another.m_ref_count;
            if (m_ref_count)
                ++(*m_ref_count);
        }
        return *this;
    }

    T & operator*() const {return *m_ptr;}
    T * operator->() const {return m_ptr;}
    T * get_ptr() const {return m_ptr;}

private:
    T * m_ptr;
    size_t * m_ref_count; 
    
    void cleanup() 
    {
        if (m_ref_count != nullptr) 
        {
            --(*m_ref_count);
            if (*m_ref_count <= 0) 
            {
                delete  m_ref_count;
                delete  [] m_ptr;
            }
            m_ptr = nullptr;
            m_ref_count = nullptr;
        }
    }
};
class CMail 
{
public:
    CMail() {}
    ~CMail() {}
    CMail(const char * from, const char * to, const char * body) :
            m_from(new char[strlen(from) + 1]), 
            m_to(new char[strlen(to) + 1]), 
            m_body(new char[strlen(body) + 1])
    {
        strcpy(m_from.get_ptr(), from);
        strcpy(m_to.get_ptr(), to);
        strcpy(m_body.get_ptr(), body);
    }
    CMail(const CMail & another) :
            m_from(new char[strlen(another.m_from.get_ptr()) + 1]), 
            m_to(new char[strlen(another.m_to.get_ptr()) + 1]), 
            m_body(new char[strlen(another.m_body.get_ptr()) + 1])
    {
        strcpy(m_from.get_ptr(), another.m_from.get_ptr());
        strcpy(m_to.get_ptr(), another.m_to.get_ptr());
        strcpy(m_body.get_ptr(), another.m_body.get_ptr());
    }

    bool operator==(const CMail & x) const 
    {
        return !strcmp(m_from.get_ptr(), x.m_from.get_ptr()) &&
               !strcmp(m_to.get_ptr(), x.m_to.get_ptr()) &&
               !strcmp(m_body.get_ptr(), x.m_body.get_ptr());
    }

    friend ostream &operator<<(ostream & os, const CMail & m) 
    {
        return os << "From: " << m.m_from.get_ptr() << ", To: " << m.m_to.get_ptr() << ", Body: " << m.m_body.get_ptr();
    };

    char * get_str_from() const {return m_from.get_ptr();}
    char * get_str_to() const {return m_to.get_ptr();}

private:
    shared_ptr<char> m_from;
    shared_ptr<char> m_to;
    shared_ptr<char> m_body;
};
class vectorCMail 
{
public:
     vectorCMail(size_t n = 10) 
     {
        vec = new CMail[n];
        current = 0;
        capacity = n;
    }
    vectorCMail(const vectorCMail & another) 
    {
        capacity = another.capacity;
        current = another.current;
        vec = new CMail[capacity];
        for (size_t i = 0; i < current; i++)
            vec.get_ptr()[i] = CMail(another.vec.get_ptr()[i]);
    }
    ~vectorCMail() {};

    void push_back(const CMail &mail)
    {
        if (current == capacity - 5) resize(2);
        vec.get_ptr()[current] = mail;
        ++current;
    }

    size_t get_current() const {return current;}
    CMail &operator[](int index) const {return vec.get_ptr()[index];}

private:
    shared_ptr<CMail>vec;
    size_t current;
    size_t capacity;

    void resize(size_t factor)
    {
        capacity = factor * capacity + 5;
        auto * new_vec = new CMail[capacity]();
        for (size_t i = 0; i < current; i++)
            new_vec[i] = CMail(vec.get_ptr()[i]);
        vec=new_vec;
    }
};


struct CAccount 
{
    shared_ptr<char> m_email;
    vectorCMail m_inbox;
    vectorCMail m_outbox;

    CAccount() : m_email(nullptr) {}
    ~CAccount() {};
    CAccount(const char * email) : 
        m_email(new char[strlen(email) + 1]) 
    {
        strcpy(m_email.get_ptr(), email);
    }

    CAccount(CAccount & another) : 
        m_email(new char[strlen(another.m_email.get_ptr()) + 1]) 
    {
        strcpy(m_email.get_ptr(), another.m_email.get_ptr());
        m_inbox = vectorCMail(another.m_inbox);
        m_outbox = vectorCMail(another.m_outbox);
    }
};

class CHashTable 
{
public:
    CHashTable(size_t n = 500) : m_table(new CAccount[n]), m_cap(n)
    {
        m_table = new CAccount[n];
        m_cap = n;
    }
    CHashTable(const CHashTable & another) 
    {
        m_cap = another.m_cap;
        m_table = new CAccount[m_cap];
        for (size_t i = 0; i < another.m_cap; i++) 
        {
            if(another.m_table[i].m_email.get_ptr() != nullptr)
                m_table[i] = CAccount(another.m_table[i]);
        }
    }
    ~CHashTable() {delete [] m_table;}

    bool exists(const char* key){
        size_t hash= hasher(key);
        if(hash>m_cap){
            return false;
        }
        if(m_table[hash].m_email.get_ptr()== nullptr){
            return false;
        }

        if(strcmp(m_table[hash].m_email.get_ptr(),key)==0){
            return true;
        }
        while (hash<m_cap){
            if(m_table[hash].m_email.get_ptr() != nullptr){
                if(strcmp(m_table[hash].m_email.get_ptr(),key)==0){
                    return true;
                }
            }
            hash++;
        }
        return false;
    }

    void insert(const char* key){
        size_t hash= hasher(key);
        if(hash>m_cap){
            resize();
        }
        while (m_table[hash].m_email.get_ptr()){
            if(hash>m_cap){
                resize();
            }
            hash++;
        }
        m_table[hash]=CAccount(key);
    }

    CAccount& find(const char *key){
        size_t hash= hasher(key);
        while (strcmp(m_table[hash].m_email.get_ptr(),key)!=0){
            hash++;
        }
        return m_table[hash];
    }

private:
    CAccount * m_table;
    size_t m_cap;
    static size_t hasher(const char* key){
        size_t len=strlen(key);
        size_t sum=0;
        for(size_t i=0; i<len; i++){
            sum+=(int)key[i];
        }
        return sum/len;
    }
    void resize(){
        size_t newSize = m_cap*2+10;
        auto *newList=new CAccount[newSize]();
        for (size_t i = 0; i < m_cap; i++) {
            newList[i] = CAccount(m_table[i]);
        }
        delete[]m_table;
        m_cap=newSize;
        m_table=newList;
    }
};

class CMailIterator {
public:

    CMailIterator() {
        current = 0;
    }

    explicit CMailIterator(vectorCMail &boxFound) {
        box = boxFound;
        current = 0;
    }


    explicit operator bool() const {
        if (current < box.get_current()) {
            return true;
        }
        return false;
    }

    bool operator!(void) const {
        if (current < box.get_current()) {
            return false;
        }
        return true;
    }

    const CMail &operator*() const {
        return box[current];
    }

    CMailIterator &operator++() {
        current = current + 1;
        return *this;
    }

    [[nodiscard]] vectorCMail getBox() const{
        return box;
    }


    [[nodiscard]] CMail getCurrent() const {
        return box[current];
    }

private:
    vectorCMail box;
    size_t current;
};

class CMailServer {
public:
    CMailServer() {
        map = new CHashTable();
    }


    CMailServer(const CMailServer &src) {
        map= new CHashTable(*src.map);
    }

    CMailServer& operator=(const CMailServer& other){
        if (this != &other) {

            CHashTable* newMap = new CHashTable(*other.map);

            delete map;

            map = newMap;
        }
        return *this;
    }

    ~CMailServer(){
        delete map;
    }

    void sendMail(const CMail &m) const{

        if (!map->exists(m.get_str_from())) {
            map->insert(m.get_str_from());
        }
        map->find(m.get_str_from()).m_outbox.push_back(m);

        if (!map->exists(m.get_str_to())) {
            map->insert(m.get_str_to());
        }
        map->find(m.get_str_to()).m_inbox.push_back(m);
    }

    CMailIterator outbox(const char *email) const {

        if (map->exists(email) && map->find(email).m_outbox.get_current()) {
            return CMailIterator(map->find(email).m_outbox);
        } else {
            return {};
        }
    }

    CMailIterator inbox(const char *email) const {
        if (map->exists(email) && map->find(email).m_inbox.get_current()) {
            return CMailIterator(map->find(email).m_inbox);
        } else {
            return {};
        }
    }

    [[nodiscard]] CHashTable *getMap() const {
        return map;
    }

private:
    // todo
    CHashTable *map;

};

#ifndef __PROGTEST__

bool matchOutput(const CMail &m,
                 const char *str) {
    ostringstream oss;
    oss << m;
    return oss.str() == str;
}

int main(void) {
    char from[100], to[100], body[1024];

    assert (CMail("john", "peter", "progtest deadline") == CMail("john", "peter", "progtest deadline"));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("john", "progtest deadline", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "john", "progtest deadline")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("peter", "progtest deadline", "john")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "john", "peter")));
    assert (!(CMail("john", "peter", "progtest deadline") == CMail("progtest deadline", "peter", "john")));
    CMailServer s0;
    s0.sendMail(CMail("john", "peter", "some important mail"));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "thomas", sizeof(to));
    strncpy(body, "another important mail", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    strncpy(from, "john", sizeof(from));
    strncpy(to, "alice", sizeof(to));
    strncpy(body, "deadline notice", sizeof(body));
    s0.sendMail(CMail(from, to, body));
    s0.sendMail(CMail("alice", "john", "deadline confirmation"));
    s0.sendMail(CMail("peter", "alice", "PR bullshit"));
    CMailIterator i0 = s0.inbox("alice");
    assert (i0 && *i0 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i0, "From: john, To: alice, Body: deadline notice"));
    assert (++i0 && *i0 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i0, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i0);


    CMailIterator i1 = s0.inbox("john");
    assert (i1 && *i1 == CMail("alice", "john", "deadline confirmation"));
    assert (matchOutput(*i1, "From: alice, To: john, Body: deadline confirmation"));
    assert (!++i1);
    CMailIterator i2 = s0.outbox("john");
    assert (i2 && *i2 == CMail("john", "peter", "some important mail"));
    assert (matchOutput(*i2, "From: john, To: peter, Body: some important mail"));
    assert (++i2 && *i2 == CMail("john", "thomas", "another important mail"));
    assert (matchOutput(*i2, "From: john, To: thomas, Body: another important mail"));
    assert (++i2 && *i2 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i2, "From: john, To: alice, Body: deadline notice"));
    assert (!++i2);

    CMailIterator i3 = s0.outbox("thomas");
    assert (!i3);

    CMailIterator i4 = s0.outbox("steve");
    assert (!i4);

    CMailIterator i5 = s0.outbox("thomas");
    s0.sendMail(CMail("thomas", "boss", "daily report"));
    assert (!i5);

    CMailIterator i6 = s0.outbox("thomas");
    assert (i6 && *i6 == CMail("thomas", "boss", "daily report"));
    assert (matchOutput(*i6, "From: thomas, To: boss, Body: daily report"));
    assert (!++i6);

    CMailIterator i7 = s0.inbox("alice");
    s0.sendMail(CMail("thomas", "alice", "meeting details"));
    assert (i7 && *i7 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i7, "From: john, To: alice, Body: deadline notice"));
    assert (++i7 && *i7 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i7, "From: peter, To: alice, Body: PR bullshit"));
    assert (!++i7);

    CMailIterator i8 = s0.inbox("alice");
    assert (i8 && *i8 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i8, "From: john, To: alice, Body: deadline notice"));
    assert (++i8 && *i8 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i8, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i8 && *i8 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i8, "From: thomas, To: alice, Body: meeting details"));
    assert (!++i8);

    CMailServer s1(s0);
    s0.sendMail(CMail("joe", "alice", "delivery details"));
    s1.sendMail(CMail("sam", "alice", "order confirmation"));
    CMailIterator i9 = s0.inbox("alice");
    assert (i9 && *i9 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i9, "From: john, To: alice, Body: deadline notice"));
    assert (++i9 && *i9 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i9, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i9 && *i9 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i9, "From: thomas, To: alice, Body: meeting details"));
    assert (++i9 && *i9 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i9, "From: joe, To: alice, Body: delivery details"));
    assert (!++i9);

    CMailIterator i10 = s1.inbox("alice");

    assert (i10 && *i10 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i10, "From: john, To: alice, Body: deadline notice"));
    assert (++i10 && *i10 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i10, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i10 && *i10 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i10, "From: thomas, To: alice, Body: meeting details"));
    assert (++i10 && *i10 == CMail("sam", "alice", "order confirmation"));
    assert (matchOutput(*i10, "From: sam, To: alice, Body: order confirmation"));
    assert (!++i10);

    CMailServer s2;
    s2.sendMail(CMail("alice", "alice", "mailbox test"));
    CMailIterator i11 = s2.inbox("alice");
    assert (i11 && *i11 == CMail("alice", "alice", "mailbox test"));
    assert (matchOutput(*i11, "From: alice, To: alice, Body: mailbox test"));
    assert (!++i11);

    s2 = s0;
    s0.sendMail(CMail("steve", "alice", "newsletter"));
    s2.sendMail(CMail("paul", "alice", "invalid invoice"));
    CMailIterator i12 = s0.inbox("alice");
    assert (i12 && *i12 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i12, "From: john, To: alice, Body: deadline notice"));
    assert (++i12 && *i12 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i12, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i12 && *i12 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i12, "From: thomas, To: alice, Body: meeting details"));
    assert (++i12 && *i12 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i12, "From: joe, To: alice, Body: delivery details"));
    assert (++i12 && *i12 == CMail("steve", "alice", "newsletter"));
    assert (matchOutput(*i12, "From: steve, To: alice, Body: newsletter"));
    assert (!++i12);

    CMailIterator i13 = s2.inbox("alice");
    assert (i13 && *i13 == CMail("john", "alice", "deadline notice"));
    assert (matchOutput(*i13, "From: john, To: alice, Body: deadline notice"));
    assert (++i13 && *i13 == CMail("peter", "alice", "PR bullshit"));
    assert (matchOutput(*i13, "From: peter, To: alice, Body: PR bullshit"));
    assert (++i13 && *i13 == CMail("thomas", "alice", "meeting details"));
    assert (matchOutput(*i13, "From: thomas, To: alice, Body: meeting details"));
    assert (++i13 && *i13 == CMail("joe", "alice", "delivery details"));
    assert (matchOutput(*i13, "From: joe, To: alice, Body: delivery details"));
    assert (++i13 && *i13 == CMail("paul", "alice", "invalid invoice"));
    assert (matchOutput(*i13, "From: paul, To: alice, Body: invalid invoice"));
    assert (!++i13);

    cout << "Finally success!!!" << endl;
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */