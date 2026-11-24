/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "MonitorLayoutTests.h"

#include "server/MonitorLayout.h"

using namespace deskflow;
using namespace deskflow::server::monitorlayout;

namespace {

// an L-shaped arrangement in list order (top to bottom, left to right):
//   0: top left     0,0     2560x1440
//   1: top right    2560,0  2560x1440
//   2: bottom left  0,1440  1920x1080
// the bottom right 3200x1080 is dead space.
MonitorList lLayout()
{
  return {
      {"a", 0, 0, 2560, 1440},
      {"b", 2560, 0, 2560, 1440},
      {"c", 0, 1440, 1920, 1080},
  };
}

constexpr size_t TopLeft = 0;
constexpr size_t TopRight = 1;
constexpr size_t BottomLeft = 2;

} // namespace

void MonitorLayoutTests::bounds_lLayout_unionOfMonitors()
{
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
  bounds(lLayout(), x, y, w, h);
  QCOMPARE(std::make_tuple(x, y, w, h), std::make_tuple(0, 0, 5120, 2520));
}

void MonitorLayoutTests::bounds_emptyList_zero()
{
  int32_t x;
  int32_t y;
  int32_t w;
  int32_t h;
  bounds({}, x, y, w, h);
  QCOMPARE(std::make_tuple(x, y, w, h), std::make_tuple(0, 0, 0, 0));
}

void MonitorLayoutTests::monitorAt_pointInside_returnsThatMonitor()
{
  QCOMPARE(monitorAt(lLayout(), 100, 2000), BottomLeft);
}

void MonitorLayoutTests::monitorAt_pointInDeadSpace_returnsNearest()
{
  // just right of the bottom left monitor, below the top right one
  QCOMPARE(monitorAt(lLayout(), 1930, 2000), BottomLeft);
}

void MonitorLayoutTests::hasMonitorBeyond_rightOfTopLeft_true()
{
  QVERIFY(hasMonitorBeyond(lLayout(), TopLeft, Direction::Right, 700));
}

void MonitorLayoutTests::hasMonitorBeyond_rightOfBottomLeft_false()
{
  QVERIFY(!hasMonitorBeyond(lLayout(), BottomLeft, Direction::Right, 2000));
}

void MonitorLayoutTests::hasMonitorBeyond_bottomOfTopLeft_true()
{
  QVERIFY(hasMonitorBeyond(lLayout(), TopLeft, Direction::Bottom, 1000));
}

void MonitorLayoutTests::hasMonitorBeyond_bottomOfTopRight_false()
{
  QVERIFY(!hasMonitorBeyond(lLayout(), TopRight, Direction::Bottom, 3000));
}

void MonitorLayoutTests::hasMonitorBeyond_leftOfTopLeft_false()
{
  QVERIFY(!hasMonitorBeyond(lLayout(), TopLeft, Direction::Left, 700));
}

void MonitorLayoutTests::hasMonitorBeyond_rightOfTopLeftBelowNeighbor_false()
{
  // the top left monitor's bottom-right corner area has the top right
  // monitor beside it only within the top right monitor's own height
  QVERIFY(!hasMonitorBeyond(lLayout(), TopLeft, Direction::Right, 1500));
}

void MonitorLayoutTests::span_topLeftLeftSide_topFraction()
{
  const auto s = span(lLayout(), TopLeft, Direction::Left);
  QCOMPARE(s.first, 0.0f);
  QVERIFY(qFuzzyCompare(s.second, 1440.0f / 2520.0f));
}

void MonitorLayoutTests::span_bottomLeftBottomSide_leftFraction()
{
  const auto s = span(lLayout(), BottomLeft, Direction::Bottom);
  QCOMPARE(s.first, 0.0f);
  QVERIFY(qFuzzyCompare(s.second, 1920.0f / 5120.0f));
}

void MonitorLayoutTests::snapToEdge_rightEntryBelowTopRight_landsOnBottomLeft()
{
  // the server put the cursor on the bounding box's right edge, in dead space
  int32_t x = 5119;
  int32_t y = 2000;
  const auto index = snapToEdge(lLayout(), Direction::Right, x, y);
  QCOMPARE(index, BottomLeft);
  QCOMPARE(std::make_pair(x, y), std::make_pair(1919, 2000));
}

void MonitorLayoutTests::snapToEdge_rightEntryBesideTopRight_landsOnTopRight()
{
  int32_t x = 5119;
  int32_t y = 700;
  const auto index = snapToEdge(lLayout(), Direction::Right, x, y);
  QCOMPARE(index, TopRight);
  QCOMPARE(std::make_pair(x, y), std::make_pair(5119, 700));
}

void MonitorLayoutTests::snapToEdge_bottomEntryInDeadColumn_clampsIntoNearest()
{
  // x is right of every monitor, so no monitor column contains it
  int32_t x = 6000;
  int32_t y = 2519;
  const auto index = snapToEdge(lLayout(), Direction::Bottom, x, y);
  QCOMPARE(index, TopRight);
  QCOMPARE(std::make_pair(x, y), std::make_pair(5119, 1439));
}

void MonitorLayoutTests::atOuterEdge_bottomLeftRightEdge_true()
{
  QVERIFY(atOuterEdge(lLayout(), BottomLeft, Direction::Right, 1919, 2000, 1));
}

void MonitorLayoutTests::atOuterEdge_topLeftRightEdge_false()
{
  QVERIFY(!atOuterEdge(lLayout(), TopLeft, Direction::Right, 2559, 700, 1));
}

void MonitorLayoutTests::atOuterEdge_insideMonitor_false()
{
  QVERIFY(!atOuterEdge(lLayout(), BottomLeft, Direction::Right, 1000, 2000, 1));
}

QTEST_MAIN(MonitorLayoutTests)
