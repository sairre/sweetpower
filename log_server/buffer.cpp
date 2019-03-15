#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
buffer::buffer():m_pHead(0),m_len(0),m_maxLen(0)
{
    
}


buffer::buffer(const char* pMsg, int len)
{
    if (len <= 0)
    {
        return ;
    }
    if (NULL == pMsg)
    {
        return;
    }
    m_pHead = (char*)calloc(1, sizeof(len));
    if (NULL == m_pHead)
    {
        return;
    }
    
    memmove(m_pHead, pMsg, len);
}

buffer::~buffer()
{
    if (NULL != m_pHead)
    {
        free(m_pHead);
        m_pHead = NULL;
    }
}
