#include "tools/varchar.hpp"

namespace oos {

varchar_base::varchar_base(size_type capacity)
  : capacity_(capacity)
{}

varchar_base::varchar_base(const varchar_base &x)
  : capacity_(x.capacity_)
  , data_(x.data_)
{}

varchar_base& varchar_base::operator=(const varchar_base &x)
{
  capacity_ = x.capacity_;
  data_ = x.data_;
  return *this;
}

varchar_base& varchar_base::operator=(const std::string &x)
{
  ok(x);
  data_ = x;
  return *this;
}

varchar_base& varchar_base::operator=(const char *x)
{
  ok(x);
  data_.assign(x);
  return *this;
}

varchar_base::~varchar_base()
{}

bool varchar_base::operator==(const varchar_base &x) const
{
  return data_ == x.data_;
}

bool varchar_base::operator!=(const varchar_base &x) const
{
  return !this->operator ==(x);
}

varchar_base& varchar_base::operator+=(const varchar_base &x)
{
  data_ += x.str();
  return *this;
}

varchar_base& varchar_base::operator+=(const std::string &x)
{
  data_ += x;
  return *this;
}

varchar_base& varchar_base::operator+=(const char *x)
{
  data_ += x;
  return *this;
}

void varchar_base::assign(const char *s, size_t n)
{
  data_.assign(s, n);
}

std::string varchar_base::str() const
{
  return data_;
}

varchar_base::size_type varchar_base::size() const
{
  return data_.size();
}

varchar_base::size_type varchar_base::capacity() const
{
  return capacity_;
}

std::ostream& operator<<(std::ostream &out, const varchar_base &val)
{
  out << val.str();
  return out;
}

void varchar_base::ok(const std::string &x)
{
  if (x.size() > capacity_) {
    throw std::logic_error("string is to long");
  }
}

}