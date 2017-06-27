#include "matador/object/relation_field_endpoint.hpp"
#include "matador/object/object_store.hpp"

namespace matador {
namespace detail {

template<class T>
void basic_relation_endpoint::set_has_many_item_proxy(has_many_item_holder<T> &holder, const object_holder &obj)
{
  set_has_many_item_proxy(holder, obj.proxy_);
}

template<class T>
void basic_relation_endpoint::set_has_many_item_proxy(has_many_item_holder<T> &holder, object_proxy *proxy)
{
  holder.has_many_to_many_item_poxy_ = proxy;
}

template < class Value >
void basic_relation_endpoint::insert_value_into_foreign(const has_many_item_holder<Value> &holder, object_proxy *owner)
{
  insert_value_into_foreign(holder.value().proxy_, owner, holder.item_proxy());
}

template < class Value >
void basic_relation_endpoint::remove_value_from_foreign(const has_many_item_holder<Value> &holder, object_proxy *owner)
{
  remove_value_from_foreign(holder.value().proxy_, owner, holder.item_proxy());
}

template < class Value, class Owner, basic_relation_endpoint::relation_type Type >
void from_one_endpoint<Value, Owner, Type>::insert_holder(object_store &/*store*/, has_many_item_holder<Value> &holder, object_proxy *owner)
{
  this->set_has_many_item_proxy(holder, owner);
}

template < class Value, class Owner, basic_relation_endpoint::relation_type Type >
void from_one_endpoint<Value, Owner, Type>::remove_holder(object_store &/*store*/, has_many_item_holder<Value> &holder, object_proxy */*owner*/)
{
  this->set_has_many_item_proxy(holder, nullptr);
}

template < class Value, class Owner, basic_relation_endpoint::relation_type Type>
void from_one_endpoint<Value, Owner, Type>::insert_value(object_proxy *value, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Owner> ownptr(owner);
  inserter.insert(ownptr, this->field, value, item_proxy);
}

template < class Value, class Owner, basic_relation_endpoint::relation_type Type>
void from_one_endpoint<Value, Owner, Type>::remove_value(object_proxy *, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Owner> ownptr(owner);
  object_ptr<Value> valptr;
  remover.remove(ownptr, this->field, valptr, item_proxy);
}

template < class Value, class Owner >
void belongs_to_many_endpoint<Value, Owner>::insert_holder(object_store &, has_many_item_holder <Value> &holder, object_proxy *owner)
{
  this->set_has_many_item_proxy(holder, owner);
}

template < class Value, class Owner >
void belongs_to_many_endpoint<Value, Owner>::remove_holder(object_store &, has_many_item_holder <Value> &holder, object_proxy */*owner*/)
{
  this->set_has_many_item_proxy(holder, nullptr);
}

template < class Value, class Owner >
void belongs_to_many_endpoint<Value, Owner>::insert_value(object_proxy *value, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Owner> ownptr(value);
  inserter.insert(ownptr, this->field, owner, item_proxy);
}

template < class Value, class Owner >
void belongs_to_many_endpoint<Value, Owner>::remove_value(object_proxy *value, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Owner> ownptr(value);
  object_ptr<Value> valptr(owner);
  remover.remove(ownptr, this->field, valptr, item_proxy);
}

template < class Value, class Owner >
void many_to_one_endpoint<Value, Owner, typename std::enable_if<!std::is_base_of<basic_has_many_to_many_item, Value>::value>::type>::insert_holder(object_store &/*store*/, has_many_item_holder<Value> &holder, object_proxy *owner)
{
  this->set_has_many_item_proxy(holder, owner);
}

template < class Value, class Owner >
void many_to_one_endpoint<Value, Owner, typename std::enable_if<!std::is_base_of<basic_has_many_to_many_item, Value>::value>::type>::remove_holder(object_store &/*store*/, has_many_item_holder<Value> &holder, object_proxy */*owner*/)
{
  this->set_has_many_item_proxy(holder, nullptr);
}

template < class Value, class Owner >
void many_to_one_endpoint<Value, Owner, typename std::enable_if<!std::is_base_of<basic_has_many_to_many_item, Value>::value>::type>::insert_value(object_proxy *value, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Owner> ownptr(owner);
  inserter.insert(ownptr, this->field, value, item_proxy);
}

template < class Value, class Owner >
void many_to_one_endpoint<Value, Owner, typename std::enable_if<!std::is_base_of<basic_has_many_to_many_item, Value>::value>::type>::remove_value(object_proxy *value, object_proxy *owner, object_proxy *item_proxy)
{
  object_ptr<Value> valptr(value);
  object_ptr<Owner> ownptr(owner);
  remover.remove(ownptr, this->field, valptr, item_proxy);
}

}
}