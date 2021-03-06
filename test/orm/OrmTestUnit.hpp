//
// Created by sascha on 20.03.16.
//

#ifndef OOS_ORMTESTUNIT_HPP
#define OOS_ORMTESTUNIT_HPP


#include "oos/unit/unit_test.hpp"

class OrmTestUnit : public oos::unit_test
{
public:
  OrmTestUnit(const std::string &prefix, const std::string &dns);

  void test_create();
  void test_insert();
  void test_select();
  void test_update();
  void test_delete();
  void test_load();
  void test_load_has_one();
  void test_load_has_many();
  void test_load_has_many_int();
  void test_has_many_delete();

private:
  std::string dns_;
};


#endif //OOS_ORMTESTUNIT_HPP
