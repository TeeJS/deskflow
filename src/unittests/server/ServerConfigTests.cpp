/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Chris Rizzitello <sithlord48@gmail.com>
 * SPDX-FileCopyrightText: (C) 2014 - 2016 Synergy App Ltd
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "ServerConfigTests.h"

#include "deskflow/MonitorTypes.h"
#include "server/Config.h"

#include <sstream>

class OnlySystemFilter : public InputFilter::Condition
{
public:
  Condition *clone() const override
  {
    return new OnlySystemFilter();
  }
  std::string format() const override
  {
    return "";
  }

  InputFilter::FilterStatus match(const Event &ev) override
  {
    return ev.getType() == EventTypes::System ? InputFilter::FilterStatus::Activate
                                              : InputFilter::FilterStatus::NoMatch;
  }
};

using namespace deskflow::server;

void ServerConfigTests::equalityCheck()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(a != b);

  QVERIFY(b.addComputer("computerB"));
  QVERIFY(a != b);

  QVERIFY(a.addComputer("computerB"));
  QVERIFY(a.addComputer("computerC"));
  QVERIFY(a.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(a.connect("computerB", Direction::Left, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(b.addComputer("computerC"));
  QVERIFY(b.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(b.connect("computerB", Direction::Left, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(a.addOption("computerA", kOptionClipboardSharing, 1));
  QVERIFY(b.addOption("computerA", kOptionClipboardSharing, 1));
  QVERIFY(a.addOption(std::string(), kOptionClipboardSharing, 1));
  QVERIFY(b.addOption(std::string(), kOptionClipboardSharing, 1));

  a.getInputFilter()->addFilterRule(InputFilter::Rule{new OnlySystemFilter()});
  b.getInputFilter()->addFilterRule(InputFilter::Rule{new OnlySystemFilter()});
  QVERIFY(a.addAlias("computerA", "aliasA"));
  QVERIFY(b.addAlias("computerA", "aliasA"));
  /* TODO Fix linking to the proper libs
  NetworkAddress addr1("localhost", 8080);
  addr1.resolve();
  NetworkAddress addr2("localhost", 8080);
  addr2.resolve();
  a.setDeskflowAddress(addr1);
  b.setDeskflowAddress(addr2);
  */
  QVERIFY(a == b);
}

void ServerConfigTests::equalityCheck_diff_options()
{
  Config a(nullptr);
  Config b(nullptr);

  QVERIFY(a.addComputer("computerA"));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(a.addOption("computerA", kOptionClipboardSharing, 0));
  QVERIFY(b.addOption("computerA", kOptionClipboardSharing, 1));
  QVERIFY(a != b);
}

void ServerConfigTests::equalityCheck_diff_alias()
{
  Config a(nullptr);
  Config b(nullptr);

  QVERIFY(a.addComputer("computerA"));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(b.addAlias("computerA", "aliasA"));
  QVERIFY(a != b);

  QVERIFY(a.addAlias("computerA", "aliasA"));
  QVERIFY(b.addAlias("computerA", "aliasB"));
  QVERIFY(a != b);
}

void ServerConfigTests::equalityCheck_diff_filters()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(b.addComputer("computerA"));

  a.getInputFilter()->addFilterRule(InputFilter::Rule{new OnlySystemFilter()});
  QVERIFY(a != b);
}

// TODO FIX
/*
void ServerConfigTests::equalityCheck_diff_address()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(b.addComputer("computerA"));
  a.setDeskflowAddress(NetworkAddress(8000));
  b.setDeskflowAddress(NetworkAddress(9000));
  QVERIFY(a != b);
}
*/

void ServerConfigTests::equalityCheck_diff_neighbours1()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(a.addComputer("computerB"));
  QVERIFY(a.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(b.addComputer("computerB"));
  QVERIFY(a != b);
  QVERIFY(b != a);
}

void ServerConfigTests::equalityCheck_diff_neighbours2()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(a.addComputer("computerB"));
  QVERIFY(a.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(b.addComputer("computerB"));
  QVERIFY(b.connect("computerA", Direction::Bottom, 0.0f, 0.25f, "computerB", 0.25f, 1.0f));
  QVERIFY(a != b);
}

void ServerConfigTests::equalityCheck_diff_neighbours3()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("computerA"));
  QVERIFY(a.addComputer("computerB"));
  QVERIFY(a.addComputer("computerC"));
  QVERIFY(a.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerB", 0.5f, 1.0f));
  QVERIFY(b.addComputer("computerA"));
  QVERIFY(b.addComputer("computerB"));
  QVERIFY(b.addComputer("computerC"));
  QVERIFY(b.connect("computerA", Direction::Bottom, 0.0f, 0.5f, "computerC", 0.5f, 1.0f));
  QVERIFY(a != b);
}

namespace {

// an L-shaped server, in monitor list order (top to bottom, left to right):
//   1: top left 0,0 2560x1440   2: top right 2560,0 2560x1440   3: bottom left 0,1440 1920x1080
deskflow::MonitorList lLayout()
{
  return {
      {"a", 0, 0, 2560, 1440},
      {"b", 2560, 0, 2560, 1440},
      {"c", 0, 1440, 1920, 1080},
  };
}

// a server and a client, with the client beside the server's monitor 1
Config serverWithClientLeftOfMonitor1()
{
  Config c(nullptr);
  c.addComputer("server");
  c.addComputer("client");
  c.addMonitorLink({"server", Direction::Left, 1, {0.0f, 1.0f}, "client", 0, {0.0f, 1.0f}});
  return c;
}

} // namespace

void ServerConfigTests::equalityCheck_diff_monitorLinks()
{
  Config a(nullptr);
  Config b(nullptr);
  QVERIFY(a.addComputer("server"));
  QVERIFY(a.addComputer("client"));
  QVERIFY(b.addComputer("server"));
  QVERIFY(b.addComputer("client"));
  QVERIFY(a == b);
  QVERIFY(a.addMonitorLink({"server", Direction::Left, 1, {0.0f, 1.0f}, "client", 0, {0.0f, 1.0f}}));
  QVERIFY(a != b);
}

void ServerConfigTests::addMonitorLink_unknownComputer_false()
{
  Config c(nullptr);
  QVERIFY(c.addComputer("server"));
  QVERIFY(!c.addMonitorLink({"server", Direction::Left, 1, {0.0f, 1.0f}, "nobody", 0, {0.0f, 1.0f}}));
}

void ServerConfigTests::addMonitorLink_noMonitorOnEitherEnd_false()
{
  Config c(nullptr);
  QVERIFY(c.addComputer("server"));
  QVERIFY(c.addComputer("client"));
  QVERIFY(!c.addMonitorLink({"server", Direction::Left, 0, {0.0f, 1.0f}, "client", 0, {0.0f, 1.0f}}));
}

void ServerConfigTests::resolveMonitorLinks_leaveLinkedMonitor_neighborFound()
{
  Config c = serverWithClientLeftOfMonitor1();
  QCOMPARE(c.resolveMonitorLinks("server", lLayout()), size_t{1});
  // a quarter of the way down the left edge is on monitor 1
  QVERIFY(c.getNeighbor("server", Direction::Left, 0.25f, nullptr) == "client");
}

void ServerConfigTests::resolveMonitorLinks_leaveUnlinkedMonitor_noNeighbor()
{
  Config c = serverWithClientLeftOfMonitor1();
  c.resolveMonitorLinks("server", lLayout());
  // three quarters of the way down the left edge is on monitor 3
  QVERIFY(c.getNeighbor("server", Direction::Left, 0.75f, nullptr).empty());
}

void ServerConfigTests::resolveMonitorLinks_enterLinkedMonitor_positionInsideMonitorSpan()
{
  Config c(nullptr);
  QVERIFY(c.addComputer("server"));
  QVERIFY(c.addComputer("client"));
  QVERIFY(c.addMonitorLink({"client", Direction::Right, 0, {0.0f, 1.0f}, "server", 1, {0.0f, 1.0f}}));
  QCOMPARE(c.resolveMonitorLinks("server", lLayout()), size_t{1});
  float position = -1.0f;
  QVERIFY(c.getNeighbor("client", Direction::Right, 0.5f, &position) == "server");
  // halfway down the client lands halfway down monitor 1, which is the top 1440 of 2520 pixels
  QVERIFY(qFuzzyCompare(position, 0.5f * 1440.0f / 2520.0f));
}

void ServerConfigTests::resolveMonitorLinks_monitorOutOfRange_skipped()
{
  Config c(nullptr);
  QVERIFY(c.addComputer("server"));
  QVERIFY(c.addComputer("client"));
  QVERIFY(c.addMonitorLink({"server", Direction::Left, 4, {0.0f, 1.0f}, "client", 0, {0.0f, 1.0f}}));
  QCOMPARE(c.resolveMonitorLinks("server", lLayout()), size_t{0});
}

void ServerConfigTests::resolveMonitorLinks_monitorOnOtherComputer_skipped()
{
  Config c(nullptr);
  QVERIFY(c.addComputer("server"));
  QVERIFY(c.addComputer("client"));
  QVERIFY(c.addMonitorLink({"client", Direction::Left, 1, {0.0f, 1.0f}, "server", 0, {0.0f, 1.0f}}));
  QCOMPARE(c.resolveMonitorLinks("server", lLayout()), size_t{0});
}

void ServerConfigTests::resolveMonitorLinks_overlapsConfiguredLink_skipped()
{
  Config c = serverWithClientLeftOfMonitor1();
  QVERIFY(c.connect("server", Direction::Left, 0.0f, 1.0f, "client", 0.0f, 1.0f));
  QCOMPARE(c.resolveMonitorLinks("server", lLayout()), size_t{0});
}

void ServerConfigTests::resolveMonitorLinks_calledTwice_replacesOldLinks()
{
  Config c = serverWithClientLeftOfMonitor1();
  c.resolveMonitorLinks("server", lLayout());
  // now monitor 1 is only the top quarter of the left edge
  c.resolveMonitorLinks("server", {{"a", 0, 0, 100, 50}, {"b", 0, 50, 100, 150}});
  QVERIFY(c.getNeighbor("server", Direction::Left, 0.4f, nullptr).empty());
}

void ServerConfigTests::hasNeighbor_resolvedMonitorLink_true()
{
  Config c = serverWithClientLeftOfMonitor1();
  c.resolveMonitorLinks("server", lLayout());
  QVERIFY(c.hasNeighbor("server", Direction::Left));
}

void ServerConfigTests::parseMonitorSuffix_withMonitor_splitsNameAndIndex()
{
  std::string name = "server@3";
  int monitor = 0;
  QVERIFY(Config::parseMonitorSuffix(name, monitor));
  QCOMPARE(std::make_pair(QString::fromStdString(name), monitor), std::make_pair(QString("server"), 3));
}

void ServerConfigTests::parseMonitorSuffix_withoutMonitor_zero()
{
  std::string name = "server";
  int monitor = 7;
  QVERIFY(Config::parseMonitorSuffix(name, monitor));
  QCOMPARE(std::make_pair(QString::fromStdString(name), monitor), std::make_pair(QString("server"), 0));
}

void ServerConfigTests::parseMonitorSuffix_notANumber_false()
{
  std::string name = "server@x";
  int monitor = 0;
  QVERIFY(!Config::parseMonitorSuffix(name, monitor));
}

void ServerConfigTests::parseMonitorSuffix_zero_false()
{
  std::string name = "server@0";
  int monitor = 0;
  QVERIFY(!Config::parseMonitorSuffix(name, monitor));
}

void ServerConfigTests::format_monitorLink_writesMonitorSuffix()
{
  Config c = serverWithClientLeftOfMonitor1();
  std::ostringstream out;
  out << c;
  QVERIFY(out.str().find("left@1 = client") != std::string::npos);
}

QTEST_MAIN(ServerConfigTests)
