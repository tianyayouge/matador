#include "matador/sql/result_impl.hpp"

#include "matador/utils/identifiable_holder.hpp"

namespace matador {
namespace detail {

result_impl::result_impl()
: result_identifier_reader_(*this)
{}

void result_impl::on_attribute(const char *id, char *value, long size)
{
  read_value(id, result_index_++, result_row_, value, size);
}

void result_impl::on_attribute(const char *id, std::string &value, long size)
{
  read_value(id, result_index_++, result_row_, value, size);
}

void result_impl::on_attribute(const char *id, matador::time &value, long /*size*/)
{
  read_value(id, result_index_++, result_row_, value);
}

void result_impl::on_attribute(const char *id, matador::date &value, long /*size*/)
{
  read_value(id, result_index_++, result_row_, value);
}

void result_impl::on_belongs_to(const char *id, matador::identifiable_holder &x, cascade_type)
{
  read_foreign_object(id, x);
}

void result_impl::on_has_one(const char *id, matador::identifiable_holder &x, cascade_type)
{
  read_foreign_object(id, x);
}


void result_impl::read_foreign_object(const char */*id*/, identifiable_holder &x)
{
  //determine and create primary key of object ptr
  auto pk = x.primary_key();
  if (pk.is_null()) {
    pk = x.create_identifier();
  }

  // Todo: handle serialization
  pk.serialize(result_identifier_reader_);
//  pk->serialize(id, *this);
  if (!pk.is_valid()) {
    // no pk is set => null
    pk.clear();
    x.reset(pk);
    return;
  }

  // set found primary key into object_base_ptr
  if (!x.has_primary_key()) {
    x.reset(pk);
  }
}

void result_identifier_reader::serialize(null_type_t &) {

}
}
}