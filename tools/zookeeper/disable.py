#!/usr/bin/env python3

from openpilot.tools.zookeeper import Zookeeper

if __name__ == "__main__":
  z = Zookeeper()
  z.set_device_power(False)

