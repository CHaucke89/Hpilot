#!/usr/bin/env python3
import unittest

from openpilot.tools.sim.run_bridge import parse_args
from openpilot.tools.sim.bridge.metadrive.metadrive_bridge import MetaDriveBridge
from openpilot.tools.sim.tests.test_sim_bridge import TestSimBridgeBase


class TestMetaDriveBridge(TestSimBridgeBase):
  def create_bridge(self):
    return MetaDriveBridge(parse_args([]))


if __name__ == "__main__":
  unittest.main()
