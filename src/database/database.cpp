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

#include "database/database.hpp"
#include "database/action.hpp"
#include "database/transaction.hpp"

#include "object/object_store.hpp"

#include <iostream>

using namespace std;

namespace oos {

database_impl::database_impl()
{}

database_impl::~database_impl()
{}

void database_impl::visit(insert_action *)
{
}

void database_impl::visit(update_action *)
{
}

void database_impl::visit(delete_action *)
{
}

transaction_impl* database_impl::create_transaction(transaction &tr) const
{
  return new transaction_impl(tr);
}

database::database(object_store &ostore, const std::string &/*dbstring*/)
  : ostore_(ostore)
{
  // create new database implementation instance
  impl_ = new database_impl;
  // initialize database: create prepared insert
  // update and delete statements
  /*
  object_store::prototype_iterator first == ostore.first_prototype();
  object_store::prototype_iterator last == ostore.last_prototype();
  while (first != last) {
    impl_->prepare(*first++);
  }
  */
}


database::~database()
{
  delete impl_;
}

void database::open()
{
}

void database::create()
{
}

void database::close()
{
}

query_result* database::query(const std::string &)
{
  return NULL;
}

object_store& database::ostore()
{
  return ostore_;
}

const object_store& database::ostore() const
{
  return ostore_;
}

void database::push_transaction(transaction *tr)
{
  if (!transaction_stack_.empty()) {
//    cout << "unregister transaction observer [" << transaction_stack_.top()->transaction_observer_.get() << "]\n";
    ostore_.unregister_observer(transaction_stack_.top()->impl_);
  }
  transaction_stack_.push(tr);
//  cout << "register transaction observer [" << tr->transaction_observer_.get() << "]\n";
  ostore_.register_observer(tr->impl_);
}

void database::pop_transaction()
{
//  cout << "unregister transaction observer [" << transaction_stack_.top()->transaction_observer_.get() << "]\n";
  ostore_.unregister_observer(transaction_stack_.top()->impl_);
  transaction_stack_.pop();
  if (!transaction_stack_.empty()) {
//    cout << "register transaction observer [" << transaction_stack_.top()->transaction_observer_.get() << "]\n";
    ostore_.register_observer(transaction_stack_.top()->impl_);
  }
}

transaction* database::current_transaction() const
{
  return (transaction_stack_.empty() ? 0 : transaction_stack_.top());
}

}
