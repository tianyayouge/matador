#ifndef MATADOR_JSON_SERIALIZER_HPP
#define MATADOR_JSON_SERIALIZER_HPP

#ifdef _MSC_VER
#ifdef matador_utils_EXPORTS
#define OOS_UTILS_API __declspec(dllexport)
#define EXPIMP_UTILS_TEMPLATE
#else
#define OOS_UTILS_API __declspec(dllimport)
#define EXPIMP_UTILS_TEMPLATE extern
#endif
#pragma warning(disable: 4251)
#else
#define OOS_UTILS_API
#endif

#include "matador/utils/access.hpp"
#include "matador/utils/identifier.hpp"
#include "matador/utils/string.hpp"
#include "matador/utils/json.hpp"
#include "matador/utils/is_builtin.hpp"

#include <set>
#include <unordered_set>
#include <sstream>

namespace matador {

struct OOS_UTILS_API json_format
{
public:
  json_format() = default;
  explicit json_format(bool enable_line_break);
  explicit json_format(unsigned indentation);
  json_format(bool enable_line_break, bool skip_empty, unsigned indentation);

  bool show_line_break() const;

  bool skip_empty();

  unsigned indentation() const;

  static json_format compact;
  static json_format pretty;

private:
  bool enable_line_break_ = true;
  bool skip_empty_ = true;
  unsigned indentation_ = 2;
};

class OOS_UTILS_API json_serializer
{
public:

  json_serializer() = default;

  explicit json_serializer(json_format format);

  template < class T >
  std::string to_json(const T &obj)
  {
    json_.clear();
    append(obj);
    newline();
    //serialize(obj);
    return json_;
  }

  template < class V >
  void serialize(V &obj, typename std::enable_if<!matador::is_builtin<V>::value>::type* = 0)
  {
    matador::access::serialize(*this, obj);
  }

  template< class V >
  void serialize(const char *id, identifier<V> &pk, typename std::enable_if<std::is_integral<V>::value && !std::is_same<bool, V>::value>::type* = 0)
  {
    write_id(id);
    append(pk).append(",");
    newline();
  }

  template < class V >
  void serialize(const char *id, V &obj, typename std::enable_if<!matador::is_builtin<V>::value>::type* = 0)
  {
    write_id(id);
    append(obj);
    newline();
  }

  void serialize(const char *id, identifier<std::string> &pk);

  template < int SIZE, class V >
  void serialize(const char *id, identifier<varchar<SIZE, V>> &pk)
  {
    write_id(id);
    append(pk.value()).append(",");
    newline();
  }

  // numbers
  template < class V >
  void serialize(const char *id, V &val, typename std::enable_if<std::is_arithmetic<V>::value && !std::is_same<V, bool>::value>::type* = 0)
  {
    write_id(id);
    append(val).append(",");
    newline();
  }

  // boolean
  void serialize(const char *id, bool &val);

  // string
  void serialize(const char *id, std::string &val);

  // string
  void serialize(const char *id, std::string &val, size_t);

  // date
  void serialize(const char *id, date &d);

  // time
  void serialize(const char *id, time &t);

  template < class V >
  void serialize(const char *id, std::list<V> &cont, typename std::enable_if<!std::is_class<V>::value>::type* = 0)
  {
    write_id(id);
    json_.append("[");
    join(cont, ", ").append("],");
    newline();
  }
  template < class V >
  void serialize(const char *id, std::list<V> &cont, typename std::enable_if<std::is_class<V>::value>::type* = 0)
  {
    write_id(id);
    json_.append("[");
    join(cont, "");
    auto idx = json_.find_last_of(',');
    json_.erase(idx);
    json_.append("],");
    newline();
  }

  template < class V >
  void serialize(const char *id, std::vector<V> &cont, typename std::enable_if<!std::is_class<V>::value>::type* = 0)
  {
    write_id(id);
    json_.append("[");
    join(cont, ", ").append("],");
    newline();
  }
  template < class V >
  void serialize(const char *id, std::vector<V> &cont, typename std::enable_if<std::is_class<V>::value>::type* = 0)
  {
    write_id(id);
    json_.append("[");
    join(cont, "");
    auto idx = json_.find_last_of(',');
    json_.erase(idx);
    json_.append("],");
    newline();
  }

  template < class V >
  void serialize(const char *id, std::set<V> &cont)
  {
    write_id(id);
    json_.append("[");
    join(cont, ", ").append("],\n");
  }

  template < class V >
  void serialize(const char *id, std::unordered_set<V> &cont)
  {
    write_id(id);
    json_.append("[");
    join(cont, ", ").append("],");
    newline();
  }

private:
  std::string& write_id(const char *id);

  template < class R >
  std::string& join(R &range, const std::string &delim)
  {
    if (range.size() < 2) {
      for (const auto &obj : range) {
        append(obj);
      }
    } else {
      auto it = range.begin();
      append(*it++);
      for (;it != range.end(); ++it) {
        json_.append(delim);
        append(*it);
      }
    }
    return json_;
  }

  void begin_object();

  void end_object();

  void indent();

  void newline();

  std::string& append(const char *str);

  std::string& append(const std::string &str);

  std::string& append(std::string &val, size_t);

  std::string& append(const bool &value);

  template<class V>
  std::string& append(const V &value, typename std::enable_if<std::is_arithmetic<V>::value && !std::is_same<V, bool>::value>::type* = 0)
  {
    json_.append(std::to_string(value));
    return json_;
  }

  template < class V >
  std::string& append(V &obj, typename std::enable_if<!matador::is_builtin<V>::value>::type* = 0)
  {
    begin_object();
    matador::access::serialize(*this, obj);
    auto idx = json_.find_last_of(',');
    json_.erase(idx, 1);
    end_object();
    return json_;
  }

private:
  std::string json_;
  json_format format_ {};
  unsigned depth_ = 0;
};

}
#endif //MATADOR_JSON_SERIALIZER_HPP
