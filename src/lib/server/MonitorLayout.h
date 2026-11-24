/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#pragma once

#include "base/DirectionTypes.h"
#include "deskflow/MonitorTypes.h"

#include <cstdint>
#include <utility>

namespace deskflow::server {

//! Geometry helpers for a computer made of several monitors
/*!
Pure functions over a deskflow::MonitorList.  The server uses them to
treat each monitor of the primary computer as its own box instead of
the bounding rectangle of all monitors.  Every function copes with an
empty list by behaving as if the list were one monitor matching the
caller's fallback shape.
*/
namespace monitorlayout {

//! Bounding rectangle of all monitors
void bounds(const MonitorList &monitors, int32_t &x, int32_t &y, int32_t &w, int32_t &h);

//! Monitor containing \c x,y, or the nearest one when none does
/*!
Returns an index into \c monitors.  The list must not be empty.
*/
size_t monitorAt(const MonitorList &monitors, int32_t x, int32_t y);

//! True if another monitor continues past the \c side edge of monitor \c index
/*!
\c along is the coordinate along the edge: y for Left and Right, x for
Top and Bottom.  A monitor that touches or overlaps the edge at that
coordinate counts as continuing it, so the edge is not an outer edge
of the computer there.
*/
bool hasMonitorBeyond(const MonitorList &monitors, size_t index, Direction side, int32_t along);

//! Span of monitor \c index along edges facing \c side, as a fraction of the bounds
/*!
For Left and Right the span is vertical, for Top and Bottom horizontal.
The result is the (start, end) interval in [0,1] that Config::connect()
expects.  \c index is zero based.
*/
std::pair<float, float> span(const MonitorList &monitors, size_t index, Direction side);

//! Move \c x,y onto the outermost monitor edge facing \c enteredFrom
/*!
Used when the cursor enters the computer.  \c enteredFrom is the side of
the computer the cursor arrives on (Left when it comes from a computer on
the left).  The perpendicular coordinate is moved onto the edge of the
outermost monitor whose extent contains the other coordinate; if no
monitor does, the nearest monitor is used and both coordinates are
clamped inside it.  Returns the index of the monitor the point ends on.
*/
size_t snapToEdge(const MonitorList &monitors, Direction enteredFrom, int32_t &x, int32_t &y);

//! True if \c x,y is at the outer edge of its monitor on \c side, within \c zone pixels
bool atOuterEdge(const MonitorList &monitors, size_t index, Direction side, int32_t x, int32_t y, int32_t zone);

} // namespace monitorlayout

} // namespace deskflow::server
