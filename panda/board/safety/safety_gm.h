const SteeringLimits GM_STEERING_LIMITS = {
  .max_steer = 300,
  .max_rate_up = 10,
  .max_rate_down = 15,
  .driver_torque_allowance = 65,
  .driver_torque_factor = 4,
  .max_rt_delta = 128,
  .max_rt_interval = 250000,
  .type = TorqueDriverLimited,
};

const LongitudinalLimits GM_ASCM_LONG_LIMITS = {
  .max_gas = 3072,
  .min_gas = 1404,
  .inactive_gas = 1404,
  .max_brake = 400,
};

const LongitudinalLimits GM_CAM_LONG_LIMITS = {
  .max_gas = 3400,
  .min_gas = 1514,
  .inactive_gas = 1554,
  .max_brake = 400,
};

const LongitudinalLimits *gm_long_limits;

const int GM_STANDSTILL_THRSLD = 10;  // 0.311kph

// panda interceptor threshold needs to be equivalent to openpilot threshold to avoid controls mismatches
// If thresholds are mismatched then it is possible for panda to see the gas fall and rise while openpilot is in the pre-enabled state
const int GM_GAS_INTERCEPTOR_THRESHOLD = 515; // (610 + 306.25) / 2 ratio between offset and gain from dbc file
#define GM_GET_INTERCEPTOR(msg) (((GET_BYTE((msg), 0) << 8) + GET_BYTE((msg), 1) + (GET_BYTE((msg), 2) << 8) + GET_BYTE((msg), 3)) / 2U) // avg between 2 tracks

const CanMsg GM_ASCM_TX_MSGS[] = {{0x180, 0, 4}, {0x409, 0, 7}, {0x40A, 0, 7}, {0x2CB, 0, 8}, {0x370, 0, 6}, {0x200, 0, 6},  // pt bus
                                  {0xA1, 1, 7}, {0x306, 1, 8}, {0x308, 1, 7}, {0x310, 1, 2},   // obs bus
                                  {0x315, 2, 5},  // ch bus
                                  {0x104c006c, 3, 3}, {0x10400060, 3, 5}};  // gmlan

const CanMsg GM_CAM_TX_MSGS[] = {{0x180, 0, 4}, {0x200, 0, 6}, {0x1E1, 0, 7},  // pt bus
                                 {0x1E1, 2, 7}, {0x184, 2, 8}};  // camera bus

const CanMsg GM_CAM_LONG_TX_MSGS[] = {{0x180, 0, 4}, {0x315, 0, 5}, {0x2CB, 0, 8}, {0x370, 0, 6}, {0x200, 0, 6},  // pt bus
                                      {0x1E1, 2, 7}, {0x184, 2, 8}};  // camera bus

const CanMsg GM_CC_LONG_TX_MSGS[] = {{0x180, 0, 4}, {0x1E1, 0, 7},  // pt bus
                                     {0x184, 2, 8}, {0x1E1, 2, 7}};  // camera bus

// TODO: do checksum and counter checks. Add correct timestep, 0.1s for now.
AddrCheckStruct gm_addr_checks[] = {
  {.msg = {{0x184, 0, 8, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{0x34A, 0, 5, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{0x1E1, 0, 7, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{0xF1, 0, 6, .expected_timestep = 100000U}, { 0 }, { 0 }}},
  {.msg = {{0x1C4, 0, 8, .expected_timestep = 100000U}, { 0 }, { 0 }}},
};
#define GM_RX_CHECK_LEN (sizeof(gm_addr_checks) / sizeof(gm_addr_checks[0]))
addr_checks gm_rx_checks = {gm_addr_checks, GM_RX_CHECK_LEN};

const uint16_t GM_PARAM_HW_CAM = 1;
const uint16_t GM_PARAM_HW_CAM_LONG = 2;
const uint16_t GM_PARAM_CC_LONG = 4;
const uint16_t GM_PARAM_HW_ASCM_LONG = 8;
const uint16_t GM_PARAM_NO_CAMERA = 16;
const uint16_t GM_PARAM_NO_ACC = 32;

enum {
  GM_BTN_UNPRESS = 1,
  GM_BTN_RESUME = 2,
  GM_BTN_SET = 3,
  GM_BTN_CANCEL = 6,
};

enum {GM_ASCM, GM_CAM} gm_hw = GM_ASCM;
bool gm_cam_long = false;
bool gm_cc_long = false;
bool gm_has_acc = true;
bool gm_force_ascm = false;
bool gm_pcm_cruise = false;
bool gm_skip_relay_check = false;

static int gm_rx_hook(CANPacket_t *to_push) {

  bool valid = addr_safety_check(to_push, &gm_rx_checks, NULL, NULL, NULL, NULL);

  if (valid && (GET_BUS(to_push) == 0U)) {
    int addr = GET_ADDR(to_push);

    if (addr == 0x184) {
      int torque_driver_new = ((GET_BYTE(to_push, 6) & 0x7U) << 8) | GET_BYTE(to_push, 7);
      torque_driver_new = to_signed(torque_driver_new, 11);
      // update array of samples
      update_sample(&torque_driver, torque_driver_new);
    }

    // sample rear wheel speeds
    if (addr == 0x34A) {
      int left_rear_speed = (GET_BYTE(to_push, 0) << 8) | GET_BYTE(to_push, 1);
      int right_rear_speed = (GET_BYTE(to_push, 2) << 8) | GET_BYTE(to_push, 3);
      vehicle_moving = (left_rear_speed > GM_STANDSTILL_THRSLD) || (right_rear_speed > GM_STANDSTILL_THRSLD);
    }

    // ACC steering wheel buttons (GM_CAM is tied to the PCM)
    if ((addr == 0x1E1) && (!gm_pcm_cruise || gas_interceptor_detected || gm_cc_long)) {
      int button = (GET_BYTE(to_push, 5) & 0x70U) >> 4;

      // enter controls on falling edge of set or rising edge of resume (avoids fault)
      bool set = (button != GM_BTN_SET) && (cruise_button_prev == GM_BTN_SET);
      bool res = (button == GM_BTN_RESUME) && (cruise_button_prev != GM_BTN_RESUME);
      if (set || res) {
        controls_allowed = true;
      }

      // exit controls on cancel press
      if (button == GM_BTN_CANCEL) {
        controls_allowed = false;
      }

      cruise_button_prev = button;
    }

    // Reference for brake pressed signals:
    // https://github.com/commaai/openpilot/blob/master/selfdrive/car/gm/carstate.py
    if ((addr == 0xBE) && (gm_hw == GM_ASCM)) {
      brake_pressed = GET_BYTE(to_push, 1) >= 8U;
    }

    if (addr == 0xC9) {
      bool cruise_available = GET_BIT(to_push, 29U) != 0U;
      lateral_controls_allowed = cruise_available;
    }

    if ((addr == 0xC9) && (gm_hw == GM_CAM)) {
      brake_pressed = GET_BIT(to_push, 40U) != 0U;
    }

    if (addr == 0x1C4) {
      if (!gas_interceptor_detected) {
        gas_pressed = GET_BYTE(to_push, 5) != 0U;
      }

      // enter controls on rising edge of ACC, exit controls when ACC off
      if (gm_pcm_cruise && !gas_interceptor_detected && gm_has_acc) {
        bool cruise_engaged = (GET_BYTE(to_push, 1) >> 5) != 0U;
        pcm_cruise_check(cruise_engaged);
      }
    }

    // Cruise check for CC only cars
    if ((addr == 0x3D1) && !gm_has_acc) {
      bool cruise_engaged = (GET_BYTE(to_push, 4) >> 7) != 0U;
      if (gm_cc_long) {
        pcm_cruise_check(cruise_engaged);
      } else {
        cruise_engaged_prev = cruise_engaged;
      }
    }

    if (addr == 0xBD) {
      regen_braking = (GET_BYTE(to_push, 0) >> 4) != 0U;
    }

    // Pedal Interceptor
    if (addr == 0x201) {
      gas_interceptor_detected = 1;
      int gas_interceptor = GM_GET_INTERCEPTOR(to_push);
      gas_pressed = gas_interceptor > GM_GAS_INTERCEPTOR_THRESHOLD;
      gas_interceptor_prev = gas_interceptor;
    }

    bool stock_ecu_detected = (addr == 0x180);  // ASCMLKASteeringCmd

    // Check ASCMGasRegenCmd only if we're blocking it
    if (!gm_pcm_cruise && (addr == 0x2CB)) {
      stock_ecu_detected = true;
    }
    generic_rx_checks(stock_ecu_detected);
  }
  return valid;
}

// all commands: gas/regen, friction brake and steering
// if controls_allowed and no pedals pressed
//     allow all commands up to limit
// else
//     block all commands that produce actuation

static int gm_tx_hook(CANPacket_t *to_send) {

  int tx = 1;
  int addr = GET_ADDR(to_send);

  if (gm_hw == GM_CAM) {
    if (gm_cc_long) {
      tx = msg_allowed(to_send, GM_CC_LONG_TX_MSGS, sizeof(GM_CC_LONG_TX_MSGS)/sizeof(GM_CC_LONG_TX_MSGS[0]));
    } else if (gm_cam_long) {
      tx = msg_allowed(to_send, GM_CAM_LONG_TX_MSGS, sizeof(GM_CAM_LONG_TX_MSGS)/sizeof(GM_CAM_LONG_TX_MSGS[0]));
    } else {
      tx = msg_allowed(to_send, GM_CAM_TX_MSGS, sizeof(GM_CAM_TX_MSGS)/sizeof(GM_CAM_TX_MSGS[0]));
    }
  } else {
    tx = msg_allowed(to_send, GM_ASCM_TX_MSGS, sizeof(GM_ASCM_TX_MSGS)/sizeof(GM_ASCM_TX_MSGS[0]));
  }

  // BRAKE: safety check
  if (addr == 0x315) {
    int brake = ((GET_BYTE(to_send, 0) & 0xFU) << 8) + GET_BYTE(to_send, 1);
    brake = (0x1000 - brake) & 0xFFF;
    if (longitudinal_brake_checks(brake, *gm_long_limits)) {
      tx = 0;
    }
  }

  // LKA STEER: safety check
  if (addr == 0x180) {
    int desired_torque = ((GET_BYTE(to_send, 0) & 0x7U) << 8) + GET_BYTE(to_send, 1);
    desired_torque = to_signed(desired_torque, 11);

    bool steer_req = (GET_BIT(to_send, 3U) != 0U);

    if (steer_torque_cmd_checks(desired_torque, steer_req, GM_STEERING_LIMITS)) {
      tx = 0;
    }
  }

  // GAS: safety check (interceptor)
  if (addr == 0x200) {
    if (longitudinal_interceptor_checks(to_send)) {
      tx = 0;
    }
  }

  // GAS/REGEN: safety check
  if (addr == 0x2CB) {
    bool apply = GET_BIT(to_send, 0U) != 0U;
    int gas_regen = ((GET_BYTE(to_send, 2) & 0x7FU) << 5) + ((GET_BYTE(to_send, 3) & 0xF8U) >> 3);

    bool violation = false;
    // Allow apply bit in pre-enabled and overriding states
    violation |= !controls_allowed && apply;
    violation |= longitudinal_gas_checks(gas_regen, *gm_long_limits);

    if (violation) {
      tx = 0;
    }
  }

  // BUTTONS: used for resume spamming and cruise cancellation with stock longitudinal
  if ((addr == 0x1E1) && (gm_pcm_cruise || gm_cc_long)) {
    int button = (GET_BYTE(to_send, 5) >> 4) & 0x7U;

    bool allowed_btn = (button == GM_BTN_CANCEL) && cruise_engaged_prev;
    // For standard CC, allow spamming of SET / RESUME
    if (gm_cc_long) {
      allowed_btn |= cruise_engaged_prev && (button == GM_BTN_SET || button == GM_BTN_RESUME || button == GM_BTN_UNPRESS);
    }

    if (!allowed_btn) {
      tx = 0;
    }
  }

  // 1 allows the message through
  return tx;
}

static int gm_fwd_hook(int bus_num, int addr) {

  int bus_fwd = -1;

  if (gm_hw == GM_CAM) {
    if (bus_num == 0) {
      // block PSCMStatus; forwarded through openpilot to hide an alert from the camera
      bool is_pscm_msg = (addr == 0x184);
      if (!is_pscm_msg) {
        bus_fwd = 2;
      }
    }

    if (bus_num == 2) {
      // block lkas message and acc messages if gm_cam_long, forward all others
      bool is_lkas_msg = (addr == 0x180);
      bool is_acc_msg = (addr == 0x315) || (addr == 0x2CB) || (addr == 0x370);
      int block_msg = is_lkas_msg || (is_acc_msg && gm_cam_long);
      if (!block_msg) {
        bus_fwd = 0;
      }
    }
  }

  return bus_fwd;
}

static const addr_checks* gm_init(uint16_t param) {
  gm_hw = GET_FLAG(param, GM_PARAM_HW_CAM) ? GM_CAM : GM_ASCM;

  gm_force_ascm = GET_FLAG(param, GM_PARAM_HW_ASCM_LONG);

  if (gm_hw == GM_ASCM || gm_force_ascm) {
    gm_long_limits = &GM_ASCM_LONG_LIMITS;
  } else if (gm_hw == GM_CAM) {
    gm_long_limits = &GM_CAM_LONG_LIMITS;
  } else {
  }

  gm_cc_long = GET_FLAG(param, GM_PARAM_CC_LONG);
  gm_cam_long = GET_FLAG(param, GM_PARAM_HW_CAM_LONG) && !gm_cc_long;
  gm_has_acc = !GET_FLAG(param, GM_PARAM_NO_ACC);
  gm_pcm_cruise = (gm_hw == GM_CAM) && (!gm_cam_long || gm_cc_long) && !gm_force_ascm;
  gm_skip_relay_check = GET_FLAG(param, GM_PARAM_NO_CAMERA);
  return &gm_rx_checks;
}

const safety_hooks gm_hooks = {
  .init = gm_init,
  .rx = gm_rx_hook,
  .tx = gm_tx_hook,
  .tx_lin = nooutput_tx_lin_hook,
  .fwd = gm_fwd_hook,
};
