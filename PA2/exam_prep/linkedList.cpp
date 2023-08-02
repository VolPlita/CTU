#include <iostream>

template <class T>
class CLinkedList
{
    struct Elem
    {
        T m_value;
        Elem * m_next;

        Elem(T val) : m_value(val), m_next(nullptr) {}
    };
    Elem * head;
    size_t length;
public:
    // constructor
    CLinkedList() : head(nullptr) {}
    // destructor
    // copy constructor
    // operator =
    // operator []
    // add element
    void add(T val)
    {
        Elem * new_elem = new Elem(val);
        new_elem->m_next = head;
        head = new_elem;
        length++;
    }
    // delete element by value
    void del_by_value()
    // delete element by index
    void del_on_index(int p)
    {
        if (p >= length) return;
        Elem * to_del = head;
        for (int i = 0; i < p; ++i)
        {
            to_del = to_del->m_next;
        }
        if (to_del == head)
        {
            head = head->m_next
        }
    }
    // sort
    // print
};

int main()
{
    return EXIT_SUCCESS;
}