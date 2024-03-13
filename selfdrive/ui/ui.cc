#include "selfdrive/ui/ui.h"

#include <algorithm>
#include <cassert>
#include <cmath>

#include <QtConcurrent>

#include "common/transformations/orientation.hpp"
#include "common/params.h"
#include "common/swaglog.h"
#include "common/util.h"
#include "common/watchdog.h"
#include "system/hardware/hw.h"
#define BACKLIGHT_DT 0.05
#define BACKLIGHT_TS 10.00

// Projects a point in car to space to the corresponding point in full frame
// image space.
static bool calib_frame_to_full_frame(const UIState *s, float in_x, float in_y, float in_z, QPointF *out) {
  const float margin = 500.0f;
  const QRectF clip_region{-margin, -margin, s->fb_w + 2 * margin, s->fb_h + 2 * margin};

  const vec3 pt = (vec3){{in_x, in_y, in_z}};
  const vec3 Ep = matvecmul3(s->scene.wide_cam ? s->scene.view_from_wide_calib : s->scene.view_from_calib, pt);
  const vec3 KEp = matvecmul3(s->scene.wide_cam ? ECAM_INTRINSIC_MATRIX : FCAM_INTRINSIC_MATRIX, Ep);

  // Project.
  QPointF point = s->car_space_transform.map(QPointF{KEp.v[0] / KEp.v[2], KEp.v[1] / KEp.v[2]});
  if (clip_region.contains(point)) {
    *out = point;
    return true;
  }
  return false;
}

int get_path_length_idx(const cereal::XYZTData::Reader &line, const float path_height) {
  const auto line_x = line.getX();
  int max_idx = 0;
  for (int i = 1; i < line_x.size() && line_x[i] <= path_height; ++i) {
    max_idx = i;
  }
  return max_idx;
}

void update_leads(UIState *s, const cereal::RadarState::Reader &radar_state, const cereal::XYZTData::Reader &line) {
  for (int i = 0; i < 2; ++i) {
    auto lead_data = (i == 0) ? radar_state.getLeadOne() : radar_state.getLeadTwo();
    if (lead_data.getStatus()) {
      float z = line.getZ()[get_path_length_idx(line, lead_data.getDRel())];
      calib_frame_to_full_frame(s, lead_data.getDRel(), -lead_data.getYRel(), z + 1.22, &s->scene.lead_vertices[i]);
    }
  }
}

void update_line_data(const UIState *s, const cereal::XYZTData::Reader &line,
                      float y_off, float z_off, QPolygonF *pvd, int max_idx, bool allow_invert=true) {
  const auto line_x = line.getX(), line_y = line.getY(), line_z = line.getZ();
  QPolygonF left_points, right_points;
  left_points.reserve(max_idx + 1);
  right_points.reserve(max_idx + 1);

  for (int i = 0; i <= max_idx; i++) {
    // highly negative x positions  are drawn above the frame and cause flickering, clip to zy plane of camera
    if (line_x[i] < 0) continue;
    QPointF left, right;
    bool l = calib_frame_to_full_frame(s, line_x[i], line_y[i] - y_off, line_z[i] + z_off, &left);
    bool r = calib_frame_to_full_frame(s, line_x[i], line_y[i] + y_off, line_z[i] + z_off, &right);
    if (l && r) {
      // For wider lines the drawn polygon will "invert" when going over a hill and cause artifacts
      if (!allow_invert && left_points.size() && left.y() > left_points.back().y()) {
        continue;
      }
      left_points.push_back(left);
      right_points.push_front(right);
    }
  }
  *pvd = left_points + right_points;
}

void update_model(UIState *s,
                  const cereal::ModelDataV2::Reader &model,
                  const cereal::UiPlan::Reader &plan) {
  UIScene &scene = s->scene;
  auto plan_position = plan.getPosition();
  if (plan_position.getX().size() < model.getPosition().getX().size()) {
    plan_position = model.getPosition();
  }
  float max_distance = scene.unlimited_road_ui_length ? *(plan_position.getX().end() - 1) :
                       std::clamp(*(plan_position.getX().end() - 1),
                                  MIN_DRAW_DISTANCE, MAX_DRAW_DISTANCE);

  // update lane lines
  const auto lane_lines = model.getLaneLines();
  const auto lane_line_probs = model.getLaneLineProbs();
  int max_idx = get_path_length_idx(lane_lines[0], max_distance);
  for (int i = 0; i < std::size(scene.lane_line_vertices); i++) {
    scene.lane_line_probs[i] = lane_line_probs[i];
    update_line_data(s, lane_lines[i], scene.model_ui ? scene.lane_line_width * scene.lane_line_probs[i] : 0.025 * scene.lane_line_probs[i], 0, &scene.lane_line_vertices[i], max_idx);
  }

  // update road edges
  const auto road_edges = model.getRoadEdges();
  const auto road_edge_stds = model.getRoadEdgeStds();
  for (int i = 0; i < std::size(scene.road_edge_vertices); i++) {
    scene.road_edge_stds[i] = road_edge_stds[i];
    update_line_data(s, road_edges[i], scene.model_ui ? scene.road_edge_width : 0.025, 0, &scene.road_edge_vertices[i], max_idx);
  }

  // update path
  float path;

  if (scene.dynamic_path_width) {
    float multiplier = scene.enabled ? 1.0f : scene.always_on_lateral_active ? 0.75f : 0.50f;
    path = scene.path_width * multiplier;
  } else {
    path = scene.path_width;
  }

  auto lead_one = (*s->sm)["radarState"].getRadarState().getLeadOne();
  if (lead_one.getStatus()) {
    const float lead_d = lead_one.getDRel() * 2.;
    max_distance = std::clamp((float)(lead_d - fmin(lead_d * 0.35, 10.)), 0.0f, max_distance);
  }
  max_idx = get_path_length_idx(plan_position, max_distance);
  update_line_data(s, plan_position, scene.model_ui ? path * (1 - scene.path_edge_width / 100) : 0.9, 1.22, &scene.track_vertices, max_idx, false);

  // Update path edges
  update_line_data(s, plan_position, scene.model_ui ? path : 0, 1.22, &scene.track_edge_vertices, max_idx, false);

  // Update adjacent paths
  for (int i = 4; i <= 5; i++) {
    update_line_data(s, lane_lines[i], scene.blind_spot_path ? (i == 4 ? scene.lane_width_left : scene.lane_width_right) / 2.0f : 0, 0, &scene.track_adjacent_vertices[i], max_idx);
  }
}

void update_dmonitoring(UIState *s, const cereal::DriverStateV2::Reader &driverstate, float dm_fade_state, bool is_rhd) {
  UIScene &scene = s->scene;
  const auto driver_orient = is_rhd ? driverstate.getRightDriverData().getFaceOrientation() : driverstate.getLeftDriverData().getFaceOrientation();
  for (int i = 0; i < std::size(scene.driver_pose_vals); i++) {
    float v_this = (i == 0 ? (driver_orient[i] < 0 ? 0.7 : 0.9) : 0.4) * driver_orient[i];
    scene.driver_pose_diff[i] = fabs(scene.driver_pose_vals[i] - v_this);
    scene.driver_pose_vals[i] = 0.8 * v_this + (1 - 0.8) * scene.driver_pose_vals[i];
    scene.driver_pose_sins[i] = sinf(scene.driver_pose_vals[i]*(1.0-dm_fade_state));
    scene.driver_pose_coss[i] = cosf(scene.driver_pose_vals[i]*(1.0-dm_fade_state));
  }

  const mat3 r_xyz = (mat3){{
    scene.driver_pose_coss[1]*scene.driver_pose_coss[2],
    scene.driver_pose_coss[1]*scene.driver_pose_sins[2],
    -scene.driver_pose_sins[1],

    -scene.driver_pose_sins[0]*scene.driver_pose_sins[1]*scene.driver_pose_coss[2] - scene.driver_pose_coss[0]*scene.driver_pose_sins[2],
    -scene.driver_pose_sins[0]*scene.driver_pose_sins[1]*scene.driver_pose_sins[2] + scene.driver_pose_coss[0]*scene.driver_pose_coss[2],
    -scene.driver_pose_sins[0]*scene.driver_pose_coss[1],

    scene.driver_pose_coss[0]*scene.driver_pose_sins[1]*scene.driver_pose_coss[2] - scene.driver_pose_sins[0]*scene.driver_pose_sins[2],
    scene.driver_pose_coss[0]*scene.driver_pose_sins[1]*scene.driver_pose_sins[2] + scene.driver_pose_sins[0]*scene.driver_pose_coss[2],
    scene.driver_pose_coss[0]*scene.driver_pose_coss[1],
  }};

  // transform vertices
  for (int kpi = 0; kpi < std::size(default_face_kpts_3d); kpi++) {
    vec3 kpt_this = default_face_kpts_3d[kpi];
    kpt_this = matvecmul3(r_xyz, kpt_this);
    scene.face_kpts_draw[kpi] = (vec3){{(float)kpt_this.v[0], (float)kpt_this.v[1], (float)(kpt_this.v[2] * (1.0-dm_fade_state) + 8 * dm_fade_state)}};
  }
}

static void update_sockets(UIState *s) {
  s->sm->update(0);
}

static void update_state(UIState *s) {
  SubMaster &sm = *(s->sm);
  UIScene &scene = s->scene;

  if (sm.updated("liveCalibration")) {
    auto live_calib = sm["liveCalibration"].getLiveCalibration();
    auto rpy_list = live_calib.getRpyCalib();
    auto wfde_list = live_calib.getWideFromDeviceEuler();
    Eigen::Vector3d rpy;
    Eigen::Vector3d wfde;
    if (rpy_list.size() == 3) rpy << rpy_list[0], rpy_list[1], rpy_list[2];
    if (wfde_list.size() == 3) wfde << wfde_list[0], wfde_list[1], wfde_list[2];
    Eigen::Matrix3d device_from_calib = euler2rot(rpy);
    Eigen::Matrix3d wide_from_device = euler2rot(wfde);
    Eigen::Matrix3d view_from_device;
    view_from_device << 0, 1, 0,
                        0, 0, 1,
                        1, 0, 0;
    Eigen::Matrix3d view_from_calib = view_from_device * device_from_calib;
    Eigen::Matrix3d view_from_wide_calib = view_from_device * wide_from_device * device_from_calib;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        scene.view_from_calib.v[i*3 + j] = view_from_calib(i, j);
        scene.view_from_wide_calib.v[i*3 + j] = view_from_wide_calib(i, j);
      }
    }
    scene.calibration_valid = live_calib.getCalStatus() == cereal::LiveCalibrationData::Status::CALIBRATED;
    scene.calibration_wide_valid = wfde_list.size() == 3;
  }
  if (sm.updated("pandaStates")) {
    auto pandaStates = sm["pandaStates"].getPandaStates();
    if (pandaStates.size() > 0) {
      scene.pandaType = pandaStates[0].getPandaType();

      if (scene.pandaType != cereal::PandaState::PandaType::UNKNOWN) {
        scene.ignition = false;
        for (const auto& pandaState : pandaStates) {
          scene.ignition |= pandaState.getIgnitionLine() || pandaState.getIgnitionCan();
        }
      }
    }
  } else if ((s->sm->frame - s->sm->rcv_frame("pandaStates")) > 5*UI_FREQ) {
    scene.pandaType = cereal::PandaState::PandaType::UNKNOWN;
  }
  if (sm.updated("carParams")) {
    scene.longitudinal_control = sm["carParams"].getCarParams().getOpenpilotLongitudinalControl();
  }
  if (sm.updated("carState")) {
    auto carState = sm["carState"].getCarState();
    if (scene.blind_spot_path || scene.custom_signals) {
      scene.blind_spot_left = carState.getLeftBlindspot();
      scene.blind_spot_right = carState.getRightBlindspot();
    }
    if (scene.custom_signals) {
      scene.turn_signal_left = carState.getLeftBlinker();
      scene.turn_signal_right = carState.getRightBlinker();
    }
    if (scene.driver_camera) {
      scene.show_driver_camera = carState.getGearShifter() == cereal::CarState::GearShifter::REVERSE;
    }
    if (scene.lead_info || scene.pedals_on_ui) {
      scene.acceleration = carState.getAEgo();
    }
    if (scene.pedals_on_ui) {
      scene.standstill = carState.getStandstill();
    }
    if (scene.rotating_wheel) {
      scene.steering_angle_deg = carState.getSteeringAngleDeg();
    }
    scene.parked = carState.getGearShifter() == cereal::CarState::GearShifter::PARK;
  }
  if (sm.updated("controlsState")) {
    auto controlsState = sm["controlsState"].getControlsState();
    scene.enabled = controlsState.getEnabled();
    scene.experimental_mode = controlsState.getExperimentalMode();
  }
  if (sm.updated("frogpilotCarControl")) {
    auto frogpilotCarControl = sm["frogpilotCarControl"].getFrogpilotCarControl();
    if (scene.always_on_lateral) {
      scene.always_on_lateral_active = !scene.enabled && frogpilotCarControl.getAlwaysOnLateral();
    }
  }
  if (sm.updated("frogpilotPlan")) {
    auto frogpilotPlan = sm["frogpilotPlan"].getFrogpilotPlan();
    if (scene.blind_spot_path) {
      scene.lane_width_left = frogpilotPlan.getLaneWidthLeft();
      scene.lane_width_right = frogpilotPlan.getLaneWidthRight();
    }
    if (scene.lead_info) {
      scene.desired_follow = frogpilotPlan.getDesiredFollowDistance();
      scene.obstacle_distance = frogpilotPlan.getSafeObstacleDistance();
      scene.obstacle_distance_stock = frogpilotPlan.getSafeObstacleDistanceStock();
      scene.stopped_equivalence = frogpilotPlan.getStoppedEquivalenceFactor();
    }
    scene.adjusted_cruise = frogpilotPlan.getAdjustedCruise();
  }
  if (sm.updated("liveLocationKalman")) {
    auto liveLocationKalman = sm["liveLocationKalman"].getLiveLocationKalman();
    if (scene.compass) {
      auto orientation = liveLocationKalman.getCalibratedOrientationNED();
      if (orientation.getValid()) {
        scene.bearing_deg = RAD2DEG(orientation.getValue()[2]);
      }
    }
  }
  if (sm.updated("wideRoadCameraState")) {
    auto cam_state = sm["wideRoadCameraState"].getWideRoadCameraState();
    float scale = (cam_state.getSensor() == cereal::FrameData::ImageSensor::AR0231) ? 6.0f : 1.0f;
    scene.light_sensor = std::max(100.0f - scale * cam_state.getExposureValPercent(), 0.0f);
  }
  scene.started = sm["deviceState"].getDeviceState().getStarted() && scene.ignition;

  scene.world_objects_visible = scene.world_objects_visible ||
                                (scene.started &&
                                 sm.rcv_frame("liveCalibration") > scene.started_frame &&
                                 sm.rcv_frame("modelV2") > scene.started_frame &&
                                 sm.rcv_frame("uiPlan") > scene.started_frame);
}

void ui_update_params(UIState *s) {
  auto params = Params();
  s->scene.is_metric = params.getBool("IsMetric");
  s->scene.map_on_left = params.getBool("NavSettingLeftSide");
}

void ui_update_frogpilot_params(UIState *s) {
  ui_update_params(s);

  Params params = Params();
  UIScene &scene = s->scene;

  scene.always_on_lateral = params.getBool("AlwaysOnLateral");
  scene.camera_view = params.getInt("CameraView");
  scene.compass = params.getBool("Compass");

  scene.conditional_experimental = params.getBool("ConditionalExperimental");
  scene.conditional_speed = params.getInt("CESpeed");
  scene.conditional_speed_lead = params.getInt("CESpeedLead");

  bool custom_onroad_ui = params.getBool("CustomUI");
  scene.acceleration_path = custom_onroad_ui && params.getBool("AccelerationPath");
  scene.adjacent_path = custom_onroad_ui && params.getBool("AdjacentPath");
  scene.adjacent_path_metrics = scene.adjacent_path && params.getBool("AdjacentPathMetrics");
  scene.blind_spot_path = custom_onroad_ui && params.getBool("BlindSpotPath");
  scene.fps_counter = custom_onroad_ui && params.getBool("FPSCounter");
  scene.lead_info = custom_onroad_ui && params.getBool("LeadInfo");
  scene.use_si = scene.lead_info && params.getBool("UseSI");
  scene.pedals_on_ui = custom_onroad_ui && params.getBool("PedalsOnUI");
  scene.road_name_ui = custom_onroad_ui && params.getBool("RoadNameUI");

  bool custom_theme = params.getBool("CustomTheme");
  scene.custom_colors = custom_theme ? params.getInt("CustomColors") : 0;
  scene.custom_icons = custom_theme ? params.getInt("CustomIcons") : 0;
  scene.custom_signals = custom_theme ? params.getInt("CustomSignals") : 0;
  scene.holiday_themes = custom_theme && params.getBool("HolidayThemes");

  scene.disable_smoothing_mtsc = params.getBool("DisableMTSCSmoothing");
  scene.driver_camera = params.getBool("DriverCamera");
  scene.experimental_mode_via_screen = params.getBool("ExperimentalModeViaScreen") && params.getBool("ExperimentalModeActivation");
  scene.fahrenheit = params.getBool("Fahrenheit");

  scene.model_ui = params.getBool("ModelUI");
  scene.dynamic_path_width = scene.model_ui && params.getBool("DynamicPathWidth");
  scene.lane_line_width = params.getInt("LaneLinesWidth") * (scene.is_metric ? 1.0f : INCH_TO_CM) / 200.0f;
  scene.path_edge_width = params.getInt("PathEdgeWidth");
  scene.path_width = params.getInt("PathWidth") / 10.0f * (scene.is_metric ? 1.0f : FOOT_TO_METER) / 2.0f;
  scene.road_edge_width = params.getInt("RoadEdgesWidth") * (scene.is_metric ? 1.0f : INCH_TO_CM) / 200.0f;
  scene.unlimited_road_ui_length = scene.model_ui && params.getBool("UnlimitedLength");

  scene.numerical_temp = params.getBool("NumericalTemp");

  bool quality_of_life_controls = params.getBool("QOLControls");
  scene.reverse_cruise = quality_of_life_controls && params.getBool("ReverseCruise");
  scene.reverse_cruise_ui = scene.reverse_cruise && params.getBool("ReverseCruiseUI");

  bool quality_of_life_visuals = params.getBool("QOLVisuals");
  scene.full_map = quality_of_life_visuals && params.getBool("FullMap");
  scene.hide_speed = quality_of_life_visuals && params.getBool("HideSpeed");
  scene.hide_speed_ui = scene.hide_speed && params.getBool("HideSpeedUI");

  scene.rotating_wheel = params.getBool("RotatingWheel");
}

void UIState::updateStatus() {
  if (scene.started && sm->updated("controlsState")) {
    auto controls_state = (*sm)["controlsState"].getControlsState();
    auto state = controls_state.getState();
    if (state == cereal::ControlsState::OpenpilotState::PRE_ENABLED || state == cereal::ControlsState::OpenpilotState::OVERRIDING) {
      status = STATUS_OVERRIDE;
    } else if (scene.always_on_lateral_active) {
      status = STATUS_LATERAL_ACTIVE;
    } else {
      status = controls_state.getEnabled() ? STATUS_ENGAGED : STATUS_DISENGAGED;
    }
  }

  // Handle onroad/offroad transition
  if (scene.started != started_prev || sm->frame == 1) {
    if (scene.started) {
      status = STATUS_DISENGAGED;
      scene.started_frame = sm->frame;
    }
    started_prev = scene.started;
    scene.world_objects_visible = false;
    emit offroadTransition(!scene.started);
    wifi->setTetheringEnabled(scene.started && scene.tethering_enabled);
  }
}

UIState::UIState(QObject *parent) : QObject(parent) {
  sm = std::make_unique<SubMaster, const std::initializer_list<const char *>>({
    "modelV2", "controlsState", "liveCalibration", "radarState", "deviceState",
    "pandaStates", "carParams", "driverMonitoringState", "carState", "liveLocationKalman", "driverStateV2",
    "wideRoadCameraState", "managerState", "navInstruction", "navRoute", "uiPlan",
    "frogpilotCarControl", "frogpilotDeviceState", "frogpilotPlan",
  });

  Params params;
  language = QString::fromStdString(params.get("LanguageSetting"));
  auto prime_value = params.get("PrimeType");
  if (!prime_value.empty()) {
    prime_type = static_cast<PrimeType>(std::atoi(prime_value.c_str()));
  }

  // update timer
  timer = new QTimer(this);
  QObject::connect(timer, &QTimer::timeout, this, &UIState::update);
  timer->start(1000 / UI_FREQ);

  wifi = new WifiManager(this);

  ui_update_frogpilot_params(this);
}

void UIState::update() {
  update_sockets(this);
  update_state(this);
  updateStatus();

  if (sm->frame % UI_FREQ == 0) {
    watchdog_kick(nanos_since_boot());
  }
  emit uiUpdate(*this);

  // Update FrogPilot variables when they are changed
  Params paramsMemory{"/dev/shm/params"};
  if (paramsMemory.getBool("FrogPilotTogglesUpdated")) {
    std::thread updateFrogPilotParams(ui_update_frogpilot_params, this);
    updateFrogPilotParams.detach();
  }

  // FrogPilot live variables that need to be constantly checked
  if (scene.conditional_experimental) {
    scene.conditional_status = paramsMemory.getInt("CEStatus");
  }
  if (scene.holiday_themes) {
    scene.current_holiday_theme = paramsMemory.getInt("CurrentHolidayTheme");
  }
}

void UIState::setPrimeType(PrimeType type) {
  if (type != prime_type) {
    bool prev_prime = hasPrime();

    prime_type = type;
    Params().put("PrimeType", std::to_string(prime_type));
    emit primeTypeChanged(prime_type);

    bool prime = hasPrime();
    if (prev_prime != prime) {
      emit primeChanged(prime);
    }
  }
}

Device::Device(QObject *parent) : brightness_filter(BACKLIGHT_OFFROAD, BACKLIGHT_TS, BACKLIGHT_DT), QObject(parent) {
  setAwake(true);
  resetInteractiveTimeout();

  QObject::connect(uiState(), &UIState::uiUpdate, this, &Device::update);
}

void Device::update(const UIState &s) {
  updateBrightness(s);
  updateWakefulness(s);
}

void Device::setAwake(bool on) {
  if (on != awake) {
    awake = on;
    Hardware::set_display_power(awake);
    LOGD("setting display power %d", awake);
    emit displayPowerChanged(awake);
  }
}

void Device::resetInteractiveTimeout(int timeout) {
  if (timeout == -1) {
    timeout = 30;
  }
  interactive_timeout = timeout * UI_FREQ;
}

void Device::updateBrightness(const UIState &s) {
  float clipped_brightness = offroad_brightness;
  if (s.scene.started) {
    clipped_brightness = s.scene.light_sensor;

    // CIE 1931 - https://www.photonstophotos.net/GeneralTopics/Exposure/Psychometric_Lightness_and_Gamma.htm
    if (clipped_brightness <= 8) {
      clipped_brightness = (clipped_brightness / 903.3);
    } else {
      clipped_brightness = std::pow((clipped_brightness + 16.0) / 116.0, 3.0);
    }

    // Scale back to 10% to 100%
    clipped_brightness = std::clamp(100.0f * clipped_brightness, 10.0f, 100.0f);
  }

  int brightness = brightness_filter.update(clipped_brightness);
  if (!awake) {
    brightness = 0;
  }

  if (brightness != last_brightness) {
    if (!brightness_future.isRunning()) {
      brightness_future = QtConcurrent::run(Hardware::set_brightness, brightness);
      last_brightness = brightness;
    }
  }
}

void Device::updateWakefulness(const UIState &s) {
  bool ignition_just_turned_off = !s.scene.ignition && ignition_on;
  ignition_on = s.scene.ignition;

  if (ignition_just_turned_off) {
    resetInteractiveTimeout();
  } else if (interactive_timeout > 0 && --interactive_timeout == 0) {
    emit interactiveTimeout();
  }

  setAwake(s.scene.ignition || interactive_timeout > 0);
}

UIState *uiState() {
  static UIState ui_state;
  return &ui_state;
}

Device *device() {
  static Device _device;
  return &_device;
}
