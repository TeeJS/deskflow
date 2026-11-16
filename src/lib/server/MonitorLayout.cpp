/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "server/MonitorLayout.h"

#include <algorithm>
#include <cassert>
#include <limits>

namespace deskflow::server::monitorlayout {

namespace {

bool contains(const Monitor &m, int32_t x, int32_t y)
{
  return x >= m.x && x < m.x + m.w && y >= m.y && y < m.y + m.h;
}

bool containsX(const Monitor &m, int32_t x)
{
  return x >= m.x && x < m.x + m.w;
}

bool containsY(const Monitor &m, int32_t y)
{
  return y >= m.y && y < m.y + m.h;
}

// squared distance from x,y to the nearest point of m, zero when inside
int64_t distanceSquared(const Monitor &m, int32_t x, int32_t y)
{
  int64_t dx = 0;
  if (x < m.x) {
    dx = m.x - x;
  } else if (x >= m.x + m.w) {
    dx = x - (m.x + m.w - 1);
  }
  int64_t dy = 0;
  if (y < m.y) {
    dy = m.y - y;
  } else if (y >= m.y + m.h) {
    dy = y - (m.y + m.h - 1);
  }
  return dx * dx + dy * dy;
}

float fraction(int32_t value, int32_t origin, int32_t size)
{
  if (size <= 0) {
    return 0.0f;
  }
  const float f = static_cast<float>(value - origin) / static_cast<float>(size);
  return std::clamp(f, 0.0f, 1.0f);
}

} // namespace

void bounds(const MonitorList &monitors, int32_t &x, int32_t &y, int32_t &w, int32_t &h)
{
  if (monitors.empty()) {
    x = y = w = h = 0;
    return;
  }
  int32_t left = std::numeric_limits<int32_t>::max();
  int32_t top = std::numeric_limits<int32_t>::max();
  int32_t right = std::numeric_limits<int32_t>::min();
  int32_t bottom = std::numeric_limits<int32_t>::min();
  for (const auto &m : monitors) {
    left = std::min(left, m.x);
    top = std::min(top, m.y);
    right = std::max(right, m.x + m.w);
    bottom = std::max(bottom, m.y + m.h);
  }
  x = left;
  y = top;
  w = right - left;
  h = bottom - top;
}

size_t monitorAt(const MonitorList &monitors, int32_t x, int32_t y)
{
  assert(!monitors.empty());
  size_t nearest = 0;
  int64_t nearestDistance = std::numeric_limits<int64_t>::max();
  for (size_t i = 0; i < monitors.size(); ++i) {
    if (contains(monitors[i], x, y)) {
      return i;
    }
    const int64_t distance = distanceSquared(monitors[i], x, y);
    if (distance < nearestDistance) {
      nearestDistance = distance;
      nearest = i;
    }
  }
  return nearest;
}

bool hasMonitorBeyond(const MonitorList &monitors, size_t index, Direction side, int32_t along)
{
  assert(index < monitors.size());
  const Monitor &m = monitors[index];
  for (size_t i = 0; i < monitors.size(); ++i) {
    if (i == index) {
      continue;
    }
    const Monitor &o = monitors[i];
    switch (side) {
      using enum Direction;
    case Left:
      if (o.x < m.x && o.x + o.w >= m.x && containsY(o, along)) {
        return true;
      }
      break;
    case Right:
      if (o.x + o.w > m.x + m.w && o.x <= m.x + m.w && containsY(o, along)) {
        return true;
      }
      break;
    case Top:
      if (o.y < m.y && o.y + o.h >= m.y && containsX(o, along)) {
        return true;
      }
      break;
    case Bottom:
      if (o.y + o.h > m.y + m.h && o.y <= m.y + m.h && containsX(o, along)) {
        return true;
      }
      break;
    case NoDirection:
      break;
    }
  }
  return false;
}

std::pair<float, float> span(const MonitorList &monitors, size_t index, Direction side)
{
  assert(index < monitors.size());
  int32_t bx;
  int32_t by;
  int32_t bw;
  int32_t bh;
  bounds(monitors, bx, by, bw, bh);
  const Monitor &m = monitors[index];
  switch (side) {
    using enum Direction;
  case Left:
  case Right:
    return {fraction(m.y, by, bh), fraction(m.y + m.h, by, bh)};
  case Top:
  case Bottom:
    return {fraction(m.x, bx, bw), fraction(m.x + m.w, bx, bw)};
  case NoDirection:
    break;
  }
  return {0.0f, 1.0f};
}

size_t snapToEdge(const MonitorList &monitors, Direction enteredFrom, int32_t &x, int32_t &y)
{
  assert(!monitors.empty());
  bool found = false;
  size_t best = 0;
  for (size_t i = 0; i < monitors.size(); ++i) {
    const Monitor &m = monitors[i];
    const Monitor &b = monitors[best];
    switch (enteredFrom) {
      using enum Direction;
    case Left:
      if (containsY(m, y) && (!found || m.x < b.x)) {
        best = i;
        found = true;
      }
      break;
    case Right:
      if (containsY(m, y) && (!found || m.x + m.w > b.x + b.w)) {
        best = i;
        found = true;
      }
      break;
    case Top:
      if (containsX(m, x) && (!found || m.y < b.y)) {
        best = i;
        found = true;
      }
      break;
    case Bottom:
      if (containsX(m, x) && (!found || m.y + m.h > b.y + b.h)) {
        best = i;
        found = true;
      }
      break;
    case NoDirection:
      break;
    }
  }

  if (!found) {
    best = monitorAt(monitors, x, y);
    const Monitor &m = monitors[best];
    x = std::clamp(x, m.x, m.x + m.w - 1);
    y = std::clamp(y, m.y, m.y + m.h - 1);
    return best;
  }

  const Monitor &m = monitors[best];
  switch (enteredFrom) {
    using enum Direction;
  case Left:
    x = m.x;
    break;
  case Right:
    x = m.x + m.w - 1;
    break;
  case Top:
    y = m.y;
    break;
  case Bottom:
    y = m.y + m.h - 1;
    break;
  case NoDirection:
    break;
  }
  return best;
}

bool atOuterEdge(const MonitorList &monitors, size_t index, Direction side, int32_t x, int32_t y, int32_t zone)
{
  assert(index < monitors.size());
  const Monitor &m = monitors[index];
  switch (side) {
    using enum Direction;
  case Left:
    return x < m.x + zone && !hasMonitorBeyond(monitors, index, Left, y);
  case Right:
    return x >= m.x + m.w - zone && !hasMonitorBeyond(monitors, index, Right, y);
  case Top:
    return y < m.y + zone && !hasMonitorBeyond(monitors, index, Top, x);
  case Bottom:
    return y >= m.y + m.h - zone && !hasMonitorBeyond(monitors, index, Bottom, x);
  case NoDirection:
    break;
  }
  return false;
}

} // namespace deskflow::server::monitorlayout
