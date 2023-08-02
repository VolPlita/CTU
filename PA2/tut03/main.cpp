#include <stdio.h>

class TArray
{
    int * m_arr = nullptr;
    int m_len = 0;
    int m_cap = 0;

    void check_enlarge(int req)
    {
        /*
        through while OR

        if(pos >= m_cap)
        {
            m_cap = max(2 * m_cap, pos) + 1;
            int * tmp = new int[m_cap]{0};
            for(int i = 0, i < m_size; i++)
            {
                tmp[i] = m_arr[i];
            }
            delete [] m_arr;
            m_arr = tmp;
        }
        */
        if (req <= m_cap) return;

        m_cap = req * 2;

        int * tmp = new int [m_cap];
        memcpy(tmp, m_arr, sizeof(*m_arr) * m_len);
        delete [] m_arr;
        m_arr = tmp;
    }

public:
    int insert(int pos, int val)
    {
        if(pos > m_len)
        {
            std::cout << "Out of bounds";
            return 0;
        }

        check(pos + 1);

        for (int i = m_size; i < pos; i--)
        {
            m_arr[i] = m_arr[i - 1];
        }

        m_arr[pos] = val;
        m_size++;
    }

    int remove(int pos)
    {

    }

    int get(int pos)
    {

    }
};

int main()
{

}