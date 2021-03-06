//
// Created by sascha on 1/15/16.
//

#ifndef OOS_TEST_HAS_MANY_LIST_HPP
#define OOS_TEST_HAS_MANY_LIST_HPP

#include "oos/object/basic_has_many.hpp"
#include "oos/object/object_store.hpp"

#include <list>

namespace oos {

/// @cond OOS_DEV

template < class T >
struct has_many_iterator_traits<T, std::list, typename std::enable_if<!std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef object_ptr<T> value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename container_type::const_iterator const_container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

template < class T >
struct has_many_iterator_traits<T, std::list, typename std::enable_if<std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef T value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename container_type::iterator const_container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

/// @endcond

/**
 * @brief Represents a has many iterator
 *
 * Represents a has many iterator for a has many
 * relationship with std::list container
 *
 * @tparam T The type of the iterator/container
 */
template < class T >
class has_many_iterator<T, std::list>
  : public has_many_iterator_traits<T, std::list>
{
private:
  typedef has_many_iterator_traits<T, std::list> traits;
  typedef typename traits::item_type item_type;
  typedef typename traits::internal_type internal_type;
  typedef typename traits::relation_type relation_type;
  typedef typename traits::container_type container_type;

public:
  typedef has_many_iterator<T, std::list> self;                               /**< Shortcut value self */
  typedef typename traits::value_type value_type;                             /**< Shortcut value type */
  typedef typename traits::difference_type difference_type;                   /**< Shortcut to the difference type*/
  typedef typename traits::container_iterator container_iterator;             /**< Shortcut to the internal container iterator */
  typedef typename traits::const_container_iterator const_container_iterator; /**< Shortcut to the internal const container iterator */

public:
  /**
   * @brief Creates an empty has many iterator
   */
  has_many_iterator() {}

  /**
   * @brief Copy constructs an iterator from another iterator
   *
   * @param iter The iterator to copy from
   */
  has_many_iterator(const self &iter) : iter_(iter.iter_) {}

  /**
   * @brief Creates a has many iterator from given internal container iterator
   *
   * @param iter The iterator to create the has many iterator from
   */
  explicit has_many_iterator(container_iterator iter) : iter_(iter) {}

  /**
   * @brief Copy assign an iterator from another iterator
   *
   * @param iter The iterator to copy from
   * @return A reference to self
   */
  has_many_iterator& operator=(const self &iter)
  {
    iter_ = iter.iter_;
    return *this;
  }

  ~has_many_iterator() {}

  /**
   * @brief Compares equality iterator with another iterator.
   *
   * Compares iterator with another iterator. If other iterator contain
   * the same element true es returned.
   *
   * @param i The iterator to compare with
   * @return True if iterators contain the same element
   */
  bool operator==(const self &i) const { return (iter_ == i.iter_); }

  /**
   * @brief Compares unequality iterator with another iterator.
   *
   * Compares iterator with another iterator. If other iterator doesn't
   * contain the same element true es returned.
   *
   * @param i The iterator to compare with
   * @return True if iterators doesn't contain the same element
   */
  bool operator!=(const self &i) const { return !this->operator==(i); }

  /**
   * @brief Returns the difference of two iterators a and b.
   *
   * @param a The minuend iterator
   * @param b The subtrahend iterator
   * @return The difference between both iterators
   */
  friend difference_type operator-(self a, self b) { return a.iter_ - b.iter_; }

  /**
   * @brief Pre increments self
   *
   * @return A reference to incremented self
   */
  self& operator++()
  {
    ++iter_;
    return *this;
  }

  /**
   * @brief Post increments iterator
   *
   * Post increments iterator and returns a
   * new iterator object.
   *
   * @return Returns new incremented iterator
   */
  self operator++(int)
  {
    self tmp = *this;
    ++iter_;
    return tmp;
  }

  /**
   * @brief Pre decrements self
   *
   * @return A reference to decremented self
   */
  self& operator--()
  {
    --iter_;
    return *this;
  }

  /**
   * @brief Post decrements iterator
   *
   * Post decrements iterator and returns a
   * new iterator object.
   *
   * @return Returns new decremented iterator
   */
  self operator--(int)
  {
    --iter_;
    return self();
  }

  //@{
  /**
   * Return the current value
   * represented by the iterator
   * 
   * @return The current value
   */
  value_type operator->() const { return get(); }
  value_type operator*() const { return get(); }
  value_type get() const { return (*iter_)->value(); }
  //@}

private:
  friend class has_many<T, std::list>;
  friend class const_has_many_iterator<T, std::list>;
  friend class basic_has_many<T, std::list>;
  friend class object_serializer;
  friend class detail::object_inserter;

  relation_type relation_item() const { return *iter_; }

  container_iterator iter_;
};

/// @cond OOS_DEV

template < class T >
struct const_has_many_iterator_traits<T, std::list, typename std::enable_if<!std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef object_ptr<T> value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

template < class T >
struct const_has_many_iterator_traits<T, std::list, typename std::enable_if<std::is_scalar<T>::value>::type>
  : public std::iterator<std::bidirectional_iterator_tag, T, std::ptrdiff_t, const T*, const T&>
{
  typedef has_many_iterator_traits<T, std::list> self;
  typedef T value_type;
  typedef has_many_item<T> item_type;
  typedef has_one<item_type> internal_type;
  typedef object_ptr<item_type> relation_type;
  typedef std::list<internal_type, std::allocator<internal_type>> container_type;
  typedef typename container_type::iterator container_iterator;
  typedef typename container_type::const_iterator const_container_iterator;
  typedef typename std::iterator<std::bidirectional_iterator_tag, T>::difference_type difference_type;
};

/// @endcond

/**
 * @brief Represents a const has many iterator
 *
 * Represents a const has many iterator for a has many
 * relationship with std::list container
 *
 * @tparam T The type of the iterator/container
 */
template < class T >
class const_has_many_iterator<T, std::list>
  : public const_has_many_iterator_traits<T, std::list>
{
private:
  typedef const_has_many_iterator_traits<T, std::list> traits;
  typedef typename traits::item_type item_type;
  typedef typename traits::internal_type internal_type;
  typedef typename traits::container_type container_type;
  typedef typename traits::relation_type relation_type;

public:
  typedef const_has_many_iterator<T, std::list> self;                         /**< Shortcut to self */
  typedef typename traits::value_type value_type;                             /**< Shortcut value type */
  typedef typename traits::difference_type difference_type;                   /**< Shortcut to the difference type*/
  typedef typename traits::container_iterator container_iterator;             /**< Shortcut to the internal container iterator */
  typedef typename traits::const_container_iterator const_container_iterator; /**< Shortcut to the internal const container iterator */

public:
  /**
   * @brief Creates an empty const has many iterator
   */
  const_has_many_iterator() {}

  /**
   * @brief Creates a const has many iterator from given internal container iterator
   *
   * @param iter The iterator to create the const has many iterator from
   */
  explicit const_has_many_iterator(container_iterator iter) : iter_(iter) {}

  /**
   * @brief Creates a const has many iterator from given internal const container iterator
   *
   * @param iter The const iterator to create the const has many iterator from
   */
  explicit const_has_many_iterator(const_container_iterator iter) : iter_(iter) {}

  /**
   * @brief Creates a const has many iterator from a has many iterator
   *
   * @param iter The iterator to create the const has many iterator from
   */
  const_has_many_iterator(const has_many_iterator<T, std::list> &iter) : iter_(iter.iter_) {}

  /**
   * @brief Copy assigns a new const has many iterator
   *
   * @param iter The iterator to be copy assigned from
   * @return Reference to the created iterator
   */
  const_has_many_iterator& operator=(const self &iter)
  {
    iter_ = iter.iter_;
    return *this;
  }

  /**
   * @brief Copy assigns a new const has many iterator
   *
   * Copy assigns a new const has many iterator from
   * a non const has many iterator
   *
   * @param iter The iterator to be copy assigned from
   * @return Reference to the created iterator
   */
  const_has_many_iterator& operator=(const has_many_iterator<T, std::list> &iter)
  {
    iter_ = iter.iter_;
    return *this;
  }
  ~const_has_many_iterator() {}

  /**
   * @brief Compares equality iterator with another iterator.
   *
   * Compares iterator with another iterator. If other iterator contain
   * the same element true es returned.
   *
   * @param i The iterator to compare with
   * @return True if iterators contain the same element
   */
  bool operator==(const self &i) const { return (iter_ == i.iter_); }

  /**
   * @brief Compares unequality iterator with another iterator.
   *
   * Compares iterator with another iterator. If other iterator doesn't
   * contain the same element true es returned.
   *
   * @param i The iterator to compare with
   * @return True if iterators doesn't contain the same element
   */
  bool operator!=(const self &i) const { return !this->operator==(i); }

  /**
   * @brief Pre increments self
   *
   * @return A reference to incremented self
   */
  self& operator++()
  {
    ++iter_;
    return *this;
  }

  /**
   * @brief Post increments iterator
   *
   * Post increments iterator and returns a
   * new iterator object.
   *
   * @return Returns new incremented iterator
   */
  self operator++(int)
  {
    self tmp = *this;
    ++iter_;
    return tmp;
  }

  /**
   * @brief Pre decrements self
   *
   * @return A reference to decremented self
   */
  self& operator--()
  {
    --iter_;
    return *this;
  }

  /**
   * @brief Post decrements iterator
   *
   * Post decrements iterator and returns a
   * new iterator object.
   *
   * @return Returns new decremented iterator
   */
  self operator--(int)
  {
    --iter_;
    return self();
  }

  //@{
  /**
   * Return the current value
   * represented by the iterator
   * 
   * @return The current value
   */
  const value_type operator->() const { return get(); }
  const value_type operator*() const { return get(); }
  const value_type get() const { return (*iter_)->value(); }
  //@}

private:
  friend class has_many<T, std::list>;
  friend class basic_has_many<T, std::list>;
  friend class object_serializer;
  friend class detail::object_inserter;

  const relation_type relation_item() const { return *iter_; }

  const_container_iterator iter_;
};

/**
 * @brief Has many relation class using a std::list as container
 *
 * The has many relation class uses a std::list as internal
 * container to store the objects.
 *
 * It provides all main interface functions std::list provides
 * - insert element at a iterator position
 * - push back an element
 * - push front an element
 * - erase an element at iterator position
 * - erase a range of elements within first and last iterator position
 * - clear the container
 *
 * All of these methods are wrappes around the std::list methods plus
 * the modification in the corresponding object_store and notification
 * of the transaction observer
 *
 * The relation holds object_ptr elements as well as scalar data elements.
 *
 * @tparam T The type of the elements
 */
template < class T >
class has_many<T, std::list> : public basic_has_many<T, std::list>
{
public:

  typedef basic_has_many<T, std::list> base;                    /**< Shortcut to self */
  typedef typename base::iterator iterator;                     /**< Shortcut to iterator type */
  typedef typename base::value_type value_type;                 /**< Shortcut to value_type */
  typedef typename base::item_type item_type;                   /**< Shortcut to item_type */
  typedef typename base::size_type size_type;                      /**< Shortcut to size_type */
  typedef typename base::relation_type relation_type;           /**< Shortcut to relation_type */

private:
  typedef typename base::container_iterator container_iterator; /**< Shortcut to container_iterator */

public:
  /**
   * @brief Creates an empty has_many object
   *
   * Creates an empty has_many object with a
   * std::list as container type
   */
  has_many() {}

  /**
   * @brief Inserts an element at the given position.
   *
   * @param pos The position to insert at
   * @param value The element to be inserted
   * @return The iterator at position of inserted element
   */
  iterator insert(iterator pos, const value_type &value)
  {
    // create new has_many
    item_type *item = this->create_item(value);
    relation_type iptr(item);
    if (this->ostore_) {
      this->ostore_->insert(iptr);
      this->mark_modified_owner_(*this->ostore_, this->owner_);
    }
    container_iterator i = pos.iter_;
    return iterator(this->container_.insert(i, iptr));
  }

  /**
   * @brief Inserts an element at first position.
   *
   * @param value The element to be inserted
   */
  void push_front(const value_type &value)
  {
    insert(this->begin(), value);
  }

  /**
   * @brief Inserts an element at last position.
   *
   * @param value The element to be inserted
   */
  void push_back(const value_type &value)
  {
    insert(this->end(), value);
  }

  /**
   * @brief Clears the list
   */
  void clear()
  {
    erase(this->begin(), this->end());
  }

  /**
   * @brief Erase the element at given position.
   *
   * Erase the element at given position and return the iterator
   * following the last removed element.
   * @param i Iterator to the element to remove
   * @return Iterator following the last removed element
   */
  iterator erase(iterator i)
  {
    if (this->ostore_) {
      relation_type iptr = i.relation_item();
      this->ostore_->remove(iptr);
    }
    container_iterator ci = this->container_.erase(i.iter_);
    return iterator(ci);
  }

  /**
   * @brief Remove the elements of given range.
   *
   * Remove the elements of the given range identified
   * by the first and last iterator position. Where the first
   * iterator is included and the last iterator is not included
   * [first; last)
   *
   * @param start First iterator of the range
   * @param end Last iterator of the range
   * @return Iterator following the last removed element
   */
  iterator erase(iterator start, iterator end)
  {
    iterator i = start;
    if (this->ostore_) {
      while (i != end) {
        typename base::relation_type iptr = (i++).relation_item();
        this->ostore_->remove(iptr);
      }
    }
    return iterator(this->container_.erase(start.iter_, end.iter_));
  }

private:
  item_type* create_item(const value_type &value)
  {
    return new item_type(this->owner_field_, this->item_field_, this->owner_id_, value);
  }
};

}
#endif //OOS_HAS_MANY_LIST_HPP
