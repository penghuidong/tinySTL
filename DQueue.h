#ifndef DQUEUE_H
#define DQUEUE_H

#include "DSingleLinkList.h"
#include "DException.h"

namespace DLib {

template <typename T>
class DQueue
{
private:
    DSingleLinkList<T> mqueue;
public:
    DQueue() {}

    void push(const T& val){mqueue.insert(0, val); }

    T pop(){return mqueue.remove(mqueue.length()-1);}

    T front()
    {
		if (mqueue.length() > 1)
			return mqueue[mqueue.length()-1];
		else if (mqueue.length() == 1)
			return mqueue[0];
        else
            throw_exception(invalidOperation, "DQueue length is 0");
    }

    size_t length() const{return mqueue.length();}

    bool empty() const{return !mqueue.length();}
};


}


#endif // DQUEUE_H
