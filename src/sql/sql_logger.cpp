#include "matador/sql/sql_logger.hpp"

#include "matador/logger/log_manager.hpp"

namespace matador {

sql_logger::sql_logger()
  : LOG(create_logger("sql"))
{
  add_log_sink(mk_rotating_file_sink("log/sql.log", 1000000, 5));
  add_log_sink(mk_stdout_sink());
}

void sql_logger::on_execute(const std::string &stmt)
{
  LOG.info("execute: " + stmt);
}

void sql_logger::on_prepare(const std::string &stmt)
{
  LOG.info("prepare: " + stmt);
}
}