//////////////////////////////////////////////////////////////////////
// 向量类库
// tvec.h - 基于STL技术编写

#pragma warning (disable: 4786)

#ifndef _TVEC_
#define _TVEC_

#include <string>
#include <vector>
#include <iostream>
using namespace std;

template <class T>
class tvec : public vector<T>
{
// Attributes
private:
	vector<T> vt;		// vector of T
	long nn;			// number of elements

public :
	// Construction / Destruction
	tvec() { nn = 0; vt.resize(0); }
	tvec(long n) { nn = n; vt.resize(n); }
	tvec(vector<T> v) { nn = v.size(); vt = v; };
	tvec(long n, vector<T> v) { nn = n; vt = v; }
	tvec(long n, T x) { 
		nn = n;
		for(long i=1;i<=n;i++) 
			vt.push_back(x);
	}
	tvec(long n, T *x) { 
		nn = n; 
		for(long i=1;i<=n;i++) 
			vt.push_back(x[i-1]);
	}
	~tvec() { nn = 0; vt.resize(0); }

	tvec(const tvec<T> &a)	// copy
	{
		nn = a.nn;
		vt = a.vt;
	}

	tvec<T> &operator = (const tvec<T> &a) // assign
	{
		nn = a.nn;
		vt = a.vt;
		if (this == &a) return *this;
		return *this;
	}

	long nsize() { return nn; }

	long resize(long n) { nn = n; vt.resize(n); return n;}

	T &operator() (long i)
	{
		return vt[i - 1];

	}

	friend ostream &operator << (ostream &s, tvec<T> &v)
	{	
		cout << v.nsize() << endl;
		for(int i=1;i<=v.nsize();i++)  
			s << v(i) << endl;

		return s;

	}	

};

#endif 