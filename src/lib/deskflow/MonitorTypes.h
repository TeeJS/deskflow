/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>

namespace deskflow {

//! One physical monitor of a computer
/*!
Position and size are in the same coordinate space as IComputer::getShape(),
so every monitor lies inside the computer's bounding rectangle.  \c name is
the platform's own identifier for the monitor (for example the display
device name on Windows) and is only guaranteed to be unique within one
monitor list.
*/
struct Monitor
{
  std::string name;
  int32_t x = 0;
  int32_t y = 0;
  int32_t w = 0;
  int32_t h = 0;

  bool operator==(const Monitor &) const = default;
};

using MonitorList = std::vector<Monitor>;

//! Order monitors top to bottom, then left to right
/*!
Gives every platform the same, position-based order so that the index of
a monitor in the list is stable for a given arrangement.
*/
inline void sortMonitors(MonitorList &monitors)
{
  std::sort(monitors.begin(), monitors.end(), [](const Monitor &a, const Monitor &b) {
    return a.y != b.y ? a.y < b.y : a.x < b.x;
  });
}

} // namespace deskflow
