//
// Created by sascha on 6/2/17.
//

#ifndef MATADOR_RELATION_ENDPOINT_VALUE_REMOVER_HPP
#define MATADOR_RELATION_ENDPOINT_VALUE_REMOVER_HPP

#include <iostream>

namespace matador {
namespace detail {

template < class Value >
class relation_endpoint_value_remover
{
public:
  template < class Owner >
  void remove(const object_ptr<Owner> &owner, const std::string &field, const object_ptr<Value> &value, object_proxy *item_proxy)
  {
    field_ = field;
    value_ = value;
    item_proxy_ = item_proxy;

    matador::access::serialize(*this, *owner);

    item_proxy_ = nullptr;
    field_.clear();
    value_.reset(nullptr, value_.cascade_, false);
  }

  template < class T >
  void serialize(T &x)
  {
    matador::access::serialize(*this, x);
  }

  template < class T >
  void serialize(const char *, T &) {}
  void serialize(const char *, char *, size_t) {}
  void serialize(const char *id, object_holder &x, cascade_type);
  template < template < class ... > class Container >
  void serialize(const char *id, has_many<Value, Container> &, const char*, const char*);

private:
  std::string field_;
  matador::object_ptr<Value> value_;
  object_proxy *item_proxy_ = nullptr; // only set if holder type is HAS_MANY and foreign type is also HAS_MANY
};

}
}
#endif //MATADOR_RELATION_ENDPOINT_VALUE_REMOVER_HPP
