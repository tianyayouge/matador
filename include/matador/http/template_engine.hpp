#ifndef MATADOR_TEMPLATE_ENGINE_HPP
#define MATADOR_TEMPLATE_ENGINE_HPP

#include "matador/utils/string_cursor.hpp"

#include <string>
#include <stack>
#include <memory>

namespace matador {

class json;

namespace http {

namespace detail {

class template_state;

}

class template_engine
{
public:
  void render(const std::string &format, const matador::json &data);

  void render(const char *format, size_t len, const matador::json &data);

  const std::string& str() const;

private:
  void handle_variable(const json &data);
  void handle_command(const json &data);

private:
  std::string rendered_;

  string_cursor cursor_;

  std::stack<std::shared_ptr<detail::template_state>> state_stack_;
};

}
}
#endif //MATADOR_TEMPLATE_ENGINE_HPP
