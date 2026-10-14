/*
 * Deskflow -- mouse and keyboard sharing utility
 * SPDX-FileCopyrightText: (C) 2025 Chris Rizzitello <sithlord48@gmail.com>
 * SPDX-License-Identifier: GPL-2.0-only WITH LicenseRef-OpenSSL-Exception
 */

#include "base/Log.h"

#include <QTest>

class ServerConfigTests : public QObject
{
  Q_OBJECT
private Q_SLOTS:
  void equalityCheck();
  void equalityCheck_diff_options();
  void equalityCheck_diff_alias();
  void equalityCheck_diff_filters();
  //  void equalityCheck_diff_address();
  void equalityCheck_diff_neighbours1();
  void equalityCheck_diff_neighbours2();
  void equalityCheck_diff_neighbours3();
  void equalityCheck_diff_monitorLinks();
  void addMonitorLink_unknownComputer_false();
  void addMonitorLink_noMonitorOnEitherEnd_false();
  void resolveMonitorLinks_leaveLinkedMonitor_neighborFound();
  void resolveMonitorLinks_leaveUnlinkedMonitor_noNeighbor();
  void resolveMonitorLinks_enterLinkedMonitor_positionInsideMonitorSpan();
  void resolveMonitorLinks_monitorOutOfRange_skipped();
  void resolveMonitorLinks_monitorOnOtherComputer_skipped();
  void resolveMonitorLinks_overlapsConfiguredLink_skipped();
  void resolveMonitorLinks_calledTwice_replacesOldLinks();
  void hasNeighbor_resolvedMonitorLink_true();
  void parseMonitorSuffix_withMonitor_splitsNameAndIndex();
  void parseMonitorSuffix_withoutMonitor_zero();
  void parseMonitorSuffix_notANumber_false();
  void parseMonitorSuffix_zero_false();
  void format_monitorLink_writesMonitorSuffix();

private:
  // resolveMonitorLinks() logs skipped links, which needs a log instance
  Log m_log;
};
