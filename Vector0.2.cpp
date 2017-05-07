#include <iostream>
#include <cstdio>
#include <exception>
#include <new>
#include <algorithm>

enum CONSTANTS
  {
  VECTOR_CAPACITY = 16,
  POISON_DATA = -666,
  };

template <typename T>
class Vector
  {
  public:
    Vector();
    explicit Vector(const Vector & source);
    ~Vector();
     
    T & operator [](const size_t index);
    const Vector & operator = (const Vector & that);   

    bool verify() const;
 
    void dump() const;
    size_t capacity() const;
    size_t size() const;
    void print() const;    

    void push_back(const T val);
  private:
    size_t Capacity_;
    size_t Size_;
    T* Data_;
  };

class VectorBadIndex: public std::exception                 //! that exception will be thrown
  {                                                         //! if we refer in wrong index
  virtual const char* what() const throw()
    {
    return "Index is out of range";
    }
  };
class VectorNoData: public std::exception                   //! will be thrown in case of
  {                                                         //! Data_ == NULL
  virtual const char* what() const throw()
    {
    return "Data lost";
    }
  };

#define VERIFY \
  if(!verify()) \
    { \
    VectorNoData nodata; \
    throw nodata; \
    }

template <typename T>
Vector<T>::Vector():
  Capacity_(VECTOR_CAPACITY),
  Size_(0),
  Data_(new T [Capacity_])
  {
  VERIFY;
  for(size_t i = 0; i < Capacity_; ++i)
    {
    Data_[i] = POISON_DATA;
    }
  }

template <typename T>
Vector<T>::~Vector()
  {
  Capacity_ = POISON_DATA;
  Size_ = POISON_DATA;
  delete [] Data_;
  }

template <typename T>
Vector<T>::Vector(const Vector & that):
  Capacity_(that.Capacity_),
  Size_(that.Size_),
  Data_(new T [Capacity_])
  {
  std::copy(that.Data_, that.Data_ + Capacity_, Data_);
  VERIFY;
  }

template <typename T>
const Vector<T> & Vector<T>::operator = (const Vector & that)
  {
  VERIFY;
  if(this == &that)
    {
    return *this;
    }
  this->~Vector();
  new(this)Vector(that);
  VERIFY;
  return *this;
  }
//
template <typename T>
T & Vector<T>::operator [](const size_t index)
  {
  VERIFY;
  if(index >= Size_)
    {
    VectorBadIndex error;
    throw error;
    }
  return Data_[index];
  }

template <typename T>
void Vector<T>::push_back(const T val)
  {
  VERIFY;
  if(Size_ == Capacity_)
    {
    Capacity_ *= 2;
    Data_ = (T*)realloc(Data_, sizeof(T) * Capacity_);
    for(size_t i = Size_; i < Capacity_; ++i)Data_[i] = POISON_DATA;
    }
  Data_[Size_++] = val;
  VERIFY;
  } 

template <typename T>
void Vector<T>::dump() const
  {
  printf("\n======================================\n");
  printf("Vector:dump\nCapacity_ = %zu\nSize_ = %zu\nData_ = %p\n", Capacity_, Size_, Data_);
  for(size_t i = 0; i < Capacity_; i++)
    {
    std::cout << '[' << i << ']' << Data_ + i << " - " << Data_[i];
    if(Data_[i] == POISON_DATA)std::cout << " // poisoned data. Cell is empty or broken";
    std::cout << '\n';
    }
  printf("\n");
  }
    
template <typename T>
bool Vector<T>::verify() const
  {
  if(!Data_)return false;
  return true;
  }

template <typename T>
size_t Vector<T>::capacity() const
  {
  VERIFY;
  return Capacity_;
  }

template <typename T>
size_t Vector<T>::size() const
  {
  VERIFY;
  return Size_;
  }



int main()
  {
  Vector<int> v;
  for(int i = 0; i < 201; i++)v.push_back(i);
  for(int i = 0; i < v.size(); i++)v[i] *= v[i];
  for(int i = 0; i < v.size(); i++)
    { 
    printf("%d\n", v[i]);
    }
  Vector<int> g;
  Vector<int> a;
  g = a = v;
  g.dump();
  }

















