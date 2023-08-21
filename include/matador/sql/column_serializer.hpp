#ifndef OOS_COLUMN_SERIALIZER_HPP
#define OOS_COLUMN_SERIALIZER_HPP

#include "matador/sql/sql.hpp"
#include "matador/sql/column.hpp"

#include "matador/utils/access.hpp"
#include "matador/utils/field_attributes.hpp"
#include "matador/utils/cascade_type.hpp"

namespace matador {


namespace detail {

/// @cond MATADOR_DEV

class column_serializer
{
public:
  explicit column_serializer(columns::t_brackets brackets);

  template<class T>
  columns *execute(T &x) {
    cols_ = std::make_unique<columns>(brackets_);
    matador::access::serialize(*this, x);
    return cols_.release();
  }

  template < class T >
  void serialize(T &x)
  {
    matador::access::serialize(*this, x);
  }

  void on_attribute(const char *id, char &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, short &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, int &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, long &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, long long &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, unsigned char &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, unsigned short &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, unsigned int &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, unsigned long &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, unsigned long long &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, float &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, double &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, bool &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, char *x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, std::string &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, date &x, const field_attributes &/*attr*/ = {});
  void on_attribute(const char *id, time &x, const field_attributes &/*attr*/ = {});
  void on_belongs_to(const char *id, identifiable_holder &x, cascade_type);
  void on_has_one(const char *id, identifiable_holder &x, cascade_type);

  template<typename ValueType>
  void on_primary_key(const char *id, ValueType &, const field_attributes &/*attr*/ = {})
  {
    cols_->push_back(std::make_shared<column>(id));
  }
  void on_has_many(const char *, abstract_has_many &, const char *, const char *, cascade_type) {}
  void on_has_many(const char *, abstract_has_many &, cascade_type) {}

private:
  columns::t_brackets brackets_;
  std::unique_ptr<columns> cols_;
};

/// @endcond

}
}

#endif //OOS_COLUMN_SERIALIZER_HPP
