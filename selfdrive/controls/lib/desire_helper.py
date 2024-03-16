from cereal import log
from openpilot.common.conversions import Conversions as CV
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL

LaneChangeState = log.LaneChangeState
LaneChangeDirection = log.LaneChangeDirection
TurnDirection = log.Desire

LANE_CHANGE_SPEED_MIN = 20 * CV.MPH_TO_MS
LANE_CHANGE_TIME_MAX = 10.

DESIRES = {
  LaneChangeDirection.none: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.none,
    LaneChangeState.laneChangeFinishing: log.Desire.none,
  },
  LaneChangeDirection.left: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.laneChangeLeft,
    LaneChangeState.laneChangeFinishing: log.Desire.laneChangeLeft,
  },
  LaneChangeDirection.right: {
    LaneChangeState.off: log.Desire.none,
    LaneChangeState.preLaneChange: log.Desire.none,
    LaneChangeState.laneChangeStarting: log.Desire.laneChangeRight,
    LaneChangeState.laneChangeFinishing: log.Desire.laneChangeRight,
  },
}

TURN_DESIRES = {
  TurnDirection.none: log.Desire.none,
  TurnDirection.turnLeft: log.Desire.turnLeft,
  TurnDirection.turnRight: log.Desire.turnRight,
}


class DesireHelper:
  def __init__(self):
    self.lane_change_state = LaneChangeState.off
    self.lane_change_direction = LaneChangeDirection.none
    self.lane_change_timer = 0.0
    self.lane_change_ll_prob = 1.0
    self.keep_pulse_timer = 0.0
    self.prev_one_blinker = False
    self.desire = log.Desire.none

    # FrogPilot variables
    self.params = Params()
    self.params_memory = Params("/dev/shm/params")

    self.turn_direction = TurnDirection.none

    self.lane_change_completed = False
    self.turn_completed = False

    self.lane_change_wait_timer = 0

    self.update_frogpilot_params()

  def update(self, carstate, lateral_active, lane_change_prob, frogpilotPlan):
    v_ego = carstate.vEgo
    one_blinker = carstate.leftBlinker != carstate.rightBlinker
    below_lane_change_speed = v_ego < LANE_CHANGE_SPEED_MIN

    if not (self.lane_detection and one_blinker) or below_lane_change_speed:
      lane_available = True
    else:
      desired_lane = frogpilotPlan.laneWidthLeft if carstate.leftBlinker else frogpilotPlan.laneWidthRight
      # Check if the lane width exceeds the threshold
      lane_available = desired_lane >= self.lane_detection_width

    if not lateral_active or self.lane_change_timer > LANE_CHANGE_TIME_MAX:
      self.lane_change_state = LaneChangeState.off
      self.lane_change_direction = LaneChangeDirection.none
      self.turn_direction = TurnDirection.none
    elif one_blinker and below_lane_change_speed and self.turn_desires:
      self.turn_direction = TurnDirection.turnLeft if carstate.leftBlinker else TurnDirection.turnRight
      # Set the "turn_completed" flag to prevent lane changes after completing a turn
      self.turn_completed = True
    else:
      # TurnDirection.turnLeft / turnRight
      self.turn_direction = TurnDirection.none

      # LaneChangeState.off
      if self.lane_change_state == LaneChangeState.off and one_blinker and not self.prev_one_blinker and not below_lane_change_speed:
        self.lane_change_state = LaneChangeState.preLaneChange
        self.lane_change_ll_prob = 1.0
        self.lane_change_wait_timer = 0

      # LaneChangeState.preLaneChange
      elif self.lane_change_state == LaneChangeState.preLaneChange:
        # Set lane change direction
        self.lane_change_direction = LaneChangeDirection.left if \
          carstate.leftBlinker else LaneChangeDirection.right

        torque_applied = carstate.steeringPressed and \
                         ((carstate.steeringTorque > 0 and self.lane_change_direction == LaneChangeDirection.left) or
                          (carstate.steeringTorque < 0 and self.lane_change_direction == LaneChangeDirection.right))

        blindspot_detected = ((carstate.leftBlindspot and self.lane_change_direction == LaneChangeDirection.left) or
                              (carstate.rightBlindspot and self.lane_change_direction == LaneChangeDirection.right))

        # Conduct a nudgeless lane change if all the conditions are met
        self.lane_change_wait_timer += DT_MDL
        if self.nudgeless and lane_available and not self.lane_change_completed and self.lane_change_wait_timer >= self.lane_change_delay:
          self.lane_change_wait_timer = 0
          torque_applied = True

        if not one_blinker or below_lane_change_speed:
          self.lane_change_state = LaneChangeState.off
          self.lane_change_direction = LaneChangeDirection.none
        elif torque_applied and not blindspot_detected:
          # Set the "lane_change_completed" flag to prevent any more lane changes if the toggle is on
          self.lane_change_completed = self.one_lane_change
          self.lane_change_state = LaneChangeState.laneChangeStarting

      # LaneChangeState.laneChangeStarting
      elif self.lane_change_state == LaneChangeState.laneChangeStarting:
        # fade out over .5s
        self.lane_change_ll_prob = max(self.lane_change_ll_prob - 2 * DT_MDL, 0.0)

        # 98% certainty
        if lane_change_prob < 0.02 and self.lane_change_ll_prob < 0.01:
          self.lane_change_state = LaneChangeState.laneChangeFinishing

      # LaneChangeState.laneChangeFinishing
      elif self.lane_change_state == LaneChangeState.laneChangeFinishing:
        # fade in laneline over 1s
        self.lane_change_ll_prob = min(self.lane_change_ll_prob + DT_MDL, 1.0)

        if self.lane_change_ll_prob > 0.99:
          self.lane_change_direction = LaneChangeDirection.none
          if one_blinker:
            self.lane_change_state = LaneChangeState.preLaneChange
          else:
            self.lane_change_state = LaneChangeState.off

    if self.lane_change_state in (LaneChangeState.off, LaneChangeState.preLaneChange):
      self.lane_change_timer = 0.0
    else:
      self.lane_change_timer += DT_MDL

    self.prev_one_blinker = one_blinker

    # Reset the flags
    self.lane_change_completed &= one_blinker
    self.turn_completed &= one_blinker

    if self.turn_direction != TurnDirection.none:
      self.desire = TURN_DESIRES[self.turn_direction]
    elif not self.turn_completed:
      self.desire = DESIRES[self.lane_change_direction][self.lane_change_state]
    else:
      self.desire = log.Desire.none

    # Send keep pulse once per second during LaneChangeStart.preLaneChange
    if self.lane_change_state in (LaneChangeState.off, LaneChangeState.laneChangeStarting):
      self.keep_pulse_timer = 0.0
    elif self.lane_change_state == LaneChangeState.preLaneChange:
      self.keep_pulse_timer += DT_MDL
      if self.keep_pulse_timer > 1.0:
        self.keep_pulse_timer = 0.0
      elif self.desire in (log.Desire.keepLeft, log.Desire.keepRight):
        self.desire = log.Desire.none

    if self.params_memory.get_bool("FrogPilotTogglesUpdated"):
      self.update_frogpilot_params()

  def update_frogpilot_params(self):
    is_metric = self.params.get_bool("IsMetric")

    self.nudgeless = self.params.get_bool("NudgelessLaneChange")
    self.lane_change_delay = self.params.get_int("LaneChangeTime") if self.nudgeless else 0
    self.lane_detection = self.nudgeless and self.params.get_bool("LaneDetection")
    self.lane_detection_width = self.params.get_int("LaneDetectionWidth") * (1 if is_metric else CV.FOOT_TO_METER) / 10 if self.lane_detection else 0
    self.one_lane_change = self.nudgeless and self.params.get_bool("OneLaneChange")

    self.turn_desires = self.params.get_bool("TurnDesires")
