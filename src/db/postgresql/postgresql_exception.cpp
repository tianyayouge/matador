//
// Created by sascha on 24.05.19.
//

#include "matador/sql/database_error.hpp"

#include <sqlca.h>

#include "matador/db/postgresql/postgresql_exception.hpp"

namespace matador {

namespace postgresql {

void throw_database_error(PGresult *res, PGconn *db, const std::string &source, const std::string &sql)
{
  if (res == nullptr ||
      (PQresultStatus(res) != PGRES_COMMAND_OK &&
       PQresultStatus(res) != PGRES_TUPLES_OK)) {
    database_error(PQerrorMessage(db), source, sqlca.sqlstate, sql);
  }
}

}
}