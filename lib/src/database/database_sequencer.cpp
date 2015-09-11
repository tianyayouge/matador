/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#include "database/database_sequencer.hpp"
#include "database/database_exception.hpp"
#include "database/query.hpp"
#include "database/statement.hpp"

#include "object/generic_access.hpp"

namespace oos {

database_sequencer::database_sequencer(database &db)
  : db_(db)
  , backup_(0)
  , sequence_(0)
  , name_("serializable")
{}

database_sequencer::~database_sequencer()
{
  destroy();
}

void database_sequencer::deserialize(object_reader &r)
{
  r.read("name", name_);
  r.read("number", sequence_);
}

void database_sequencer::serialize(object_writer &w) const
{
  w.write("name", name_);
  w.write("number", sequence_);
}

unsigned long database_sequencer::init()
{
  return sequence_;
}

unsigned long database_sequencer::reset(unsigned long id)
{
  sequence_ = id;
  return sequence_;
}

unsigned long database_sequencer::next()
{
  return ++sequence_;
}

unsigned long database_sequencer::current() const
{
  return sequence_;
}

unsigned long database_sequencer::update(unsigned long id)
{
  if (id > sequence_) {
    sequence_ = id;
  }
  return sequence_;
}

void database_sequencer::create()
{
  query q(db_);
  result res = q.create("oos_sequence", this).execute();
  
  res = q.reset().select(this).from("oos_sequence").where("name='serializable'").execute();


  result_iterator first = res.begin();

  if (first != res.end()) {
    oos::get(first.get(), "number", sequence_);
  } else {
    // TODO: check result
    q.reset().insert(this, "oos_sequence").execute();
  }

  update_.reset(q.reset().update("oos_sequence", this).where("name='serializable'").prepare());
}

void database_sequencer::load()
{
  query q(db_);
  result res = q.reset().select(this).from("oos_sequence").where("name='serializable'").execute();

  result_iterator first = res.begin();

  if (first != res.end()) {
    oos::get(first.get(), "number", sequence_);
  } else {
    throw database_exception("database::sequencer", "couldn't fetch sequence");
  }

  if (!update_) {
    update_.reset(q.reset().update("oos_sequence", this).where("name='serializable'").prepare());
  }
}

void database_sequencer::begin()
{
  // backup current sequence id from serializable store
  backup_ = current();
}

void database_sequencer::commit()
{
  update_->bind(this);
  // TODO: check result
  update_->execute();
  update_->reset();
}

void database_sequencer::rollback()
{
  reset(backup_);
}

void database_sequencer::drop()
{
  query q(db_);
  // TODO: check result
  q.drop("oos_sequence").execute();
}

void database_sequencer::destroy()
{
}


}
