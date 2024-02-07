#!/usr/bin/env python3
import threading
from cereal import car
from openpilot.common.params import Params
from openpilot.common.realtime import Priority, config_realtime_process
from openpilot.common.swaglog import cloudlog
from openpilot.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlanner
import cereal.messaging as messaging

from openpilot.selfdrive.frogpilot.functions.frogpilot_planner import FrogPilotPlanner

def publish_ui_plan(sm, pm, longitudinal_planner):
  ui_send = messaging.new_message('uiPlan')
  ui_send.valid = sm.all_checks(service_list=['carState', 'controlsState', 'modelV2'])
  uiPlan = ui_send.uiPlan
  uiPlan.frameId = sm['modelV2'].frameId
  uiPlan.position.x = list(sm['modelV2'].position.x)
  uiPlan.position.y = list(sm['modelV2'].position.y)
  uiPlan.position.z = list(sm['modelV2'].position.z)
  uiPlan.accel = longitudinal_planner.a_desired_trajectory_full.tolist()
  pm.send('uiPlan', ui_send)

def plannerd_thread():
  config_realtime_process(5, Priority.CTRL_LOW)

  cloudlog.info("plannerd is waiting for CarParams")
  params = Params()
  params_memory = Params("/dev/shm/params")
  with car.CarParams.from_bytes(params.get("CarParams", block=True)) as msg:
    CP = msg
  cloudlog.info("plannerd got CarParams: %s", CP.carName)

  frogpilot_planner = FrogPilotPlanner(params, params_memory)
  longitudinal_planner = LongitudinalPlanner(CP)
  pm = messaging.PubMaster(['longitudinalPlan', 'uiPlan', 'frogpilotPlan'])
  sm = messaging.SubMaster(['carControl', 'carState', 'controlsState', 'radarState', 'modelV2', 'frogpilotNavigation'],
                           poll=['radarState', 'modelV2'], ignore_avg_freq=['radarState'])

  while True:
    sm.update()

    if sm.updated['modelV2']:
      longitudinal_planner.update(sm, frogpilot_planner, params_memory)
      longitudinal_planner.publish(sm, pm, frogpilot_planner)
      publish_ui_plan(sm, pm, longitudinal_planner)

    if params_memory.get_bool("FrogPilotTogglesUpdated"):
      updateFrogPilotParams = threading.Thread(target=frogpilot_planner.update_frogpilot_params, args=(params, params_memory))
      updateFrogPilotParams.start()

def main():
  plannerd_thread()


if __name__ == "__main__":
  main()
