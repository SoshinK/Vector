#include <iostream>
#include <cstdio>
#include <exception>
#include <new>
#include <algorithm>

typedef int dtype;

enum CONSTANTS
  {
  VECTOR_CAPACITY = 16,
  POISON_DATA = -666,
  };

class Vector
  {
  public:
    Vector();
    explicit Vector(const Vector & source);
    ~Vector();
     
    dtype & operator [](const size_t index);
    Vector & operator = (const Vector & that);   

    bool verify() const;
 
    void dump() const;
    size_t capacity() const;
    size_t size() const;
    void print() const;    

    void push_back(dtype val);
  private:
    size_t Capacity_;
    size_t Size_;
    dtype* Data_;
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

Vector::Vector():
  Capacity_(VECTOR_CAPACITY),
  Size_(0),
  Data_(new dtype [Capacity_])
  {
  VERIFY;
  for(size_t i = 0; i < Capacity_; ++i)
    {
    Data_[i] = POISON_DATA;
    }
  }

Vector::~Vector()
  {
  Capacity_ = POISON_DATA;
  Size_ = POISON_DATA;
  delete [] Data_;
  }

Vector::Vector(const Vector & that):
  Capacity_(that.Capacity_),
  Size_(that.Size_),
  Data_(new dtype [Capacity_])
  {
  std::copy(that.Data_, that.Data_ + Capacity_, Data_);
  VERIFY;
  }

Vector & Vector::operator = (const Vector & that)
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

dtype & Vector::operator [](const size_t index)
  {
  VERIFY;
  if(index >= Size_)
    {
    VectorBadIndex error;
    throw error;
    }
  return Data_[index];
  }

void Vector::push_back(dtype val)
  {
  VERIFY;
  if(Size_ == Capacity_)
    {
    Capacity_ *= 2;
    Data_ = (dtype*)realloc(Data_, sizeof(dtype) * Capacity_);
    for(size_t i = Size_; i < Capacity_; ++i)Data_[i] = POISON_DATA;
    }
  Data_[Size_++] = val;
  VERIFY;
  } 

void Vector::dump() const
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
    

bool Vector::verify() const
  {
  if(!Data_)return false;
  return true;
  }

size_t Vector::capacity() const
  {
  VERIFY;
  return Capacity_;
  }

size_t Vector::size() const
  {
  VERIFY;
  return Size_;
  }

int main()
  {
  Vector a;
  a.dump();
  a.push_back(1);
  a.push_back(2);
  a[1] = 0;
  a.push_back(3);
  a.push_back(3);
  a.push_back(3);
  a.push_back(3);
  a.dump();
  }


