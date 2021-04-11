//
// Created by yb on 4/10/21.
//
#include "vector.h"
namespace ybSTL{
    //构造函数
    template<class T,class Alloc>
    vector<T,Alloc>::~vector(){
        destroyAndDeallocateAll();
    }
    template<class T, class Alloc>
    vector<T, Alloc>::vector(const size_type n){
        allocateAndFillN(n, value_type());
    }
    template<class T,class Alloc>
    vector<T,Alloc>::vector(const size_type n,const value_type& val){
        allocateAndFillN(n,val);
    }
    template<class T,class Alloc>
    template<class InputIterator>
    vector<T,Alloc>::vector(InputIterator first,InputIterator last){
        //处理指针和数字间区别的函数
        //TODO:
        vector_aux(first,last,typename std::is_integral<InputIterator>::type());
    }
    template<class T,class Alloc>
    vector<T,Alloc>::vector(const vector&v){
        allocateAndCopy(v._start,v._finish);
    }
    template<class T,class Alloc>
    vector<T,Alloc>::vector(vector&& v){
        _start=v._start;
        _finish=v._finish;
        _endOfStorage=v._endOfStorage;
        v._start=v._finish=v._endOfStorage=0;
    }
    template<class T,class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator=(const vector&v) {
        if(this!=&v)
            allocateAndCopy(v._start,v._finish);
        return *this;
    }
    template<class T,class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator=(vector &&v) {
        if (this!=&v){
            destroyAndDeallocateAll();
            _start=v._start;
            _finish=v._finish;
            _endOfStorage=v._endOfStorage;
            v._start=v._finish=v._endOfStorage=0;
        }
        return *this;
    }
    //和容器容量相关
    template<class T,class Alloc>
    void vector<T,Alloc>::resize(size_type n, value_type val) {
        if (n<size()){
            dataAllocator::destroy(_start+n,_finish);
            _finish=_start+n;
        }
        else if(n>size() && n<=capacity()){
            auto lengthOfInsert = n - size();
            _finish=ybSTL::uninitialized_fill_n(_finish,lengthOfInsert,val);
        }
        else if(n>capacity()){
            auto lengthOfInsert=n-size();
            T *newStart=dataAllocator::allocate(getNewCapacity(lengthOfInsert));
            T *newFinish=ybSTL::uninitialized_copy(begin(),end(),newStart);
            newFinish=ybSTL::uninitialized_fill_n(newFinish,lengthOfInsert,val);
            destroyAndDeallocateAll();
            _start=newStart;
            _finish=newFinish;
            _endOfStorage=_start+n;
        }
    }
    template<class T,class Alloc>
    void vector<T,Alloc>::reserve(size_type n) {
        if (n>capacity()){
            auto lengthOfInsert=n-size();
            T *newStart=dataAllocator::allocate(getNewCapacity(lengthOfInsert));
            T *newFinish= uninitialized_copy(begin(),end(),newStart);
            destroyAndDeallocateAll();
            _start=newStart;
            _finish=newFinish;
            _endOfStorage=_start+n;
        }
    }

    template<class T,class Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator position) {
        return erase(position,position+1);
    }
    template<class T,class Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator first,iterator last) {
        difference_type lenOfTail = end() - last;
        difference_type lenOfRemoved = last - first;
        _finish=_finish-lenOfRemoved;
        for (; lenOfTail != 0 ; --lenOfTail) {
            auto temp=last-lenOfRemoved;
            *temp=*(last++);
        }
        return (first);
    }
    template<class T,class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::reallocateAndCopy(iterator position, InputIterator first, InputIterator last) {
        difference_type newCapacity= getNewCapacity(last-first);
        T *newStart=dataAllocator::allocate(newCapacity);
        T *newEndOfStorage=newStart+newCapacity;
        T *newFinish=ybSTL::uninitialized_copy(begin(),position,newStart);
        newFinish=ybSTL::uninitialized_copy(first,last,newFinish);
        newFinish=ybSTL::uninitialized_copy(position,end(),newFinish);
        destroyAndDeallocateAll();
        _start=newStart;
        _finish=newFinish;
        _endOfStorage=newEndOfStorage;
    }
    template<class T,class Alloc>
    void vector<T,Alloc>::reallocateAndFillN(iterator position, const size_type &n, const value_type &val) {
        T *newStart=dataAllocator::allocate(n);
        T *newEndOfStorage=newStart+n;
        T *newFinish=ybSTL::uninitialized_copy(begin(),position,newStart);
        newFinish=ybSTL::uninitialized_fill_n(newFinish,n,val);
        newFinish=ybSTL::uninitialized_copy(position,end(),newFinish);
        destroyAndDeallocateAll();
        _start=newStart;
        _finish=newFinish;
        _endOfStorage=newEndOfStorage;
    }
    template<class T,class Alloc>

}
