#include "../entities.hpp"
#include "PrimaryKeyTestUnit.hpp"

#include "matador/orm/persistence.hpp"
#include "matador/orm/session.hpp"

using namespace matador;

PrimaryKeyTestUnit::PrimaryKeyTestUnit(const std::string &prefix, std::string dns)
  : unit_test(prefix + "_pk", prefix + " primary key test unit")
  , dns_(std::move(dns))
{
  add_test("long", [this] { test_long_pk(); }, "test long primary key");
  add_test("string", [this] { test_string_pk(); }, "test string primary key");
  add_test("varchar", [this] { test_varchar_pk(); }, "test varchar primary key");
}

void PrimaryKeyTestUnit::test_long_pk()
{
  matador::persistence p(dns_);

  p.attach<child>("child");

  p.create();

  matador::session s(p);

  auto hans = new child("hans");
  auto optr = s.insert(hans);

  s.flush();

  matador::query<child> q("child");
  matador::connection c(dns_);
  c.connect();
  auto id_col = "id"_col;
  auto res = q.select().where(id_col == 1).execute(c);
  auto first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  std::unique_ptr<child> p1(first.release());

  UNIT_EXPECT_EQUAL(1UL, p1->id);

  optr.modify()->id = 2UL;

  s.flush();

  res = q.select().where(id_col == 2UL).execute(c);
  first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  p1.reset(first.release());

  UNIT_EXPECT_EQUAL(2UL, p1->id);

  p.drop();
}

struct user
{
  std::string email;
  std::string name;

  template < class S >
  void serialize(S &s)
  {
    s.on_primary_key("email", email, 255);
    s.on_attribute("name", name, 255);
  }
};

void PrimaryKeyTestUnit::test_string_pk()
{
  matador::persistence p(dns_);

  p.attach<user>("user");

  p.create();

  matador::session s(p);

  auto hans = new user{"hans@email.com", "hans"};
  auto optr = s.insert(hans);

  s.flush();

  matador::query<user> q("user");
  matador::connection c(dns_);
  c.connect();
  auto email_col = "email"_col;
  auto res = q.select().where(email_col == "hans@email.com").execute(c);
  auto first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  std::unique_ptr<user> p1(first.release());

  UNIT_EXPECT_EQUAL("hans@email.com", p1->email);

  optr.modify()->email = "hans@email.de";

  s.flush();

  res = q.select().where(email_col == "hans@email.de").execute(c);
  first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  p1.reset(first.release());

  UNIT_EXPECT_EQUAL("hans@email.de", p1->email);

  p.drop();
}

struct product
{
  std::string id;
  std::string name;

  template < class S >
  void serialize(S &s)
  {
    s.on_primary_key("id", id, 255);
    s.on_attribute("name", name, 255);
  }
};

void PrimaryKeyTestUnit::test_varchar_pk()
{
  matador::persistence p(dns_);

  p.attach<product>("product");

  p.create();

  matador::session s(p);

  auto p1 = new product{"p1", "super stuff"};
  auto optr = s.insert(p1);

  s.flush();

  matador::query<product> q("product");
  matador::connection c(dns_);
  c.connect();
  auto id_col = "id"_col;
  auto res = q.select().where(id_col == "p1").execute(c);
  auto first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  std::unique_ptr<product> pp(first.release());

  UNIT_EXPECT_EQUAL("p1", pp->id);

  optr.modify()->id = "p2";

  s.flush();

  res = q.select().where(id_col == "p2").execute(c);
  first = res.begin();

  UNIT_ASSERT_TRUE(first != res.end());
  pp.reset(first.release());

  UNIT_EXPECT_EQUAL("p2", pp->id);

  p.drop();

}
