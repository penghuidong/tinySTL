#include "stdafx.h"
#include "DString.h"

namespace DLib
{

template<typename T>
void baseString<T>::init(const T& chart, size_t num)
{
    m_storage = static_cast<T*>(malloc(sizeof(T)*num));
    if(m_storage!=NULL)
        for(size_t i=0; i<num; i++)
            m_storage[i] = chart;
    else
        throw_exception(outOfMemory, "no memory to create baseString");
}

template<typename T>
void baseString<T>::init(const T* str)
{
    m_storage = strdup(str);
    if(m_storage)
        m_length = strlen(m_storage);
    else
        throw_exception(outOfMemory, "no memory to create baseString");
}

template<typename T>
baseString<T>::baseString()  {init("");}

template<typename T>
baseString<T>::baseString(T& c) {init(c, 1);}

template<typename T>
baseString<T>::baseString(const T& s)
{

}

template<typename T>
baseString<T>::baseString(const baseString<T>& s)
{

}

}
