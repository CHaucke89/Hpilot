from cereal import car
from panda import Panda
from openpilot.selfdrive.car import get_safety_config
from openpilot.selfdrive.car.interfaces import CarInterfaceBase
from openpilot.selfdrive.car.nissan.values import CAR


class CarInterface(CarInterfaceBase):

  @staticmethod
  def _get_params(ret, candidate, fingerprint, car_fw, experimental_long, docs):
    ret.carName = "nissan"
    ret.safetyConfigs = [get_safety_config(car.CarParams.SafetyModel.nissan)]
    ret.autoResumeSng = False

    ret.steerLimitTimer = 1.0

    ret.steerActuatorDelay = 0.1
    ret.steerRatio = 17

    ret.steerControlType = car.CarParams.SteerControlType.angle
    ret.radarUnavailable = True

    if candidate in (CAR.ROGUE, CAR.XTRAIL):
      ret.mass = 1610
      ret.wheelbase = 2.705
      ret.centerToFront = ret.wheelbase * 0.44
    elif candidate in (CAR.LEAF, CAR.LEAF_IC):
      ret.mass = 1610
      ret.wheelbase = 2.705
      ret.centerToFront = ret.wheelbase * 0.44
    elif candidate == CAR.ALTIMA:
      # Altima has EPS on C-CAN unlike the others that have it on V-CAN
      ret.safetyConfigs[0].safetyParam |= Panda.FLAG_NISSAN_ALT_EPS_BUS
      ret.mass = 1492
      ret.wheelbase = 2.824
      ret.centerToFront = ret.wheelbase * 0.44

    return ret

  # returns a car.CarState
  def _update(self, c):
    ret = self.CS.update(self.cp, self.cp_adas, self.cp_cam)

    buttonEvents = []
    be = car.CarState.ButtonEvent.new_message()
    be.type = car.CarState.ButtonEvent.Type.accelCruise
    buttonEvents.append(be)

    events = self.create_common_events(ret, extra_gears=[car.CarState.GearShifter.brake])

    if self.CS.lkas_enabled:
      events.add(car.CarEvent.EventName.invalidLkasSetting)

    ret.events = events.to_msg()

    return ret

  def apply(self, c, now_nanos):
    return self.CC.update(c, self.CS, now_nanos)
