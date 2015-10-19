/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ATTRIBUTE_SERIALIZER_HPP
#define ATTRIBUTE_SERIALIZER_HPP

#include "object/object_atomizer.hpp"
#include "object/object_ptr.hpp"
#include "object/object_container.hpp"
#include "object/identifier.hpp"

#include <stdexcept>
#include <type_traits>
#include <cstring>

namespace oos {

class object_base_ptr;

template < class T, class V >
void convert(const T &, V &)
{

}

template < class T, class V >
void convert(const T &, V &, int)
{

}

template < class T, class V >
void convert(const T &, V &, int, int)
{

}

void convert(const char* from, char *to, int size);

void convert(const char* from, char *to, int size, int);

//template < class V >
//void convert(const char *, V &, int)
//{
//
//}
//
//template < class V >
//void convert(const char *, V &, int, int)
//{
//
//}
/**
 * @tparam T The type of the attribute to set.
 * @class attribute_reader
 * @brief Set an attribute value of an serializable.
 *
 * A given attribute value of a template type is
 * tried to set into an serializable. Therefor the attribute
 * with given name must be found and the value must
 * be convertible into the objects attribute.
 */

template < class T, class Enable = void >
class attribute_reader;

template < class T >
class attribute_reader<T, typename std::enable_if< !std::is_same<T, char*>::value >::type> : public generic_object_reader<attribute_reader<T> >
{
private:
  friend class generic_object_reader<attribute_reader<T> >;

public:
  /**
   * @brief Creates an attribute_reader
   *
   * Creates an attribute_reader for an attribute id of type T
   * where id is the name of the attribute.
   *
   * @tparam T The type of the attribute.
   * @param id The name of the attribute.
   * @param from The attribute value to set.
   */
  attribute_reader(const std::string &id, const T &from)
    : generic_object_reader<attribute_reader<T> >(this)
    , id_(id)
    , from_(from)
    , success_(false)
  {}
  
  virtual ~attribute_reader() {}

  /**
   * @brief True if value was set.
   *
   * Returns true if the value could
   * be set successfully.
   *
   * @return True if value was set.
   */
  bool success() const
  {
    return success_;
  }

private:
  template < class V >
  void read_value(const char *id, V &to, typename std::enable_if< std::is_same<T, V>::value >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    to = from_;
//    convert(from_, to);
    success_ = true;
  }

  template < class V >
  void read_value(const char *id, V &to, typename std::enable_if<
    std::is_base_of<V, T>::value &&
    std::is_same<V, oos::varchar_base>::value
  >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    to = from_;
  }

  template < class V >
  void read_value(const char *id, V &/*to*/, typename std::enable_if<
    std::is_base_of<V, T>::value &&
    std::is_same<V, oos::object_base_ptr>::value
  >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
  }

  template < class V >
  void read_value(const char *id, V &/*to*/, typename std::enable_if<
    !std::is_same<T, V>::value &&
    !std::is_base_of<V, T>::value>::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    // Todo: throw exception
  }

  void read_value(const char *id, basic_identifier &x)
  {
    x.deserialize(id, *this);
  }

  void read_value(const char *id, char *to, int s)
  {
    if (id_ != id) {
      return;
    }

    // convert any type to char*
    convert(from_, to, s);
    success_ = true;
  }
  void read_value(const char*, date&) {}
  void read_value(const char*, time&) {}
  void read_value(const char*, object_container&) {}

private:
  std::string id_;
  const T &from_;
  bool success_;
};

// set
template < class T >
class attribute_reader<T, typename std::enable_if< std::is_same<T, char*>::value >::type> : public generic_object_reader<attribute_reader<T> >
{
public:
  attribute_reader(const std::string &id, const char from[], int size)
    : generic_object_reader<attribute_reader<T> >(this)
    , id_(id)
    , from_(from)
    , size_(size)
    , success_(false)
  {}

  /**
 * @brief True if value was set.
 *
 * Returns true if the value could
 * be set successfully.
 *
 * @return True if value was set.
 */
  bool success() const
  {
    return success_;
  }

  template < class V >
  void read_value(const char *id, V &/*to*/)
  {
    if (id_ != id) {
      return;
    }
//    to = from_;
//    convert(from_, to);
    success_ = true;
  }

  void read_value(const char *id, char *to, int s)
  {
    if (id_ != id) {
      return;
    }
    if (s < size_) {
      return;
    }
    strncpy(to, from_, size_);
    to[size_] = '\0';
//    to = from_;
//    convert(from_, to);
    success_ = true;
  }
private:
  std::string id_;
  const char *from_;
  int size_;
  bool success_;
};

/**
 * @tparam T The type of the attribute to retieve.
 * @class attribute_writer
 * @brief Retrieve an attribute value from an serializable.
 *
 * A attribute value of a template type is
 * tried to retriev from an serializable. Therefor the attribute
 * of given name must be found and the value must
 * be convertible from the objects attribute.
 */
template < class T >
class attribute_writer : public generic_object_writer<attribute_writer<T> >
{
private:
  friend class generic_object_writer<attribute_writer<T> >;

public:
  /**
   * @brief Creates an attribute_writer
   *
   * Creates an attribute_writer for an attribute id of type T
   * where id is the name of the attribute.
   *
   * @tparam T The type of the attribute.
   * @param id The name of the attribute.
   * @param to The attribute value to retrieve.
   * @param precision The precision of the value.
   */
  attribute_writer(const std::string &id, T &to, int precision = -1)
    : generic_object_writer<attribute_writer<T> >(this)
    , id_(id)
    , to_(to)
    , success_(false)
    , precision_(precision)
  {}
   
  virtual ~attribute_writer() {}

  /**
   * @brief True if value could be retrieved.
   *
   * Returns true if the value could
   * be retrieved successfully.
   *
   * @return True if value could be retrieved.
   */
  bool success() const
  {
    return success_;
  }

private:
  template < class V >
  void write_value(const char *id, const V &from, typename std::enable_if<
    std::is_same<T, V>::value &&
    !std::is_floating_point<V>::value
  >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    to_ = from;
    success_ = true;
  }

  template < class V >
  void write_value(const char *id, const V &from, typename std::enable_if<
    std::is_same<T, V>::value &&
    std::is_floating_point<V>::value
  >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    if (precision_ < 0) {
      to_ = from;
//      convert(from, to_);
      success_ = true;
    } else {
      success_ = false;
//      convert(from, to_, precision_);
    }
  }

  template < class V >
  void write_value(const char *id, const V &from, typename std::enable_if<
    !std::is_same<T, V>::value &&
    std::is_same<V, oos::varchar_base>::value &&
    std::is_base_of<V, T>::value
  >::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    if (precision_ < 0) {
      to_ = from.str();
      success_ = true;
    } else {
      success_ = false;
//      convert(from, to_, precision_);
    }
  }

  template < class V >
  void write_value(const char *id, V &/*to*/, typename std::enable_if<
    !std::is_same<T, V>::value &&
    !std::is_base_of<V, T>::value>::type* = 0)
  {
    if (id_ != id) {
      return;
    }
    // Todo: throw exception
  }

  void write_value(const char *id, const basic_identifier &x)
  {
    x.serialize(id, *this);
  }

  void write_value(const char *id, const char *from, int)
  {
    if (id_ != id) {
      return;
    }
    if (precision_ < 0) {
      convert(from, to_);
    } else {
      convert(from, to_, precision_);
    }
    success_ = true;
  }

private:
  std::string id_;
  T &to_;
  bool success_;
  int precision_;
};

/// @cond OOS_DEV

template <>
class attribute_writer<char*> : public generic_object_writer<attribute_writer<char*> >
{
public:
  /**
   * @brief Creates an attribute_writer
   *
   * Creates an attribute_writer for an attribute id of type T
   * where id is the name of the attribute.
   *
   * @tparam T The type of the attribute.
   * @param id The name of the attribute.
   * @param to The attribute value to retrieve.
   */
  attribute_writer(const std::string &id, char *to, int size, int precision = -1)
    : generic_object_writer<attribute_writer<char*> >(this)
    , id_(id)
    , to_(to)
    , size_(size)
    , success_(false)
    , precision_(precision)
  {}
   
  virtual ~attribute_writer() {}

  /**
   * @brief True if value could be retrieved.
   *
   * Returns true if the value could
   * be retrieved successfully.
   *
   * @return True if value could be retrieved.
   */
  bool success() const
  {
    return success_;
  }

  template < class V >
  void write_value(const char *id, const V &from)
  {
    if (id_ != id) {
      return;
    }
    if (precision_ < 0) {
      convert(from, to_, size_);
    } else {
      convert(from, to_, size_, precision_);
    }
    success_ = true;
  }

  void write_value(const char*, const date&) {}
  void write_value(const char*, const time&) {}
  void write_value(const char*, const object_container&) {}
  void write_value(const char*, const basic_identifier &) {}

  void write_value(const char *id, const char *from, int)
  {
    if (id_ != id) {
      return;
    }
    if (precision_ < 0) {
      convert(from, to_, size_);
    } else {
      convert(from, to_, size_, precision_);
    }
    success_ = true;
  }

private:
  std::string id_;
  char *to_;
  int size_;
  bool success_;
  int precision_;
};

/// @endcond

}

#endif /* ATTRIBUTE_SERIALIZER_HPP */
