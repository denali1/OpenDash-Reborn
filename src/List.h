#ifndef __List_H__
#define __List_H__

// List<T> class is a template class to implement a linked list
// of a given object.  It has built in functions for adding,
// and manipulating each List's elements.

template<class T>
class List {

public:
   List(T *data, List<T> *link);
   List();
   
   ~List();
   
   List<T> *getLink();
   T *getData();
   T *getData(int fIndex);
   
   void addItem(T *m);
   int getLength();
   
   void setLink(List<T> *m);
   void setData(T *m);
   
private:
   T *mData;
   List<T> *mLink;

};

template<class T>
List<T>::List()
{
	mData = 0;
	mLink = 0;
}

template<class T>
List<T>::List(T* data, List<T> *link)
{
	mData = data;
	mLink = link;
}

template<class T>
List<T>::~List()
{
	if (mData) {
	   delete mData;
	}
	
	if (mLink) {
	   delete mLink;
	}
}

template<class T>
void List<T>::addItem(T *m)
{
	if (mLink) {
	   mLink->addItem(m);
	} else {
	   mLink = new List<T>(m, 0);
	}
}

template<class T>
int List<T>::getLength()
{
	if (mLink) {
	   return 1 + mLink->getLength();
	} else {
	   return 1;
	}
}
   
template<class T>
List<T> *List<T>::getLink()
{
	return mLink;
}

template<class T>
T *List<T>::getData()
{
	return mData;
}

template<class T>
T *List<T>::getData(int fIndex)
{
        if (fIndex == 0) {
           return mData;
        } else {
           if (mLink) {
              return mLink->getData(fIndex-1);
           } else {
              return 0;
           }
        }
}

template<class T>
void List<T>::setLink(List<T> *l)
{
	mLink = l;
}

template<class T>
void List<T>::setData(T *d)
{
	mData = d;
}


#endif
