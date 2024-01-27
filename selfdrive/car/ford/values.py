from collections import defaultdict
from dataclasses import dataclass, field
from enum import Enum, StrEnum
from typing import Dict, List, Union

from cereal import car
from openpilot.selfdrive.car import AngleRateLimit, dbc_dict
from openpilot.selfdrive.car.docs_definitions import CarFootnote, CarHarness, CarInfo, CarParts, Column, \
                                           Device
from openpilot.selfdrive.car.fw_query_definitions import FwQueryConfig, Request, StdQueries

Ecu = car.CarParams.Ecu


class CarControllerParams:
  STEER_STEP = 5        # LateralMotionControl, 20Hz
  LKA_STEP = 3          # Lane_Assist_Data1, 33Hz
  ACC_CONTROL_STEP = 2  # ACCDATA, 50Hz
  LKAS_UI_STEP = 100    # IPMA_Data, 1Hz
  ACC_UI_STEP = 20      # ACCDATA_3, 5Hz
  BUTTONS_STEP = 5      # Steering_Data_FD1, 10Hz, but send twice as fast

  CURVATURE_MAX = 0.02  # Max curvature for steering command, m^-1
  STEER_DRIVER_ALLOWANCE = 1.0  # Driver intervention threshold, Nm

  # Curvature rate limits
  # The curvature signal is limited to 0.003 to 0.009 m^-1/sec by the EPS depending on speed and direction
  # Limit to ~2 m/s^3 up, ~3 m/s^3 down at 75 mph
  # Worst case, the low speed limits will allow 4.3 m/s^3 up, 4.9 m/s^3 down at 75 mph
  ANGLE_RATE_LIMIT_UP = AngleRateLimit(speed_bp=[5, 25], angle_v=[0.0002, 0.0001])
  ANGLE_RATE_LIMIT_DOWN = AngleRateLimit(speed_bp=[5, 25], angle_v=[0.000225, 0.00015])
  CURVATURE_ERROR = 0.002  # ~6 degrees at 10 m/s, ~10 degrees at 35 m/s

  ACCEL_MAX = 2.0               # m/s^2 max acceleration
  ACCEL_MIN = -3.5              # m/s^2 max deceleration
  MIN_GAS = -0.5
  INACTIVE_GAS = -5.0

  def __init__(self, CP):
    pass


BUTTON_STATES = {
  "accelCruise": False,
  "decelCruise": False,
  "cancel": False,
  "setCruise": False,
  "resumeCruise": False,
  "gapAdjustCruise": False
}


class CAR(StrEnum):
  BRONCO_SPORT_MK1 = "FORD BRONCO SPORT 1ST GEN"
  ESCAPE_MK4 = "FORD ESCAPE 4TH GEN"
  EXPLORER_MK6 = "FORD EXPLORER 6TH GEN"
  F_150_MK14 = "FORD F-150 14TH GEN"
  FOCUS_MK4 = "FORD FOCUS 4TH GEN"
  MAVERICK_MK1 = "FORD MAVERICK 1ST GEN"
  F_150_LIGHTNING_MK1 = "FORD F-150 LIGHTNING 1ST GEN"
  MUSTANG_MACH_E_MK1 = "FORD MUSTANG MACH-E 1ST GEN"


CANFD_CAR = {CAR.F_150_MK14, CAR.F_150_LIGHTNING_MK1, CAR.MUSTANG_MACH_E_MK1}


class RADAR:
  DELPHI_ESR = 'ford_fusion_2018_adas'
  DELPHI_MRR = 'FORD_CADS'
  STEER_ASSIST_DATA = 'ford_lincoln_base_pt'


DBC: Dict[str, Dict[str, str]] = defaultdict(lambda: dbc_dict("ford_lincoln_base_pt", RADAR.DELPHI_MRR))

DBC[CAR.F_150_MK14] = dbc_dict("ford_lincoln_base_pt", RADAR.STEER_ASSIST_DATA)
DBC[CAR.F_150_LIGHTNING_MK1] = dbc_dict("ford_lincoln_base_pt", RADAR.STEER_ASSIST_DATA)
DBC[CAR.MUSTANG_MACH_E_MK1] = dbc_dict("ford_lincoln_base_pt", RADAR.STEER_ASSIST_DATA)


class Footnote(Enum):
  FOCUS = CarFootnote(
    "Refers only to the Focus Mk4 (C519) available in Europe/China/Taiwan/Australasia, not the Focus Mk3 (C346) in " +
    "North and South America/Southeast Asia.",
    Column.MODEL,
  )


@dataclass
class FordCarInfo(CarInfo):
  package: str = "Co-Pilot360 Assist+"
  car_parts: CarParts = field(default_factory=CarParts.common([CarHarness.ford_q3]))

  def init_make(self, CP: car.CarParams):
    if CP.carFingerprint in (CAR.BRONCO_SPORT_MK1, CAR.MAVERICK_MK1):
      self.car_parts = CarParts([Device.threex_angled_mount, CarHarness.ford_q3])
    if CP.carFingerprint in (CAR.F_150_LIGHTNING_MK1, CAR.F_150_MK14, CAR.MUSTANG_MACH_E_MK1):
      self.car_parts = CarParts([CarHarness.ford_q4])
    if CP.carFingerprint in (CAR.F_150_LIGHTNING_MK1, CAR.F_150_MK14):
      self.car_parts = CarParts([Device.threex_angled_mount])


CAR_INFO: Dict[str, Union[CarInfo, List[CarInfo]]] = {
  CAR.BRONCO_SPORT_MK1: FordCarInfo("Ford Bronco Sport 2021-22"),
  CAR.ESCAPE_MK4: [
    FordCarInfo("Ford Escape 2020-22"),
    FordCarInfo("Ford Kuga 2020-22", "Adaptive Cruise Control with Lane Centering"),
  ],
  CAR.EXPLORER_MK6: [
    FordCarInfo("Ford Explorer 2020-22"),
    FordCarInfo("Lincoln Aviator 2020-21", "Co-Pilot360 Plus"),
  ],
  CAR.F_150_MK14: FordCarInfo("Ford F-150 2022-23", "Co-Pilot360 Active 2.0"),
  CAR.F_150_LIGHTNING_MK1: FordCarInfo("Ford F-150 Lightning 2021-23", "Co-Pilot360 Active 2.0"),
  CAR.MUSTANG_MACH_E_MK1: FordCarInfo("Ford Mustang Mach-E 2021-23", "Co-Pilot360 Active 2.0"),
  CAR.FOCUS_MK4: FordCarInfo("Ford Focus 2018", "Adaptive Cruise Control with Lane Centering", footnotes=[Footnote.FOCUS]),
  CAR.MAVERICK_MK1: [
    FordCarInfo("Ford Maverick 2022", "LARIAT Luxury"),
    FordCarInfo("Ford Maverick 2023", "Co-Pilot360 Assist"),
  ],
}

FW_QUERY_CONFIG = FwQueryConfig(
  requests=[
    # CAN and CAN FD queries are combined.
    # FIXME: For CAN FD, ECUs respond with frames larger than 8 bytes on the powertrain bus
    # TODO: properly handle auxiliary requests to separate queries and add back whitelists
    Request(
      [StdQueries.TESTER_PRESENT_REQUEST, StdQueries.MANUFACTURER_SOFTWARE_VERSION_REQUEST],
      [StdQueries.TESTER_PRESENT_RESPONSE, StdQueries.MANUFACTURER_SOFTWARE_VERSION_RESPONSE],
      # whitelist_ecus=[Ecu.engine],
    ),
    Request(
      [StdQueries.TESTER_PRESENT_REQUEST, StdQueries.MANUFACTURER_SOFTWARE_VERSION_REQUEST],
      [StdQueries.TESTER_PRESENT_RESPONSE, StdQueries.MANUFACTURER_SOFTWARE_VERSION_RESPONSE],
      # whitelist_ecus=[Ecu.eps, Ecu.abs, Ecu.fwdRadar, Ecu.fwdCamera, Ecu.shiftByWire],
      bus=0,
      auxiliary=True,
    ),
  ],
  extra_ecus=[
    (Ecu.shiftByWire, 0x732, None),
  ],
)

FW_VERSIONS = {
  CAR.BRONCO_SPORT_MK1: {
    (Ecu.eps, 0x730, None): [
      b'LX6C-14D003-AH\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-14D003-AK\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'LX6C-2D053-RD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-2D053-RE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'LB5T-14D049-AB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'M1PT-14F397-AC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'M1PA-14C204-GF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'N1PA-14C204-AC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'N1PA-14C204-AD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.ESCAPE_MK4: {
    (Ecu.eps, 0x730, None): [
      b'LX6C-14D003-AF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-14D003-AH\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-14D003-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'LX6C-2D053-NS\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-2D053-NT\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-2D053-NY\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-2D053-SA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6C-2D053-SD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'LB5T-14D049-AB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'LJ6T-14F397-AD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LJ6T-14F397-AE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LV4T-14F397-GG\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'LX6A-14C204-BJV\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6A-14C204-BJX\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6A-14C204-CNG\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6A-14C204-DPK\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LX6A-14C204-ESG\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MX6A-14C204-BEF\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MX6A-14C204-BEJ\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MX6A-14C204-CAB\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NX6A-14C204-BLE\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.EXPLORER_MK6: {
    (Ecu.eps, 0x730, None): [
      b'L1MC-14D003-AJ\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-14D003-AK\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-14D003-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'M1MC-14D003-AB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'M1MC-14D003-AC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'L1MC-2D053-AJ\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-2D053-BA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-2D053-BB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-2D053-BF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'L1MC-2D053-KB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'LB5T-14D049-AB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'LB5T-14F397-AD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5T-14F397-AE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5T-14F397-AF\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LC5T-14F397-AH\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'LB5A-14C204-ATJ\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5A-14C204-AUJ\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5A-14C204-AZL\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5A-14C204-BUJ\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LB5A-14C204-EAC\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MB5A-14C204-MD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MB5A-14C204-RC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NB5A-14C204-AZD\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NB5A-14C204-HB\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.F_150_MK14: {
    (Ecu.eps, 0x730, None): [
      b'ML3V-14D003-BC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'PL34-2D053-CA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NL34-2D053-CA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'ML3T-14D049-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'PJ6T-14H102-ABJ\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'PL3A-14C204-BRB\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PL3A-14C204-BTB\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'ML3A-14C204-VHD\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
   CAR.F_150_LIGHTNING_MK1: {
    (Ecu.abs, 0x760, None): [
      b'PL38-2D053-AA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'ML3T-14H102-ABT\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'ML3T-14D049-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'NL3A-14C204-BAR\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.MUSTANG_MACH_E_MK1: {
    (Ecu.eps, 0x730, None): [
      b'LJ9C-14D003-AK\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LJ9C-14D003-AM\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LJ9C-14D003-CC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LJ9C-14D003-EA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'LK9C-2D053-CC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LK9C-2D053-CD\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'LK9C-2D053-CK\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'ML3T-14D049-AK\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'ML3T-14D049-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'ML3T-14H102-ABS\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'ML3T-14H102-ABP\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PJ6T-14H102-ABK\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'MJ98-14C204-BBP\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'MJ98-14C204-AXD\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NJ98-14C204-VE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PJ98-14C204-BE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.FOCUS_MK4: {
    (Ecu.eps, 0x730, None): [
      b'JX6C-14D003-AH\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'JX61-2D053-CJ\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'JX7T-14D049-AC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'JX7T-14F397-AH\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'JX6A-14C204-BPL\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
  CAR.MAVERICK_MK1: {
    (Ecu.eps, 0x730, None): [
      b'NZ6C-14D003-AL\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.abs, 0x760, None): [
      b'NZ6C-2D053-AG\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PZ6C-2D053-ED\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdRadar, 0x764, None): [
      b'NZ6T-14D049-AA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.fwdCamera, 0x706, None): [
      b'NZ6T-14F397-AC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
    (Ecu.engine, 0x7E0, None): [
      b'NZ6A-14C204-AAA\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NZ6A-14C204-PA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'NZ6A-14C204-ZA\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PZ6A-14C204-BE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PZ6A-14C204-JC\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
      b'PZ6A-14C204-JE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00',
    ],
  },
}
