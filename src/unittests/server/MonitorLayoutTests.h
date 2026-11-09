/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2026 Deskflow Developers
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include <QTest>

class MonitorLayoutTests : public QObject
{
  Q_OBJECT
private Q_SLOTS:
  void bounds_lLayout_unionOfMonitors();
  void bounds_emptyList_zero();
  void monitorAt_pointInside_returnsThatMonitor();
  void monitorAt_pointInDeadSpace_returnsNearest();
  void hasMonitorBeyond_rightOfTopLeft_true();
  void hasMonitorBeyond_rightOfBottomLeft_false();
  void hasMonitorBeyond_bottomOfTopLeft_true();
  void hasMonitorBeyond_bottomOfTopRight_false();
  void hasMonitorBeyond_leftOfTopLeft_false();
  void hasMonitorBeyond_rightOfTopLeftBelowNeighbor_false();
  void span_topLeftLeftSide_topFraction();
  void span_bottomLeftBottomSide_leftFraction();
  void snapToEdge_rightEntryBelowTopRight_landsOnBottomLeft();
  void snapToEdge_rightEntryBesideTopRight_landsOnTopRight();
  void snapToEdge_bottomEntryInDeadColumn_clampsIntoNearest();
  void atOuterEdge_bottomLeftRightEdge_true();
  void atOuterEdge_topLeftRightEdge_false();
  void atOuterEdge_insideMonitor_false();
};
