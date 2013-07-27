//---------------------------------------------------------------------------

#ifndef BinaryHeapH
#define BinaryHeapH
//---------------------------------------------------------------------------
#include<iostream>
#include<vector>

using namespace std;

typedef struct
{
    int x;
    int y;

}tsPos;

namespace Heap
{
    template <class T>
    class BinaryHeap
    {
    public:
        int n;
        BinaryHeap();
        BinaryHeap(const T &err);
        void max_heapify(int i);
        void min_heapify(int i);
        void insert(const T &e , int PosX, int PosY );
        tsPos minnum() const;
        tsPos extract_min();
        void decrease_key(int x,const T &k);
        void kill(int x);

        int Search( int PosX, int PosY);
        vector<T> h;
        vector<tsPos> Pos;
    private:
        tsPos TmpData;
    };
    
    template<class T>
    BinaryHeap<T> heap_union(const BinaryHeap<T> &h1 , const BinaryHeap<T> &h2);
}

template<class T>
Heap::BinaryHeap<T>::BinaryHeap()
{
    TmpData.x = -1;
    TmpData.y = -1;

    h.push_back(T ());
    Pos.push_back( TmpData );
    n=1;
}
 
template<class T>
Heap::BinaryHeap<T>::BinaryHeap(const T &err)
{
    TmpData.x = err;
    TmpData.y = err;

    h.push_back(err);
    Pos.push_back(TmpData);
    n=1;
}
 
template<class T>
void Heap::BinaryHeap<T>::max_heapify( int i )
{
    for(;i>1;)
    {
        int p = i>>1;
	    if( h[i] < h[p] )
	    {
	        T temp( h[i] );
	        h[i] = h[p];
	        h[p] = temp;

            TmpData = Pos[i];
            Pos[i] = Pos[p];
            Pos[p] = TmpData;

	        i = p;
	    }
	    else
	        break;
    }
}
 
template<class T>
void Heap::BinaryHeap<T>::min_heapify(int i)
{
    if(i<1 ||i>=n)
        return;
    for(;;)
    {
        int left  = i<<1;
	    int right = left+1;
	    int smallest;

	    if( left >= n )
	        break;

	    if( right >= n )
	        smallest=left;
	    else
        {
	        if(h[left]<h[right])
	            smallest=left;
	        else
	            smallest=right;
	    }

	    if( h[smallest] < h[i] )
	    {
	        T temp( h[i] );
	        h[i] = h[smallest];
	        h[smallest] = temp;

            TmpData = Pos[i];
            Pos[i] = Pos[smallest];
            Pos[smallest] = TmpData;

	        i = smallest;
	    }
	    else
	        break;
    }
}
 
template<class T>
void Heap::BinaryHeap<T>::insert(const T &e , int PosX, int PosY)
{
    int Size = h.size();
    if( n >= Size )
    {
        h.push_back(e);
        TmpData.x = PosX;
        TmpData.y = PosY;
        Pos.push_back(TmpData);
    }
    else
    {
        h[n]=e;
        Pos[n].x = PosX;
        Pos[n].y = PosY;
    }
    n++;
    max_heapify(n-1);
}
 
template<class T>
tsPos Heap::BinaryHeap<T>::minnum() const
{
    if( n > 1 )
        return Pos[1];
    return Pos[0];
}
 
template<class T>
void Heap::BinaryHeap<T>::decrease_key( int x ,const T &k )
{
    if( x == -1 || h[x] < k  )
        return;
    h[x] = k;
    max_heapify( x );
}
 
template<class T>
void Heap::BinaryHeap<T>::kill(int x)
{
    if(x>=1 && x<n)
    {
        h[x] = h[n-1];
        Pos[x] = Pos[n-1];
	    min_heapify(x);
	    n--;
    }
}
template<class T>
tsPos Heap::BinaryHeap<T>::extract_min()
{
    if(n>1)
    {
        tsPos min = Pos[1];
	    kill(1);
	    return min;
    }
    return Pos[0];
}
template<class T>
int Heap::BinaryHeap<T>::Search( int PosX, int PosY )
{
    int sort = 1;
    while( sort < n )
    {
        if( Pos[sort].x == PosX && Pos[sort].y == PosY ) return sort;
        sort++;
    }
    return -1;
}
#endif

