#ifndef DSTRING_H
#define DSTRING_H

#include "DObject.h"
#include "DException.h"

namespace DLib {

using std::ostream;

template<typename T>
class baseString : public DObject<>
{
private:
    T* m_storage;
    size_t m_length;

    void init(const T* str);

    void init(const T& c, size_t num);

public:
    baseString();

    baseString(T& c);

    baseString(const T &s);

    baseString(const baseString<T>& s);

public:


};


typedef baseString<char> DString;
typedef baseString<wchar_t> DWString;

}





#endif // DSTRING_H
