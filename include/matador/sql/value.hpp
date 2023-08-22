#ifndef VALUE_HPP
#define VALUE_HPP

#include "matador/sql/token.hpp"
#include "matador/sql/basic_dialect.hpp"
#include "matador/sql/value_processor.hpp"

#include "matador/utils/date.hpp"
#include "matador/utils/time.hpp"
#include "matador/utils/string.hpp"
#include "matador/utils/serializer.hpp"
#include "matador/utils/any.hpp"

#include <string>
#include <typeinfo>
#include <cstring>
#include <type_traits>

namespace matador {

/// @cond MATADOR_DEV

struct value : public detail::token
{
  template<typename T, typename U = std::decay<T>>
  explicit value(T &&val)
    : token(detail::token::VALUE)
    , value_(val)
  {}

  template<typename T, typename U = std::decay<T>>
  value(T &&val, std::size_t s)
    : token(detail::token::VALUE)
    , value_(val)
    , size_(s)
  {}

  template < class T > T get() {
    return value_._<T>();
  }

  void serialize(const char *id, serializer &srlzr);

  void accept(token_visitor &visitor) override;

  std::string str() const;
  std::string safe_string(const basic_dialect &) const;

  virtual const char* type_id() const;

  any value_;
  std::size_t size_ = 0;
  detail::value_processor value_visitor_;
  detail::value_to_string_visitor value_to_string_visitor_;
};

struct null_value : public value
{
  static std::string NULLSTR;

  null_value() : value(NULLSTR) { }

  const char* type_id() const override;
};

template < class T >
value* make_value(const T &val)
{
  return new value(val);
}

value* make_value(const char* val, std::size_t len);

/// @endcond

}

#endif /* VALUE_HPP */
