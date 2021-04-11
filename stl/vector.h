//
// Created by yb on 4/10/21.
//

#ifndef LEARNCPP_VECTOR_H
#define LEARNCPP_VECTOR_H

#include <algorithm>
#include <type_traits>

#include "alloc.h"
#include "allocator.h"
#include "Iterator.h"
#include "reverseIterator.h"
#include "uninitialized.h"

namespace ybSTL {
    //***********vector**********
    template<class T, class Alloc=allocator<T>>
    class vector {
    private:
        T *_start;
        T *_finish;
        T *_endOfStorage;
        typedef Alloc dataAllocator;
    public:
        typedef T value_type;
        typedef T *iterator;
        typedef const T *const_iterator;
        typedef reverse_iterator_t<T *> reverse_iterator;
        typedef reverse_iterator_t<const T *> const_reverse_iterator;
        typedef iterator pointer;
        typedef T &reference;
        typedef const T &const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    public:
        vector():_start(0),_finish(0),_endOfStorage(0){}
        explicit vector (const size_type n);
        vector(const size_type n,const value_type& value);
        template<class InputIterator>
        vector(InputIterator first,InputIterator last);
        vector(const vector& v);
        vector(vector&& v);
        vector& operator = (const vector& v);
        vector& operator = (vector&& v);
        ~vector();

        bool operator == (const vector& v)const;
        bool operator != (const vector& v)const;

        iterator begin(){return (_start);}
        const_iterator begin()const{ return (_start); }
        const_iterator cbegin()const{ return (_start); }
        iterator end(){ return (_finish); }
        const_iterator end()const{ return (_finish); }
        const_iterator cend()const{ return (_finish); }
        reverse_iterator rbegin(){ return reverse_iterator(_finish); }
        const_reverse_iterator crbegin()const{ return const_reverse_iterator(_finish); }
        reverse_iterator rend(){ return reverse_iterator(_start); }
        const_reverse_iterator crend()const{ return const_reverse_iterator(_start); }

        difference_type size() const{return _finish-_start;}
        difference_type capacity() const{return _endOfStorage-_start;}
        bool empty() const{return _start==_finish;}
        void resize(size_type n,value_type val=value_type());
        void reserve(size_type n);
        void shrink_to_fit();

        reference operator[](const difference_type i){return *(begin()+i);}
        const_reference operator[](const difference_type i)const{return *(cbegin()+i);}
        reference front(){return *(begin());}
        reference back(){return *(end()-1);}
        pointer data(){return _start;}

        void clear();
        void swap(vector &v);
        void push_back(const value_type &value);
        void pop_back();
        iterator insert(iterator position,const value_type &val);
        void insert(iterator position,const size_type &n,const value_type &val);
        template <class InputIterator>
        void insert(iterator position,InputIterator first,InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first,iterator last);

        Alloc get_allocator(){return dataAllocator();}

    private:
        void destroyAndDeallocateAll();
        void allocateAndFillN(const size_type n, const value_type& value);
        template<class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);

        template<class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type);
        template<class Integer>
        void vector_aux(Integer n, const value_type& value, std::true_type);
        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
        template<class Integer>
        void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
        template<class InputIterator>
        void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
        void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
        size_type getNewCapacity(size_type len)const;
    public:
        friend bool operator == (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
        friend bool operator != (const vector<T, Alloc>& v1, const vector<T, Alloc>& v2);
    };
}
#endif //LEARNCPP_VECTOR_H
