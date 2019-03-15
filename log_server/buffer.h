#ifndef BUFFER_H
#define BUFFER_H
class buffer
{
public:
    buffer();
    buffer(const char* pMsg, int len);

    char* get_buffer() { return m_pHead; }
    int get_len() { return m_len; }

    ~buffer();

private:
    char* m_pHead;
    int m_len;
    int m_maxLen;
};
#endif

