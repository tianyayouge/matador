//
// Created by sascha on 29.05.19.
//

#include <cstring>

#include "matador/utils/string.hpp"
#include "matador/utils/date.hpp"
#include "matador/utils/time.hpp"
#include "matador/utils/basic_identifier.hpp"

#include "matador/db/postgresql/postgresql_result.hpp"

namespace matador {
namespace postgresql {

postgresql_result::postgresql_result(PGresult *res)
  : affected_rows_(0)
  , rows_(PQntuples(res))
  , fields_(PQnfields(res))
  , res_(res)
{
}

postgresql_result::~postgresql_result()
{
  if (res_) {
    PQclear(res_);
  }
}

const char *postgresql_result::column(postgresql_result::size_type c) const
{
  return nullptr;
}

bool postgresql_result::fetch()
{
  return ++pos_ < rows_;
}

postgresql_result::size_type postgresql_result::affected_rows() const
{
  return affected_rows_;
}

postgresql_result::size_type postgresql_result::result_rows() const
{
  return rows_;
}

postgresql_result::size_type postgresql_result::fields() const
{
  return fields_;
}

int postgresql_result::transform_index(int index) const
{
  return index;
}

void postgresql_result::serialize(const char *, char &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) > 1) {
    x = val[0];
  }
}

void postgresql_result::serialize(const char *, short &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (short)strtol(val, &end, 10);
}

void postgresql_result::serialize(const char *, int &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (int)strtol(val, &end, 10);
}

void postgresql_result::serialize(const char *, long &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtol(val, &end, 10);
}

void postgresql_result::serialize(const char *, unsigned char &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned char)strtoul(val, &end, 10);
  // Todo: check error
}

void postgresql_result::serialize(const char *, unsigned short &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned short)strtoul(val, &end, 10);
  // Todo: check error
}

void postgresql_result::serialize(const char *, unsigned int &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned int)strtoul(val, &end, 10);
  // Todo: check error
}

void postgresql_result::serialize(const char *, unsigned long &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtoul(val, &end, 10);
  // Todo: check error
}

void postgresql_result::serialize(const char *, bool &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned char)strtoul(val, &end, 10) > 0;
  // Todo: check error
}

void postgresql_result::serialize(const char *, float &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (float)strtod(val, &end);
  // Todo: check error
}

void postgresql_result::serialize(const char *, double &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtod(val, &end);
  // Todo: check error
}

void postgresql_result::serialize(const char *, char *x, size_t s)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  size_t len = strlen(val);
  if (len > (size_t)s) {
    strncpy(x, val, s);
    x[s-1] = '\n';
  } else {
    strcpy(x, val);
  }
}

void postgresql_result::serialize(const char *, varchar_base &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  x.assign(val);
}

void postgresql_result::serialize(const char *, std::string &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  x = val;
}

void postgresql_result::serialize(const char *, matador::date &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  x.set(val, date_format::ISO8601);
}

void postgresql_result::serialize(const char *, matador::time &x)
{
  auto val = PQgetvalue(res_, pos_, column_++);

  x = matador::time::parse(val, "%Y-%m-%d %T.%f");
}

void postgresql_result::serialize(const char *id, matador::basic_identifier &x)
{
  x.serialize(id, *this);
}

void postgresql_result::serialize(const char *id, matador::identifiable_holder &x, cascade_type)
{
  read_foreign_object(id, x);
}

bool postgresql_result::prepare_fetch()
{
  column_ = 0;
  return pos_ + 1 <= rows_;
}

bool postgresql_result::finalize_fetch()
{
  ++pos_;
  return true;
}
}
}