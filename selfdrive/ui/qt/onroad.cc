#include "selfdrive/ui/qt/onroad.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <memory>
#include <sstream>

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>

#include "common/swaglog.h"
#include "common/timing.h"
#include "selfdrive/ui/qt/util.h"
#ifdef ENABLE_MAPS
#include "selfdrive/ui/qt/maps/map_helpers.h"
#include "selfdrive/ui/qt/maps/map_panel.h"
#endif

static void drawIcon(QPainter &p, const QPoint &center, const QPixmap &img, const QBrush &bg, float opacity, const int angle = 0) {
  p.setRenderHint(QPainter::Antialiasing);
  p.setOpacity(1.0);  // bg dictates opacity of ellipse
  p.setPen(Qt::NoPen);
  p.setBrush(bg);
  p.drawEllipse(center, btn_size / 2, btn_size / 2);
  p.save();
  p.translate(center);
  p.rotate(-angle);
  p.setOpacity(opacity);
  p.drawPixmap(-QPoint(img.width() / 2, img.height() / 2), img);
  p.setOpacity(1.0);
  p.restore();
}

static void drawIconGif(QPainter &p, const QPoint &center, const QMovie &img, const QBrush &bg, float opacity) {
  p.setRenderHint(QPainter::Antialiasing);
  p.setOpacity(1.0);  // bg dictates opacity of ellipse
  p.setPen(Qt::NoPen);
  p.setBrush(bg);
  p.drawEllipse(center.x() - btn_size / 2, center.y() - btn_size / 2, btn_size, btn_size);
  p.setOpacity(opacity);
  QPixmap currentFrame = img.currentPixmap();
  p.drawPixmap(center - QPoint(currentFrame.width() / 2, currentFrame.height() / 2), currentFrame);
  p.setOpacity(1.0);
}

OnroadWindow::OnroadWindow(QWidget *parent) : QWidget(parent), scene(uiState()->scene) {
  QVBoxLayout *main_layout  = new QVBoxLayout(this);
  main_layout->setMargin(UI_BORDER_SIZE);
  QStackedLayout *stacked_layout = new QStackedLayout;
  stacked_layout->setStackingMode(QStackedLayout::StackAll);
  main_layout->addLayout(stacked_layout);

  nvg = new AnnotatedCameraWidget(VISION_STREAM_ROAD, this);

  QWidget * split_wrapper = new QWidget;
  split = new QHBoxLayout(split_wrapper);
  split->setContentsMargins(0, 0, 0, 0);
  split->setSpacing(0);
  split->addWidget(nvg);

  if (getenv("DUAL_CAMERA_VIEW")) {
    CameraWidget *arCam = new CameraWidget("camerad", VISION_STREAM_ROAD, true, this);
    split->insertWidget(0, arCam);
  }

  if (getenv("MAP_RENDER_VIEW")) {
    CameraWidget *map_render = new CameraWidget("navd", VISION_STREAM_MAP, false, this);
    split->insertWidget(0, map_render);
  }

  stacked_layout->addWidget(split_wrapper);

  alerts = new OnroadAlerts(this);
  alerts->setAttribute(Qt::WA_TransparentForMouseEvents, true);
  stacked_layout->addWidget(alerts);

  // setup stacking order
  alerts->raise();

  setAttribute(Qt::WA_OpaquePaintEvent);
  QObject::connect(uiState(), &UIState::uiUpdate, this, &OnroadWindow::updateState);
  QObject::connect(uiState(), &UIState::offroadTransition, this, &OnroadWindow::offroadTransition);
  QObject::connect(uiState(), &UIState::primeChanged, this, &OnroadWindow::primeChanged);

  QObject::connect(&clickTimer, &QTimer::timeout, this, [this]() {
    clickTimer.stop();
    QMouseEvent *event = new QMouseEvent(QEvent::MouseButtonPress, timeoutPoint, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    QApplication::postEvent(this, event);
  });
}

void OnroadWindow::updateState(const UIState &s) {
  if (!s.scene.started) {
    return;
  }

  QColor bgColor = bg_colors[s.status];
  Alert alert = Alert::get(*(s.sm), s.scene.started_frame);
  alerts->updateAlert(alert);

  if (s.scene.map_on_left) {
    split->setDirection(QBoxLayout::LeftToRight);
  } else {
    split->setDirection(QBoxLayout::RightToLeft);
  }

  nvg->updateState(s);

  if (bg != bgColor) {
    // repaint border
    bg = bgColor;
    update();
  }
}

void OnroadWindow::mousePressEvent(QMouseEvent* e) {
  Params params = Params();

  // FrogPilot clickable widgets
  bool widgetClicked = false;

  // Change cruise control increments button
  QRect maxSpeedRect(7, 25, 225, 225);
  bool isMaxSpeedClicked = maxSpeedRect.contains(e->pos());

  // Hide speed button
  QRect hideSpeedRect(rect().center().x() - 175, 50, 350, 350);
  bool isSpeedClicked = hideSpeedRect.contains(e->pos());

  // Speed limit confirmation buttons
  QSize size = this->size();
  QRect leftRect(0, 0, size.width() / 2, size.height());
  QRect rightRect = leftRect.translated(size.width() / 2, 0);
  bool isLeftSideClicked = leftRect.contains(e->pos()) && scene.speed_limit_changed;
  bool isRightSideClicked = rightRect.contains(e->pos()) && scene.speed_limit_changed;

  // Speed limit offset button
  QRect speedLimitRect(7, 250, 225, 225);
  bool isSpeedLimitClicked = speedLimitRect.contains(e->pos());

  if (isMaxSpeedClicked || isSpeedClicked || isSpeedLimitClicked) {
    if (isMaxSpeedClicked && scene.reverse_cruise_ui) {
      bool currentReverseCruise = scene.reverse_cruise;

      uiState()->scene.reverse_cruise = !currentReverseCruise;
      params.putBoolNonBlocking("ReverseCruise", !currentReverseCruise);

    } else if (isSpeedClicked && scene.hide_speed_ui) {
      bool currentHideSpeed = scene.hide_speed;

      uiState()->scene.hide_speed = !currentHideSpeed;
      params.putBoolNonBlocking("HideSpeed", !currentHideSpeed);
    } else if (isSpeedLimitClicked && scene.show_slc_offset_ui) {
      bool currentShowSLCOffset = scene.show_slc_offset;

      uiState()->scene.show_slc_offset = !currentShowSLCOffset;
      params.putBoolNonBlocking("ShowSLCOffset", !currentShowSLCOffset);
    }

    widgetClicked = true;
  } else if (isLeftSideClicked || isRightSideClicked) {
    bool slcConfirmed = isLeftSideClicked && !scene.right_hand_drive || isRightSideClicked && scene.right_hand_drive;
    paramsMemory.putBool("SLCConfirmed", slcConfirmed);
    paramsMemory.putBool("SLCConfirmedPressed", true);

    widgetClicked = true;
  // If the click wasn't for anything specific, change the value of "ExperimentalMode"
  } else if (scene.experimental_mode_via_screen && e->pos() != timeoutPoint) {
    if (clickTimer.isActive()) {
      clickTimer.stop();

      if (scene.conditional_experimental) {
        int override_value = (scene.conditional_status >= 1 && scene.conditional_status <= 6) ? 0 : scene.conditional_status >= 7 ? 1 : 2;
        paramsMemory.putIntNonBlocking("CEStatus", override_value);
      } else {
        bool experimentalMode = params.getBool("ExperimentalMode");
        params.putBoolNonBlocking("ExperimentalMode", !experimentalMode);
      }

    } else {
      clickTimer.start(500);
    }
    widgetClicked = true;
  }

#ifdef ENABLE_MAPS
  if (map != nullptr && !widgetClicked) {
    // Switch between map and sidebar when using navigate on openpilot
    bool sidebarVisible = geometry().x() > 0;
    bool show_map = uiState()->scene.navigate_on_openpilot ? sidebarVisible : !sidebarVisible;
    if (!clickTimer.isActive()) {
      map->setVisible(show_map && !map->isVisible());
      if (scene.full_map) {
        map->setFixedWidth(width());
      } else {
        map->setFixedWidth(topWidget(this)->width() / 2 - UI_BORDER_SIZE);
      }
    }
  }
#endif
  // propagation event to parent(HomeWindow)
  if (!widgetClicked) {
    QWidget::mousePressEvent(e);
  }
}

void OnroadWindow::offroadTransition(bool offroad) {
#ifdef ENABLE_MAPS
  if (!offroad) {
    if (map == nullptr && (uiState()->hasPrime() || !MAPBOX_TOKEN.isEmpty())) {
      auto m = new MapPanel(get_mapbox_settings());
      map = m;

      QObject::connect(m, &MapPanel::mapPanelRequested, this, &OnroadWindow::mapPanelRequested);
      QObject::connect(nvg->map_settings_btn, &MapSettingsButton::clicked, m, &MapPanel::toggleMapSettings);
      QObject::connect(nvg->map_settings_btn_bottom, &MapSettingsButton::clicked, m, &MapPanel::toggleMapSettings);
      nvg->map_settings_btn->setEnabled(true);

      m->setFixedWidth(topWidget(this)->width() / 2 - UI_BORDER_SIZE);
      split->insertWidget(0, m);

      // hidden by default, made visible when navRoute is published
      m->setVisible(false);
    }
  }
#endif

  alerts->updateAlert({});
}

void OnroadWindow::primeChanged(bool prime) {
#ifdef ENABLE_MAPS
  if (map && (!prime && MAPBOX_TOKEN.isEmpty())) {
    nvg->map_settings_btn->setEnabled(false);
    nvg->map_settings_btn->setVisible(false);
    map->deleteLater();
    map = nullptr;
  }
#endif
}

void OnroadWindow::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.fillRect(rect(), QColor(bg.red(), bg.green(), bg.blue(), 255));

  // Draw FPS on screen
  if (scene.fps_counter) {
    updateFPSCounter();

    // Format the FPS string
    QString fpsDisplayString = QString("FPS: %1 (%2) | Min: %3 | Max: %4 | Avg: %5")
      .arg(fps, 0, 'f', 2)
      .arg(paramsMemory.getInt("CameraFPS"))
      .arg(minFPS, 0, 'f', 2)
      .arg(maxFPS, 0, 'f', 2)
      .arg(avgFPS, 0, 'f', 2);

    // Configure the text
    p.setFont(InterFont(30, QFont::DemiBold));
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setPen(Qt::white);

    // Center the text
    QRect currentRect = rect();
    int textWidth = p.fontMetrics().horizontalAdvance(fpsDisplayString);
    int xPos = (currentRect.width() - textWidth) / 2;
    int yPos = currentRect.bottom() - 5;

    // Draw the text
    p.drawText(xPos, yPos, fpsDisplayString);

    update();
  }
}

void OnroadWindow::updateFPSCounter() {
  qint64 currentMillis = QDateTime::currentMSecsSinceEpoch();
  std::queue<std::pair<qint64, double>> fpsQueue = std::queue<std::pair<qint64, double>>();

  minFPS = qMin(minFPS, fps);
  maxFPS = qMax(maxFPS, fps);
  fpsQueue.push({currentMillis, fps});

  while (!fpsQueue.empty() && currentMillis - fpsQueue.front().first > 60000) {
    fpsQueue.pop();
  }

  if (!fpsQueue.empty()) {
    double totalFPS = 0;
    std::queue<std::pair<qint64, double>> tempQueue = fpsQueue;
    while (!tempQueue.empty()) {
      totalFPS += tempQueue.front().second;
      tempQueue.pop();
    }
    avgFPS = totalFPS / fpsQueue.size();
  }
}

// ***** onroad widgets *****

// OnroadAlerts
void OnroadAlerts::updateAlert(const Alert &a) {
  if (!alert.equal(a)) {
    alert = a;
    update();
  }
}

void OnroadAlerts::paintEvent(QPaintEvent *event) {
  if (alert.size == cereal::ControlsState::AlertSize::NONE) {
    return;
  }

  if ((scene.hide_alerts || scene.show_driver_camera) && alert.size == cereal::ControlsState::AlertSize::SMALL) {
    return;
  }

  static std::map<cereal::ControlsState::AlertSize, const int> alert_heights = {
    {cereal::ControlsState::AlertSize::SMALL, 271},
    {cereal::ControlsState::AlertSize::MID, 420},
    {cereal::ControlsState::AlertSize::FULL, height()},
  };
  int h = alert_heights[alert.size];

  int margin = 40;
  int radius = 30;
  int offset = scene.always_on_lateral || scene.conditional_experimental || scene.road_name_ui ? 25 : 0;
  if (alert.size == cereal::ControlsState::AlertSize::FULL) {
    margin = 0;
    radius = 0;
    offset = 0;
  }
  QRect r = QRect(0 + margin, height() - h + margin - offset, width() - margin*2, h - margin*2);

  QPainter p(this);

  // draw background + gradient
  p.setPen(Qt::NoPen);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);
  p.setBrush(QBrush(alert_colors[alert.status]));
  p.drawRoundedRect(r, radius, radius);

  QLinearGradient g(0, r.y(), 0, r.bottom());
  g.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0.05));
  g.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0.35));

  p.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  p.setBrush(QBrush(g));
  p.drawRoundedRect(r, radius, radius);
  p.setCompositionMode(QPainter::CompositionMode_SourceOver);

  // text
  const QPoint c = r.center();
  p.setPen(QColor(0xff, 0xff, 0xff));
  p.setRenderHint(QPainter::TextAntialiasing);
  if (alert.size == cereal::ControlsState::AlertSize::SMALL) {
    p.setFont(InterFont(74, QFont::DemiBold));
    p.drawText(r, Qt::AlignCenter, alert.text1);
  } else if (alert.size == cereal::ControlsState::AlertSize::MID) {
    p.setFont(InterFont(88, QFont::Bold));
    p.drawText(QRect(0, c.y() - 125, width(), 150), Qt::AlignHCenter | Qt::AlignTop, alert.text1);
    p.setFont(InterFont(66));
    p.drawText(QRect(0, c.y() + 21, width(), 90), Qt::AlignHCenter, alert.text2);
  } else if (alert.size == cereal::ControlsState::AlertSize::FULL) {
    bool l = alert.text1.length() > 15;
    p.setFont(InterFont(l ? 132 : 177, QFont::Bold));
    p.drawText(QRect(0, r.y() + (l ? 240 : 270), width(), 600), Qt::AlignHCenter | Qt::TextWordWrap, alert.text1);
    p.setFont(InterFont(88));
    p.drawText(QRect(0, r.height() - (l ? 361 : 420), width(), 300), Qt::AlignHCenter | Qt::TextWordWrap, alert.text2);
  }
}

// ExperimentalButton
ExperimentalButton::ExperimentalButton(QWidget *parent) : experimental_mode(false), engageable(false), QPushButton(parent), scene(uiState()->scene) {
  setFixedSize(btn_size, btn_size + 10);

  engage_img = loadPixmap("../assets/img_chffr_wheel.png", {img_size, img_size});
  experimental_img = loadPixmap("../assets/img_experimental.svg", {img_size, img_size});
  QObject::connect(this, &QPushButton::clicked, this, &ExperimentalButton::changeMode);

  // Custom steering wheel images
  wheelImages = {
    {0, loadPixmap("../assets/img_chffr_wheel.png", {img_size, img_size})},
    {1, loadPixmap("../frogpilot/assets/wheel_images/lexus.png", {img_size, img_size})},
    {2, loadPixmap("../frogpilot/assets/wheel_images/toyota.png", {img_size, img_size})},
    {3, loadPixmap("../frogpilot/assets/wheel_images/frog.png", {img_size, img_size})},
    {4, loadPixmap("../frogpilot/assets/wheel_images/rocket.png", {img_size, img_size})},
    {5, loadPixmap("../frogpilot/assets/wheel_images/hyundai.png", {img_size, img_size})},
    {6, loadPixmap("../frogpilot/assets/wheel_images/stalin.png", {img_size, img_size})},
    {7, loadPixmap("../frogpilot/assets/random_events/images/firefox.png", {img_size, img_size})},
  };

  wheelImagesGif[1] = new QMovie("../frogpilot/assets/random_events/images/weeb_wheel.gif", QByteArray(), this);
}

void ExperimentalButton::changeMode() {
  Params paramsMemory = Params("/dev/shm/params");

  const auto cp = (*uiState()->sm)["carParams"].getCarParams();
  bool can_change = hasLongitudinalControl(cp) && (params.getBool("ExperimentalModeConfirmed") || scene.experimental_mode_via_screen);
  if (can_change) {
    if (scene.conditional_experimental) {
      int override_value = (scene.conditional_status >= 1 && scene.conditional_status <= 4) ? 0 : scene.conditional_status >= 5 ? 3 : 4;
      paramsMemory.putIntNonBlocking("ConditionalStatus", override_value);
    } else {
      params.putBool("ExperimentalMode", !experimental_mode);
    }
  }
}

void ExperimentalButton::updateState(const UIState &s, bool leadInfo) {
  const auto cs = (*s.sm)["controlsState"].getControlsState();
  bool eng = cs.getEngageable() || cs.getEnabled() || scene.always_on_lateral_active;
  if ((cs.getExperimentalMode() != experimental_mode) || (eng != engageable)) {
    engageable = eng;
    experimental_mode = cs.getExperimentalMode();
    update();
  }

  // FrogPilot variables
  firefoxRandomEventTriggered = scene.current_random_event == 1;
  weebRandomEventTriggered = scene.current_random_event == 2;
  rotatingWheel = scene.rotating_wheel;
  wheelIcon = scene.wheel_icon;
  wheelIconGif = 0;

  y_offset = leadInfo ? 10 : 0;

  if (firefoxRandomEventTriggered) {
    static int rotationDegree = 0;
    rotationDegree = (rotationDegree + 36) % 360;
    steeringAngleDeg = rotationDegree;
    wheelIcon = 7;
    update();

  } else if (weebRandomEventTriggered) {
    if (!gifLabel) {
      gifLabel = new QLabel(this);
      QMovie *movie = new QMovie("../frogpilot/assets/random_events/images/weeb_wheel.gif");
      gifLabel->setMovie(movie);
      movie->start();
      gifLabel->resize(img_size, img_size);
    }
    gifLabel->show();
    wheelIconGif = 1;
    update();

  } else {
    if (gifLabel) {
      gifLabel->hide();
    }
    if (rotatingWheel) {
      // Update the icon so the steering wheel rotates in real time
      if (steeringAngleDeg != scene.steering_angle_deg) {
        steeringAngleDeg = scene.steering_angle_deg;
        update();
      }
    } else {
      steeringAngleDeg = 0;
    }
  }
}

void ExperimentalButton::paintEvent(QPaintEvent *event) {
  if (wheelIcon < 0) {
    return;
  }

  QPainter p(this);
  // Custom steering wheel icon
  engage_img = wheelImages[wheelIcon];
  QPixmap img = wheelIcon ? engage_img : (experimental_mode ? experimental_img : engage_img);

  QMovie *gif = wheelImagesGif[wheelIconGif];

  QColor background_color = wheelIcon != 0 && !isDown() && engageable ?
    (scene.always_on_lateral_active ? QColor(10, 186, 181, 255) :
    (scene.conditional_status == 1 ? QColor(255, 246, 0, 255) :
    (experimental_mode ? QColor(218, 111, 37, 241) :
    (scene.navigate_on_openpilot ? QColor(49, 161, 238, 255) : QColor(0, 0, 0, 166)))))) :
    QColor(0, 0, 0, 166);

  if (!(scene.show_driver_camera || scene.map_open && scene.full_map)) {
    if (wheelIconGif != 0) {
      QBrush backgroundBrush(background_color);
      drawIconGif(p, QPoint(btn_size / 2, btn_size / 2 + y_offset), *gif, backgroundBrush, 1.0);
    } else {
      drawIcon(p, QPoint(btn_size / 2, btn_size / 2 + y_offset), img, background_color, (isDown() || !(engageable || scene.always_on_lateral_active)) ? 0.6 : 1.0, steeringAngleDeg);
    }
  }
}


// MapSettingsButton
MapSettingsButton::MapSettingsButton(QWidget *parent) : QPushButton(parent) {
  setFixedSize(btn_size, btn_size + 20);
  settings_img = loadPixmap("../assets/navigation/icon_directions_outlined.svg", {img_size, img_size});

  // hidden by default, made visible if map is created (has prime or mapbox token)
  setVisible(false);
  setEnabled(false);
}

void MapSettingsButton::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  drawIcon(p, QPoint(btn_size / 2, btn_size / 2), settings_img, QColor(0, 0, 0, 166), isDown() ? 0.6 : 1.0);
}


// Window that shows camera view and variety of info drawn on top
AnnotatedCameraWidget::AnnotatedCameraWidget(VisionStreamType type, QWidget* parent) : fps_filter(UI_FREQ, 3, 1. / UI_FREQ), CameraWidget("camerad", type, true, parent), scene(uiState()->scene) {
  pm = std::make_unique<PubMaster, const std::initializer_list<const char *>>({"uiDebug"});

  main_layout = new QVBoxLayout(this);
  main_layout->setMargin(UI_BORDER_SIZE);
  main_layout->setSpacing(0);

  QHBoxLayout *buttons_layout = new QHBoxLayout();
  buttons_layout->setSpacing(0);

  // Neokii screen recorder
  recorder_btn = new ScreenRecorder(this);
  buttons_layout->addWidget(recorder_btn);

  experimental_btn = new ExperimentalButton(this);
  buttons_layout->addWidget(experimental_btn);

  QVBoxLayout *top_right_layout = new QVBoxLayout();
  top_right_layout->setSpacing(0);
  top_right_layout->addLayout(buttons_layout);

  pedal_icons = new PedalIcons(this);
  top_right_layout->addWidget(pedal_icons, 0, Qt::AlignRight);

  main_layout->addLayout(top_right_layout, 0);
  main_layout->setAlignment(top_right_layout, Qt::AlignTop | Qt::AlignRight);

  map_settings_btn = new MapSettingsButton(this);
  main_layout->addWidget(map_settings_btn, 0, Qt::AlignBottom | Qt::AlignRight);

  dm_img = loadPixmap("../assets/img_driver_face.png", {img_size + 5, img_size + 5});

  // Initialize FrogPilot widgets
  initializeFrogPilotWidgets();
}

void AnnotatedCameraWidget::updateState(const UIState &s) {
  const int SET_SPEED_NA = 255;
  const SubMaster &sm = *(s.sm);

  const bool cs_alive = sm.alive("controlsState");
  const bool nav_alive = sm.alive("navInstruction") && sm["navInstruction"].getValid();
  const auto cs = sm["controlsState"].getControlsState();
  const auto car_state = sm["carState"].getCarState();
  const auto nav_instruction = sm["navInstruction"].getNavInstruction();

  // Handle older routes where vCruiseCluster is not set
  float v_cruise = cs.getVCruiseCluster() == 0.0 ? cs.getVCruise() : cs.getVCruiseCluster();
  setSpeed = cs_alive ? v_cruise : SET_SPEED_NA;
  is_cruise_set = setSpeed > 0 && (int)setSpeed != SET_SPEED_NA;
  if (is_cruise_set && !s.scene.is_metric) {
    setSpeed *= KM_TO_MILE;
  }

  // Handle older routes where vEgoCluster is not set
  v_ego_cluster_seen = v_ego_cluster_seen || car_state.getVEgoCluster() != 0.0;
  float v_ego = v_ego_cluster_seen ? car_state.getVEgoCluster() : car_state.getVEgo();
  speed = cs_alive ? std::max<float>(0.0, v_ego) : 0.0;
  speed *= s.scene.is_metric ? MS_TO_KPH : MS_TO_MPH;

  auto speed_limit_sign = nav_instruction.getSpeedLimitSign();
  speedLimit = slcOverridden ? scene.speed_limit_overridden_speed : speedLimitController ? slcSpeedLimit : nav_alive ? nav_instruction.getSpeedLimit() : 0.0;
  speedLimit *= (s.scene.is_metric ? MS_TO_KPH : MS_TO_MPH);
  if (speedLimitController && !slcOverridden) {
    speedLimit = speedLimit - (showSLCOffset ? slcSpeedLimitOffset : 0);
  }

  has_us_speed_limit = (nav_alive && speed_limit_sign == cereal::NavInstruction::SpeedLimitSign::MUTCD) || (speedLimitController && !useViennaSLCSign);
  has_eu_speed_limit = (nav_alive && speed_limit_sign == cereal::NavInstruction::SpeedLimitSign::VIENNA) || (speedLimitController && useViennaSLCSign);
  is_metric = s.scene.is_metric;
  speedUnit =  s.scene.is_metric ? tr("km/h") : tr("mph");
  hideBottomIcons = (cs.getAlertSize() != cereal::ControlsState::AlertSize::NONE || customSignals && (turnSignalLeft || turnSignalRight)) || fullMapOpen || showDriverCamera;
  status = s.status;

  // update engageability/experimental mode button
  experimental_btn->updateState(s, leadInfo);

  // update DM icon
  auto dm_state = sm["driverMonitoringState"].getDriverMonitoringState();
  dmActive = dm_state.getIsActiveMode();
  rightHandDM = dm_state.getIsRHD();
  // DM icon transition
  dm_fade_state = std::clamp(dm_fade_state+0.2*(0.5-dmActive), 0.0, 1.0);

  // hide map settings button for alerts and flip for right hand DM
  if (map_settings_btn->isEnabled()) {
    map_settings_btn->setVisible(!hideBottomIcons && compass && !scene.hide_map_icon);
    main_layout->setAlignment(map_settings_btn, (rightHandDM ? Qt::AlignLeft : Qt::AlignRight) | (compass ? Qt::AlignTop : Qt::AlignBottom));
  }
}

void AnnotatedCameraWidget::drawHud(QPainter &p) {
  p.save();

  // Header gradient
  QLinearGradient bg(0, UI_HEADER_HEIGHT - (UI_HEADER_HEIGHT / 2.5), 0, UI_HEADER_HEIGHT);
  bg.setColorAt(0, QColor::fromRgbF(0, 0, 0, 0.45));
  bg.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
  p.fillRect(0, 0, width(), UI_HEADER_HEIGHT, bg);

  QString speedLimitStr = (speedLimit > 1) ? QString::number(std::nearbyint(speedLimit)) : "–";
  QString speedLimitOffsetStr = slcSpeedLimitOffset == 0 ? "–" : QString::number(slcSpeedLimitOffset, 'f', 0).prepend(slcSpeedLimitOffset > 0 ? "+" : "");
  QString speedStr = QString::number(std::nearbyint(speed));
  QString setSpeedStr = is_cruise_set ? QString::number(std::nearbyint(setSpeed - cruiseAdjustment)) : "–";

  if (!(scene.hide_max_speed)) {
    // Draw outer box + border to contain set speed and speed limit
    const int sign_margin = 12;
    const int us_sign_height = 186;
    const int eu_sign_size = 176;

    const QSize default_size = {172, 204};
    QSize set_speed_size = default_size;
    if (is_metric || has_eu_speed_limit) set_speed_size.rwidth() = 200;
    if (has_us_speed_limit && speedLimitStr.size() >= 3) set_speed_size.rwidth() = 223;

    if (has_us_speed_limit) set_speed_size.rheight() += us_sign_height + sign_margin;
    else if (has_eu_speed_limit) set_speed_size.rheight() += eu_sign_size + sign_margin;

    int top_radius = 32;
    int bottom_radius = has_eu_speed_limit ? 100 : 32;

    QRect set_speed_rect(QPoint(60 + (default_size.width() - set_speed_size.width()) / 2, 45), set_speed_size);
    if (is_cruise_set && cruiseAdjustment) {
      float transition = qBound(0.0f, 4.0f * (cruiseAdjustment / setSpeed), 1.0f);
      QColor min = whiteColor(75);
      QColor max = vtscControllingCurve ? redColor(75) : greenColor(75);

      p.setPen(QPen(QColor::fromRgbF(
        min.redF()   + transition * (max.redF()   - min.redF()),
        min.greenF() + transition * (max.greenF() - min.greenF()),
        min.blueF()  + transition * (max.blueF()  - min.blueF())
      ), 6));
    } else if (scene.reverse_cruise) {
      p.setPen(QPen(QColor(0, 150, 255), 6));
    } else {
      p.setPen(QPen(whiteColor(75), 6));
    }
    p.setBrush(blackColor(166));
    drawRoundedRect(p, set_speed_rect, top_radius, top_radius, bottom_radius, bottom_radius);

    // Draw MAX
    QColor max_color = QColor(0x80, 0xd8, 0xa6, 0xff);
    QColor set_speed_color = whiteColor();
    if (is_cruise_set) {
      if (status == STATUS_DISENGAGED) {
        max_color = whiteColor();
      } else if (status == STATUS_OVERRIDE) {
        max_color = QColor(0x91, 0x9b, 0x95, 0xff);
      } else if (speedLimit > 0) {
        auto interp_color = [=](QColor c1, QColor c2, QColor c3) {
          return speedLimit > 0 ? interpColor(setSpeed, {speedLimit + 5, speedLimit + 15, speedLimit + 25}, {c1, c2, c3}) : c1;
        };
        max_color = interp_color(max_color, QColor(0xff, 0xe4, 0xbf), QColor(0xff, 0xbf, 0xbf));
        set_speed_color = interp_color(set_speed_color, QColor(0xff, 0x95, 0x00), QColor(0xff, 0x00, 0x00));
      }
    } else {
      max_color = QColor(0xa6, 0xa6, 0xa6, 0xff);
      set_speed_color = QColor(0x72, 0x72, 0x72, 0xff);
    }
    p.setFont(InterFont(40, QFont::DemiBold));
    p.setPen(max_color);
    p.drawText(set_speed_rect.adjusted(0, 27, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("MAX"));
    p.setFont(InterFont(90, QFont::Bold));
    p.setPen(set_speed_color);
    p.drawText(set_speed_rect.adjusted(0, 77, 0, 0), Qt::AlignTop | Qt::AlignHCenter, setSpeedStr);

    const QRect sign_rect = set_speed_rect.adjusted(sign_margin, default_size.height(), -sign_margin, -sign_margin);
    // US/Canada (MUTCD style) sign
    if (has_us_speed_limit) {
      p.setPen(Qt::NoPen);
      p.setBrush(whiteColor());
      p.drawRoundedRect(sign_rect, 24, 24);
      p.setPen(QPen(blackColor(), 6));
      p.drawRoundedRect(sign_rect.adjusted(9, 9, -9, -9), 16, 16);

      p.save();
      p.setOpacity(slcOverridden ? 0.25 : 1.0);
      if (speedLimitController && showSLCOffset && !slcOverridden) {
        p.setFont(InterFont(28, QFont::DemiBold));
        p.drawText(sign_rect.adjusted(0, 22, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("LIMIT"));
        p.setFont(InterFont(70, QFont::Bold));
        p.drawText(sign_rect.adjusted(0, 51, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedLimitStr);
        p.setFont(InterFont(50, QFont::DemiBold));
        p.drawText(sign_rect.adjusted(0, 120, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedLimitOffsetStr);
      } else {
        p.setFont(InterFont(28, QFont::DemiBold));
        p.drawText(sign_rect.adjusted(0, 22, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("SPEED"));
        p.drawText(sign_rect.adjusted(0, 51, 0, 0), Qt::AlignTop | Qt::AlignHCenter, tr("LIMIT"));
        p.setFont(InterFont(70, QFont::Bold));
        p.drawText(sign_rect.adjusted(0, 85, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedLimitStr);
      }
      p.restore();
    }

    // EU (Vienna style) sign
    if (has_eu_speed_limit) {
      p.setPen(Qt::NoPen);
      p.setBrush(whiteColor());
      p.drawEllipse(sign_rect);
      p.setPen(QPen(Qt::red, 20));
      p.drawEllipse(sign_rect.adjusted(16, 16, -16, -16));

      p.save();
      p.setOpacity(slcOverridden ? 0.25 : 1.0);
      p.setPen(blackColor());
      if (showSLCOffset) {
        p.setFont(InterFont((speedLimitStr.size() >= 3) ? 60 : 70, QFont::Bold));
        p.drawText(sign_rect.adjusted(0, -25, 0, 0), Qt::AlignCenter, speedLimitStr);
        p.setFont(InterFont(40, QFont::DemiBold));
        p.drawText(sign_rect.adjusted(0, 100, 0, 0), Qt::AlignTop | Qt::AlignHCenter, speedLimitOffsetStr);
      } else {
        p.setFont(InterFont((speedLimitStr.size() >= 3) ? 60 : 70, QFont::Bold));
        p.drawText(sign_rect, Qt::AlignCenter, speedLimitStr);
      }
      p.restore();
    }
    p.restore();
  }

  // current speed
  if (!(scene.hide_speed || fullMapOpen)) {
    p.setFont(InterFont(176, QFont::Bold));
    drawText(p, rect().center().x(), 210, speedStr);
    p.setFont(InterFont(66));
    drawText(p, rect().center().x(), 290, speedUnit, 200);
  }

  p.restore();
}

void AnnotatedCameraWidget::drawText(QPainter &p, int x, int y, const QString &text, int alpha) {
  QRect real_rect = p.fontMetrics().boundingRect(text);
  real_rect.moveCenter({x, y - real_rect.height() / 2});

  p.setPen(QColor(0xff, 0xff, 0xff, alpha));
  p.drawText(real_rect.x(), real_rect.bottom(), text);
}

void AnnotatedCameraWidget::initializeGL() {
  CameraWidget::initializeGL();
  qInfo() << "OpenGL version:" << QString((const char*)glGetString(GL_VERSION));
  qInfo() << "OpenGL vendor:" << QString((const char*)glGetString(GL_VENDOR));
  qInfo() << "OpenGL renderer:" << QString((const char*)glGetString(GL_RENDERER));
  qInfo() << "OpenGL language version:" << QString((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

  prev_draw_t = millis_since_boot();
  setBackgroundColor(bg_colors[STATUS_DISENGAGED]);
}

void AnnotatedCameraWidget::updateFrameMat() {
  CameraWidget::updateFrameMat();
  UIState *s = uiState();
  int w = width(), h = height();

  s->fb_w = w;
  s->fb_h = h;

  // Apply transformation such that video pixel coordinates match video
  // 1) Put (0, 0) in the middle of the video
  // 2) Apply same scaling as video
  // 3) Put (0, 0) in top left corner of video
  s->car_space_transform.reset();
  s->car_space_transform.translate(w / 2 - x_offset, h / 2 - y_offset)
      .scale(zoom, zoom)
      .translate(-intrinsic_matrix.v[2], -intrinsic_matrix.v[5]);
}

void AnnotatedCameraWidget::drawLaneLines(QPainter &painter, const UIState *s) {
  painter.save();

  SubMaster &sm = *(s->sm);

  // lanelines
  for (int i = 0; i < std::size(scene.lane_line_vertices); ++i) {
    if (currentHolidayTheme != 0) {
      painter.setBrush(std::get<3>(holidayThemeConfiguration[currentHolidayTheme]).begin()->second);
    } else if (customColors != 0) {
      painter.setBrush(std::get<3>(themeConfiguration[customColors]).begin()->second);
    } else {
      painter.setBrush(QColor::fromRgbF(1.0, 1.0, 1.0, std::clamp<float>(scene.lane_line_probs[i], 0.0, 0.7)));
    }
    painter.drawPolygon(scene.lane_line_vertices[i]);
  }

  // road edges
  for (int i = 0; i < std::size(scene.road_edge_vertices); ++i) {
    if (currentHolidayTheme != 0) {
      painter.setBrush(std::get<3>(holidayThemeConfiguration[currentHolidayTheme]).begin()->second);
    } else if (customColors != 0) {
      painter.setBrush(std::get<3>(themeConfiguration[customColors]).begin()->second);
    } else {
      painter.setBrush(QColor::fromRgbF(1.0, 0, 0, std::clamp<float>(1.0 - scene.road_edge_stds[i], 0.0, 1.0)));
    }
    painter.drawPolygon(scene.road_edge_vertices[i]);
  }

  // paint path
  QLinearGradient bg(0, height(), 0, 0);
  if (sm["controlsState"].getControlsState().getExperimentalMode() || scene.acceleration_path) {
    // The first half of track_vertices are the points for the right side of the path
    // and the indices match the positions of accel from uiPlan
    const auto &acceleration_const = sm["uiPlan"].getUiPlan().getAccel();
    const int max_len = std::min<int>(scene.track_vertices.length() / 2, acceleration_const.size());

    // Copy of the acceleration vector
    std::vector<float> acceleration;
    for (int i = 0; i < acceleration_const.size(); i++) {
      acceleration.push_back(acceleration_const[i]);
    }

    for (int i = 0; i < max_len; ++i) {
      // Some points are out of frame
      if (scene.track_vertices[i].y() < 0 || scene.track_vertices[i].y() > height()) continue;

      // Flip so 0 is bottom of frame
      float lin_grad_point = (height() - scene.track_vertices[i].y()) / height();

      // If acceleration is between -0.2 and 0.2, resort to the theme color
      if (std::abs(acceleration[i]) < 0.2 && (currentHolidayTheme != 0)) {
        const auto &colorMap = std::get<3>(holidayThemeConfiguration[currentHolidayTheme]);
        for (const auto &[position, brush] : colorMap) {
          bg.setColorAt(position, brush.color());
        }
      } else if (std::abs(acceleration[i]) < 0.2 && (customColors != 0)) {
        const auto &colorMap = std::get<3>(themeConfiguration[customColors]);
        for (const auto &[position, brush] : colorMap) {
          bg.setColorAt(position, brush.color());
        }
      } else {
        // speed up: 120, slow down: 0
        float path_hue = fmax(fmin(60 + acceleration[i] * 35, 120), 0);
        // FIXME: painter.drawPolygon can be slow if hue is not rounded
        path_hue = int(path_hue * 100 + 0.5) / 100;

        float saturation = fmin(fabs(acceleration[i] * 1.5), 1);
        float lightness = util::map_val(saturation, 0.0f, 1.0f, 0.95f, 0.62f);  // lighter when grey
        float alpha = util::map_val(lin_grad_point, 0.75f / 2.f, 0.75f, 0.4f, 0.0f);  // matches previous alpha fade
        bg.setColorAt(lin_grad_point, QColor::fromHslF(path_hue / 360., saturation, lightness, alpha));

        // Skip a point, unless next is last
        i += (i + 2) < max_len ? 1 : 0;
      }
    }

  } else if (currentHolidayTheme != 0) {
    const auto &colorMap = std::get<3>(holidayThemeConfiguration[currentHolidayTheme]);
    for (const auto &[position, brush] : colorMap) {
      bg.setColorAt(position, brush.color());
    }
  } else if (customColors != 0) {
    const auto &colorMap = std::get<3>(themeConfiguration[customColors]);
    for (const auto &[position, brush] : colorMap) {
      bg.setColorAt(position, brush.color());
    }
  } else {
    bg.setColorAt(0.0, QColor::fromHslF(148 / 360., 0.94, 0.51, 0.4));
    bg.setColorAt(0.5, QColor::fromHslF(112 / 360., 1.0, 0.68, 0.35));
    bg.setColorAt(1.0, QColor::fromHslF(112 / 360., 1.0, 0.68, 0.0));
  }

  painter.setBrush(bg);
  painter.drawPolygon(scene.track_vertices);

  // Create new path with track vertices and track edge vertices
  QPainterPath path;
  path.addPolygon(scene.track_vertices);
  path.addPolygon(scene.track_edge_vertices);

  // Paint path edges
  QLinearGradient pe(0, height(), 0, 0);
  if (alwaysOnLateralActive) {
    pe.setColorAt(0.0, QColor::fromHslF(178 / 360., 0.90, 0.38, 1.0));
    pe.setColorAt(0.5, QColor::fromHslF(178 / 360., 0.90, 0.38, 0.5));
    pe.setColorAt(1.0, QColor::fromHslF(178 / 360., 0.90, 0.38, 0.1));
  } else if (conditionalStatus == 1 || conditionalStatus == 3) {
    pe.setColorAt(0.0, QColor::fromHslF(58 / 360., 1.00, 0.50, 1.0));
    pe.setColorAt(0.5, QColor::fromHslF(58 / 360., 1.00, 0.50, 0.5));
    pe.setColorAt(1.0, QColor::fromHslF(58 / 360., 1.00, 0.50, 0.1));
  } else if (experimentalMode) {
    pe.setColorAt(0.0, QColor::fromHslF(25 / 360., 0.71, 0.50, 1.0));
    pe.setColorAt(0.5, QColor::fromHslF(25 / 360., 0.71, 0.50, 0.5));
    pe.setColorAt(1.0, QColor::fromHslF(25 / 360., 0.71, 0.50, 0.1));
  } else if (scene.navigate_on_openpilot) {
    pe.setColorAt(0.0, QColor::fromHslF(205 / 360., 0.85, 0.56, 1.0));
    pe.setColorAt(0.5, QColor::fromHslF(205 / 360., 0.85, 0.56, 0.5));
    pe.setColorAt(1.0, QColor::fromHslF(205 / 360., 0.85, 0.56, 0.1));
  } else if (currentHolidayTheme != 0) {
    const auto &colorMap = std::get<3>(holidayThemeConfiguration[currentHolidayTheme]);
    for (const auto &[position, brush] : colorMap) {
      QColor darkerColor = brush.color().darker(120);
      pe.setColorAt(position, darkerColor);
    }
  } else if (customColors != 0) {
    const auto &colorMap = std::get<3>(themeConfiguration[customColors]);
    for (const auto &[position, brush] : colorMap) {
      QColor darkerColor = brush.color().darker(120);
      pe.setColorAt(position, darkerColor);
    }
  } else {
    pe.setColorAt(0.0, QColor::fromHslF(148 / 360., 0.94, 0.51, 1.0));
    pe.setColorAt(0.5, QColor::fromHslF(112 / 360., 1.00, 0.68, 0.5));
    pe.setColorAt(1.0, QColor::fromHslF(112 / 360., 1.00, 0.68, 0.1));
  }

  painter.setBrush(pe);
  painter.drawPath(path);

  // Paint blindspot path
  if (scene.blind_spot_path) {
    QLinearGradient bs(0, height(), 0, 0);
    if (blindSpotLeft || blindSpotRight) {
      bs.setColorAt(0.0, QColor::fromHslF(0 / 360., 0.75, 0.50, 0.6));
      bs.setColorAt(0.5, QColor::fromHslF(0 / 360., 0.75, 0.50, 0.4));
      bs.setColorAt(1.0, QColor::fromHslF(0 / 360., 0.75, 0.50, 0.2));
    }

    painter.setBrush(bs);
    if (blindSpotLeft) {
      painter.drawPolygon(scene.track_adjacent_vertices[4]);
    }
    if (blindSpotRight) {
      painter.drawPolygon(scene.track_adjacent_vertices[5]);
    }
  }

  // Paint adjacent lane paths
  if (scene.adjacent_path && (laneWidthLeft != 0 || laneWidthRight != 0)) {
    // Set up the units
    double distanceValue = is_metric ? 1.0 : METER_TO_FOOT;
    QString unit_d = is_metric ? " meters" : " feet";

    // Declare the lane width thresholds
    constexpr float minLaneWidth = 2.0f;
    constexpr float maxLaneWidth = 4.0f;

    // Set gradient colors based on laneWidth and blindspot
    auto setGradientColors = [](QLinearGradient &gradient, float laneWidth, bool blindspot) {
      // Make the path red for smaller paths or if there's a car in the blindspot and green for larger paths
      double hue = (laneWidth < minLaneWidth || laneWidth > maxLaneWidth || blindspot)
                         ? 0.0 : 120.0 * (laneWidth - minLaneWidth) / (maxLaneWidth - minLaneWidth);
      double hue_ratio = hue / 360.0;
      gradient.setColorAt(0.0, QColor::fromHslF(hue_ratio, 0.75, 0.50, 0.6));
      gradient.setColorAt(0.5, QColor::fromHslF(hue_ratio, 0.75, 0.50, 0.4));
      gradient.setColorAt(1.0, QColor::fromHslF(hue_ratio, 0.75, 0.50, 0.2));
    };

    // Paint the lanes
    auto paintLane = [&](QPainter &painter, const QPolygonF &lane, float laneWidth, bool blindspot) {
      QLinearGradient gradient(0, height(), 0, 0);
      setGradientColors(gradient, laneWidth, blindspot);

      painter.setFont(InterFont(30, QFont::DemiBold));
      painter.setBrush(gradient);
      painter.setPen(Qt::transparent);
      painter.drawPolygon(lane);
      painter.setPen(Qt::white);

      QRectF boundingRect = lane.boundingRect();
      if (scene.adjacent_path_metrics) {
        painter.drawText(boundingRect.center(),
                         blindspot ? "Vehicle in blind spot" :
                         QString("%1%2").arg(laneWidth * distanceValue, 0, 'f', 2).arg(unit_d));
      }
      painter.setPen(Qt::NoPen);
    };

    paintLane(painter, scene.track_adjacent_vertices[4], laneWidthLeft, blindSpotLeft);
    paintLane(painter, scene.track_adjacent_vertices[5], laneWidthRight, blindSpotRight);
  }

  painter.restore();
}

void AnnotatedCameraWidget::drawDriverState(QPainter &painter, const UIState *s) {
  painter.save();

  // base icon
  int offset = UI_BORDER_SIZE + btn_size / 2;
  offset += alwaysOnLateral || conditionalExperimental || roadNameUI ? 25 : 0;
  int x = rightHandDM ? width() - offset : offset;
  x += onroadAdjustableProfiles ? 250 : 0;
  int y = height() - offset;
  float opacity = dmActive ? 0.65 : 0.2;
  drawIcon(painter, QPoint(x, y), dm_img, blackColor(70), opacity);

  // face
  QPointF face_kpts_draw[std::size(default_face_kpts_3d)];
  float kp;
  for (int i = 0; i < std::size(default_face_kpts_3d); ++i) {
    kp = (scene.face_kpts_draw[i].v[2] - 8) / 120 + 1.0;
    face_kpts_draw[i] = QPointF(scene.face_kpts_draw[i].v[0] * kp + x, scene.face_kpts_draw[i].v[1] * kp + y);
  }

  painter.setPen(QPen(QColor::fromRgbF(1.0, 1.0, 1.0, opacity), 5.2, Qt::SolidLine, Qt::RoundCap));
  painter.drawPolyline(face_kpts_draw, std::size(default_face_kpts_3d));

  // tracking arcs
  const int arc_l = 133;
  const float arc_t_default = 6.7;
  const float arc_t_extend = 12.0;
  QColor arc_color = QColor::fromRgbF(0.545 - 0.445 * s->engaged(),
                                      0.545 + 0.4 * s->engaged(),
                                      0.545 - 0.285 * s->engaged(),
                                      0.4 * (1.0 - dm_fade_state));
  float delta_x = -scene.driver_pose_sins[1] * arc_l / 2;
  float delta_y = -scene.driver_pose_sins[0] * arc_l / 2;
  painter.setPen(QPen(arc_color, arc_t_default+arc_t_extend*fmin(1.0, scene.driver_pose_diff[1] * 5.0), Qt::SolidLine, Qt::RoundCap));
  painter.drawArc(QRectF(std::fmin(x + delta_x, x), y - arc_l / 2, fabs(delta_x), arc_l), (scene.driver_pose_sins[1]>0 ? 90 : -90) * 16, 180 * 16);
  painter.setPen(QPen(arc_color, arc_t_default+arc_t_extend*fmin(1.0, scene.driver_pose_diff[0] * 5.0), Qt::SolidLine, Qt::RoundCap));
  painter.drawArc(QRectF(x - arc_l / 2, std::fmin(y + delta_y, y), arc_l, fabs(delta_y)), (scene.driver_pose_sins[0]>0 ? 0 : 180) * 16, 180 * 16);

  painter.restore();
}

void AnnotatedCameraWidget::drawLead(QPainter &painter, const cereal::RadarState::LeadData::Reader &lead_data, const QPointF &vd) {
  painter.save();

  const float speedBuff = customColors ? 25. : 10.;  // Make the center of the chevron appear sooner if a custom theme is active
  const float leadBuff = customColors ? 100. : 40.;  // Make the center of the chevron appear sooner if a custom theme is active
  const float d_rel = lead_data.getDRel();
  const float v_rel = lead_data.getVRel();

  float fillAlpha = 0;
  if (d_rel < leadBuff) {
    fillAlpha = 255 * (1.0 - (d_rel / leadBuff));
    if (v_rel < 0) {
      fillAlpha += 255 * (-1 * (v_rel / speedBuff));
    }
    fillAlpha = (int)(fmin(fillAlpha, 255));
  }

  float sz = std::clamp((25 * 30) / (d_rel / 3 + 30), 15.0f, 30.0f) * 2.35;
  float x = std::clamp((float)vd.x(), 0.f, width() - sz / 2);
  float y = std::fmin(height() - sz * .6, (float)vd.y());

  float g_xo = sz / 5;
  float g_yo = sz / 10;

  QPointF glow[] = {{x + (sz * 1.35) + g_xo, y + sz + g_yo}, {x, y - g_yo}, {x - (sz * 1.35) - g_xo, y + sz + g_yo}};
  painter.setBrush(QColor(218, 202, 37, 255));
  painter.drawPolygon(glow, std::size(glow));

  // chevron
  QPointF chevron[] = {{x + (sz * 1.25), y + sz}, {x, y}, {x - (sz * 1.25), y + sz}};
  if (currentHolidayTheme != 0) {
    painter.setBrush(std::get<3>(holidayThemeConfiguration[currentHolidayTheme]).begin()->second);
  } else if (customColors != 0) {
    painter.setBrush(std::get<3>(themeConfiguration[customColors]).begin()->second);
  } else {
    painter.setBrush(redColor(fillAlpha));
  }
  painter.drawPolygon(chevron, std::size(chevron));

  // Add lead info
  if (leadInfo) {
    // Declare the variables
    float lead_speed = std::max(lead_data.getVLead(), 0.0f);  // Ensure lead speed doesn't go under 0 m/s cause that's dumb

    // Form the text and center it below the chevron
    painter.setPen(Qt::white);
    painter.setFont(InterFont(35, QFont::Bold));

    QString text = QString("%1 %2 | %3 %4")
                           .arg(qRound(d_rel * distanceConversion))
                           .arg(leadDistanceUnit)
                           .arg(qRound(lead_speed * speedConversion))
                           .arg(leadSpeedUnit);

    // Calculate the text starting position
    QFontMetrics metrics(painter.font());
    int middle_x = (chevron[2].x() + chevron[0].x()) / 2;
    int textWidth = metrics.horizontalAdvance(text);
    painter.drawText(middle_x - textWidth / 2, chevron[0].y() + metrics.height() + 5, text);
  }

  painter.restore();
}

void AnnotatedCameraWidget::paintGL() {
}

void AnnotatedCameraWidget::paintEvent(QPaintEvent *event) {
  UIState *s = uiState();
  SubMaster &sm = *(s->sm);
  QPainter painter(this);
  const double start_draw_t = millis_since_boot();
  const cereal::ModelDataV2::Reader &model = sm["modelV2"].getModelV2();

  // draw camera frame
  {
    std::lock_guard lk(frame_lock);

    if (frames.empty()) {
      if (skip_frame_count > 0) {
        skip_frame_count--;
        qDebug() << "skipping frame, not ready";
        return;
      }
    } else {
      // skip drawing up to this many frames if we're
      // missing camera frames. this smooths out the
      // transitions from the narrow and wide cameras
      skip_frame_count = 5;
    }

    // Wide or narrow cam dependent on speed
    bool has_wide_cam = available_streams.count(VISION_STREAM_WIDE_ROAD);
    if (has_wide_cam && cameraView == 0) {
      float v_ego = sm["carState"].getCarState().getVEgo();
      if ((v_ego < 10) || available_streams.size() == 1) {
        wide_cam_requested = true;
      } else if (v_ego > 15) {
        wide_cam_requested = false;
      }
      wide_cam_requested = wide_cam_requested && sm["controlsState"].getControlsState().getExperimentalMode();
      // for replay of old routes, never go to widecam
      wide_cam_requested = wide_cam_requested && s->scene.calibration_wide_valid;
    }
    CameraWidget::setStreamType(cameraView == 3 || showDriverCamera ? VISION_STREAM_DRIVER :
                                cameraView == 2 || wide_cam_requested ? VISION_STREAM_WIDE_ROAD :
                                VISION_STREAM_ROAD);
    }

  s->scene.wide_cam = CameraWidget::getStreamType() == VISION_STREAM_WIDE_ROAD;
  if (s->scene.calibration_valid) {
    auto calib = s->scene.wide_cam ? s->scene.view_from_wide_calib : s->scene.view_from_calib;
    CameraWidget::updateCalibration(calib);
  } else {
    CameraWidget::updateCalibration(DEFAULT_CALIBRATION);
  }
  painter.beginNativePainting();
  CameraWidget::setFrameId(model.getFrameId());
  CameraWidget::paintGL();
  painter.endNativePainting();

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);

  if (s->scene.world_objects_visible && !showDriverCamera) {
    update_model(s, model, sm["uiPlan"].getUiPlan());
    drawLaneLines(painter, s);

    if (s->scene.longitudinal_control && sm.rcv_frame("radarState") > s->scene.started_frame && !scene.hide_lead_marker) {
      auto radar_state = sm["radarState"].getRadarState();
      update_leads(s, radar_state, model.getPosition());
      auto lead_one = radar_state.getLeadOne();
      auto lead_two = radar_state.getLeadTwo();
      if (lead_one.getStatus()) {
        drawLead(painter, lead_one, s->scene.lead_vertices[0]);
      }
      if (lead_two.getStatus() && (std::abs(lead_one.getDRel() - lead_two.getDRel()) > 3.0)) {
        drawLead(painter, lead_two, s->scene.lead_vertices[1]);
      }
    }
  }

  // DMoji
  if (!hideBottomIcons && (sm.rcv_frame("driverStateV2") > s->scene.started_frame)) {
    update_dmonitoring(s, sm["driverStateV2"].getDriverStateV2(), dm_fade_state, rightHandDM);
    drawDriverState(painter, s);
  }

  drawHud(painter);

  double cur_draw_t = millis_since_boot();
  double dt = cur_draw_t - prev_draw_t;
  fps = fps_filter.update(1. / dt * 1000);
  if (fps < 15) {
    LOGW("slow frame rate: %.2f fps", fps);
  }
  prev_draw_t = cur_draw_t;

  // publish debug msg
  MessageBuilder msg;
  auto m = msg.initEvent().initUiDebug();
  m.setDrawTimeMillis(cur_draw_t - start_draw_t);
  pm->send("uiDebug", msg);

  // Update FrogPilot widgets
  updateFrogPilotWidgets(painter);
}

void AnnotatedCameraWidget::showEvent(QShowEvent *event) {
  CameraWidget::showEvent(event);

  ui_update_params(uiState());
  prev_draw_t = millis_since_boot();
}

// FrogPilot widgets
void AnnotatedCameraWidget::initializeFrogPilotWidgets() {
  bottom_layout = new QHBoxLayout();

  personality_btn = new PersonalityButton(this);
  bottom_layout->addWidget(personality_btn);

  QSpacerItem *spacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
  bottom_layout->addItem(spacer);

  compass_img = new Compass(this);
  bottom_layout->addWidget(compass_img);

  map_settings_btn_bottom = new MapSettingsButton(this);
  bottom_layout->addWidget(map_settings_btn_bottom);

  main_layout->addLayout(bottom_layout);

  // Custom themes configuration
  themeConfiguration = {
    {1, {"frog_theme", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                     {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                     {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
    {2, {"tesla_theme", 4, QColor(0, 72, 255, 255), {{0.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.9))},
                                                     {0.5, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.5))},
                                                     {1.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.1))}}}},
    {3, {"stalin_theme", 6, QColor(255, 0, 0, 255), {{0.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.9))},
                                                     {0.5, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.5))},
                                                     {1.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.1))}}}},
  };

  // Holiday themes configuration
  holidayThemeConfiguration = {
    {1, {"april_fools", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                      {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                      {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
    {2, {"christmas", 4, QColor(0, 72, 255, 255), {{0.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.9))},
                                                   {0.5, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.5))},
                                                   {1.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.1))}}}},
    {3, {"cinco_de_mayo", 6, QColor(255, 0, 0, 255), {{0.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.9))},
                                                      {0.5, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.5))},
                                                      {1.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.1))}}}},
    {4, {"easter", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                 {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                 {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
    {5, {"fourth_of_july", 4, QColor(0, 72, 255, 255), {{0.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.9))},
                                                        {0.5, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.5))},
                                                        {1.0, QBrush(QColor::fromHslF(223 / 360., 1.0, 0.5, 0.1))}}}},
    {6, {"halloween", 6, QColor(255, 0, 0, 255), {{0.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.9))},
                                                  {0.5, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.5))},
                                                  {1.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.1))}}}},
    {7, {"new_years_day", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                        {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                        {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
    {8, {"st_patricks_day", 4, QColor(0, 128, 0, 255), {{0.0, QBrush(QColor::fromHslF(120 / 360., 1.0, 0.25, 0.9))},
                                                        {0.5, QBrush(QColor::fromHslF(120 / 360., 1.0, 0.25, 0.5))},
                                                        {1.0, QBrush(QColor::fromHslF(120 / 360., 1.0, 0.25, 0.1))}}}},
    {9, {"thanksgiving", 6, QColor(255, 0, 0, 255), {{0.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.9))},
                                                     {0.5, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.5))},
                                                     {1.0, QBrush(QColor::fromHslF(0 / 360., 1.0, 0.5, 0.1))}}}},
    {10, {"valentines_day", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                          {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                          {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
    {11, {"world_frog_day", 4, QColor(23, 134, 68, 242), {{0.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.9))},
                                                          {0.5, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.5))},
                                                          {1.0, QBrush(QColor::fromHslF(144 / 360., 0.71, 0.31, 0.1))}}}},
  };

  // Initialize the timer for the turn signal animation
  animationTimer = new QTimer(this);
  connect(animationTimer, &QTimer::timeout, this, [this] {
    animationFrameIndex = (animationFrameIndex + 1) % totalFrames;
  });

  // Initialize the timer for the screen recorder
  QTimer *record_timer = new QTimer(this);
  connect(record_timer, &QTimer::timeout, this, [this]() {
    if (recorder_btn) {
      recorder_btn->update_screen();
    }
  });
  record_timer->start(1000 / UI_FREQ);
}

void AnnotatedCameraWidget::updateFrogPilotWidgets(QPainter &p) {
  alwaysOnLateral = scene.always_on_lateral && !scene.hide_aol_status_bar;
  alwaysOnLateralActive = scene.always_on_lateral_active;

  blindSpotLeft = scene.blind_spot_left;
  blindSpotRight = scene.blind_spot_right;

  cameraView = scene.camera_view;

  compass = scene.compass;

  conditionalExperimental = scene.conditional_experimental && !scene.hide_cem_status_bar;
  conditionalSpeed = scene.conditional_speed;
  conditionalSpeedLead = scene.conditional_speed_lead;
  conditionalStatus = scene.conditional_status;

  bool disableSmoothing = scene.vtsc_controlling_curve ? scene.disable_smoothing_vtsc : scene.disable_smoothing_mtsc;
  cruiseAdjustment = disableSmoothing ? fmax(setSpeed - scene.adjusted_cruise, 0) : fmax(0.25 * (setSpeed - scene.adjusted_cruise) + 0.75 * cruiseAdjustment - 1, 0);

  customColors = scene.custom_colors;

  experimentalMode = scene.experimental_mode;

  laneWidthLeft = scene.lane_width_left;
  laneWidthRight = scene.lane_width_right;

  leadInfo = scene.lead_info;
  obstacleDistance = scene.obstacle_distance;
  obstacleDistanceStock = scene.obstacle_distance_stock;

  mapOpen = scene.map_open;
  fullMapOpen = mapOpen && scene.full_map;

  onroadAdjustableProfiles = scene.personalities_via_screen;

  pedalsOnUI = scene.pedals_on_ui;

  rightHandDrive = scene.right_hand_drive;

  roadNameUI = scene.road_name_ui;

  showDriverCamera = scene.show_driver_camera;

  speedLimitController = scene.speed_limit_controller;
  showSLCOffset = speedLimitController && scene.show_slc_offset;
  slcOverridden = speedLimitController && scene.speed_limit_overridden;
  slcSpeedLimit = scene.speed_limit;
  slcSpeedLimitOffset = scene.speed_limit_offset * (is_metric ? MS_TO_KPH : MS_TO_MPH);
  useViennaSLCSign = scene.use_vienna_slc_sign;

  turnSignalLeft = scene.turn_signal_left;
  turnSignalRight = scene.turn_signal_right;

  if (!(showDriverCamera || fullMapOpen)) {
    if (leadInfo) {
      drawLeadInfo(p);
    }

    if (alwaysOnLateral || conditionalExperimental || roadNameUI) {
      drawStatusBar(p);
    }

    if (customSignals && (turnSignalLeft || turnSignalRight)) {
      if (!animationTimer->isActive()) {
        animationTimer->start(totalFrames * 11);  // 440 milliseconds per loop; syncs up perfectly with my 2019 Lexus ES 350 turn signal clicks
      }
      drawTurnSignals(p);
    } else if (animationTimer->isActive()) {
      animationTimer->stop();
    }

    if (scene.speed_limit_changed) {
      drawSLCConfirmation(p);
    }
  }

  bool enableCompass = compass && !hideBottomIcons;
  compass_img->setVisible(enableCompass);
  if (enableCompass) {
    compass_img->updateState(scene.bearing_deg);
    bottom_layout->setAlignment(compass_img, (rightHandDM ? Qt::AlignLeft : Qt::AlignRight));
  }

  bool enablePedalIcons = pedalsOnUI && !fullMapOpen;
  pedal_icons->setVisible(enablePedalIcons);
  if (enablePedalIcons) {
    pedal_icons->updateState();
  }

  bool enablePersonalityButton = onroadAdjustableProfiles && !hideBottomIcons;
  personality_btn->setVisible(enablePersonalityButton);
  if (enablePersonalityButton) {
    if (paramsMemory.getBool("PersonalityChangedViaWheel")) {
      personality_btn->checkUpdate();
    }
    bottom_layout->setAlignment(personality_btn, (rightHandDM ? Qt::AlignRight : Qt::AlignLeft));
  }

  map_settings_btn_bottom->setEnabled(map_settings_btn->isEnabled());
  if (map_settings_btn_bottom->isEnabled()) {
    map_settings_btn_bottom->setVisible(!hideBottomIcons && !compass && !scene.hide_map_icon);
    bottom_layout->setAlignment(map_settings_btn_bottom, rightHandDM ? Qt::AlignLeft : Qt::AlignRight);
  }

  recorder_btn->setVisible(scene.screen_recorder && !mapOpen);

  // Update the turn signal animation images upon toggle change
  if (customSignals != scene.custom_signals || currentHolidayTheme != scene.current_holiday_theme) {
    currentHolidayTheme = scene.current_holiday_theme;
    customSignals = scene.custom_signals;

    if (currentHolidayTheme != 0) {
      auto themeConfigIt = holidayThemeConfiguration.find(currentHolidayTheme);
      QString themeConfigKey = themeConfigIt != holidayThemeConfiguration.end() ? std::get<0>(themeConfigIt->second) : "";
      themePath = QString("../frogpilot/assets/holiday_themes/%1/images").arg(themeConfigKey);
      availableImages = std::get<1>(themeConfigIt->second);
    } else {
      auto themeConfigIt = themeConfiguration.find(customSignals);
      QString themeConfigKey = themeConfigIt != themeConfiguration.end() ? std::get<0>(themeConfigIt->second) : "";
      themePath = QString("../frogpilot/assets/custom_themes/%1/images").arg(themeConfigKey);
      availableImages = std::get<1>(themeConfigIt->second);
    }

    for (int i = 1; i <= totalFrames; ++i) {
      int imageIndex = ((i - 1) % availableImages) + 1;
      QString imagePath = themePath + QString("/turn_signal_%1.png").arg(imageIndex);
      imagePaths.push_back(imagePath);
    }

    signalImgVector.clear();
    signalImgVector.reserve(2 * imagePaths.size());  // Reserve space for both regular and flipped images
    for (const QString &imagePath : imagePaths) {
      QPixmap pixmap(imagePath);
      signalImgVector.push_back(pixmap);  // Regular image
      signalImgVector.push_back(pixmap.transformed(QTransform().scale(-1, 1)));  // Flipped image
    }

    signalImgVector.push_back(QPixmap(themePath + "/turn_signal_1_red.png"));  // Regular blindspot image
    signalImgVector.push_back(QPixmap(themePath + "/turn_signal_1_red.png").transformed(QTransform().scale(-1, 1)));  // Flipped blindspot image
  }
}

Compass::Compass(QWidget *parent) : QWidget(parent) {
  setFixedSize(btn_size * 1.5, btn_size * 1.5);

  compassSize = btn_size;
  circleOffset = compassSize / 2;
  degreeLabelOffset = circleOffset + 25;
  innerCompass = compassSize / 2;

  x = (btn_size * 1.5) / 2 + 20;
  y = (btn_size * 1.5) / 2;

  compassInnerImg = loadPixmap("../frogpilot/assets/other_images/compass_inner.png", QSize(compassSize / 1.75, compassSize / 1.75));

  staticElements = QPixmap(size());
  staticElements.fill(Qt::transparent);
  QPainter p(&staticElements);

  p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  // Configure the circles
  QPen whitePen(Qt::white, 2);
  p.setPen(whitePen);

  // Draw the circle background and white inner circle
  p.setOpacity(1.0);
  p.setBrush(QColor(0, 0, 0, 100));
  p.drawEllipse(x - circleOffset, y - circleOffset, circleOffset * 2, circleOffset * 2);

  // Draw the white circles
  p.setBrush(Qt::NoBrush);
  p.drawEllipse(x - (innerCompass + 5), y - (innerCompass + 5), (innerCompass + 5) * 2, (innerCompass + 5) * 2);
  p.drawEllipse(x - degreeLabelOffset, y - degreeLabelOffset, degreeLabelOffset * 2, degreeLabelOffset * 2);

  // Draw the black background for the bearing degrees
  QPainterPath outerCircle, innerCircle;
  outerCircle.addEllipse(x - degreeLabelOffset, y - degreeLabelOffset, degreeLabelOffset * 2, degreeLabelOffset * 2);
  innerCircle.addEllipse(x - circleOffset, y - circleOffset, compassSize, compassSize);
  p.fillPath(outerCircle.subtracted(innerCircle), Qt::black);

  // Draw the static degree lines
  for (int i = 0; i < 360; i += 15) {
    bool isCardinalDirection = i % 90 == 0;
    int lineLength = isCardinalDirection ? 15 : 10;
    p.setPen(QPen(Qt::white, isCardinalDirection ? 3 : 1));
    p.save();
    p.translate(x, y);
    p.rotate(i);
    p.drawLine(0, -(compassSize / 2 - lineLength), 0, -(compassSize / 2));
    p.restore();
  }
}

void Compass::updateState(int bearing_deg) {
  if (bearingDeg != bearing_deg) {
    update();
    bearingDeg = bearing_deg;
  }
}

void Compass::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  // Draw static elements
  p.drawPixmap(0, 0, staticElements);

  // Rotate and draw the compassInnerImg image
  p.translate(x, y);
  p.rotate(bearingDeg);
  p.drawPixmap(-compassInnerImg.width() / 2, -compassInnerImg.height() / 2, compassInnerImg);
  p.rotate(-bearingDeg);
  p.translate(-x, -y);

  // Draw the bearing degree numbers
  QFont font = InterFont(10, QFont::Normal);
  for (int i = 0; i < 360; i += 15) {
    bool isBold = abs(i - bearingDeg) <= 7;
    font.setWeight(isBold ? QFont::Bold : QFont::Normal);
    p.setFont(font);
    p.setPen(QPen(Qt::white, i % 90 == 0 ? 2 : 1));

    p.save();
    p.translate(x, y);
    p.rotate(i);
    p.drawLine(0, -(compassSize / 2 - (i % 90 == 0 ? 12 : 8)), 0, -(compassSize / 2));
    p.translate(0, -(compassSize / 2 + 12));
    p.rotate(-i);
    p.drawText(QRect(-20, -10, 40, 20), Qt::AlignCenter, QString::number(i));
    p.restore();
  }

  // Draw cardinal directions
  p.setFont(InterFont(20, QFont::Bold));
  std::map<QString, std::tuple<QPair<float, float>, int, QColor>> directionInfo = {
    {"N", {{292.5, 67.5}, Qt::AlignTop | Qt::AlignHCenter, Qt::white}},
    {"E", {{22.5, 157.5}, Qt::AlignRight | Qt::AlignVCenter, Qt::white}},
    {"S", {{112.5, 247.5}, Qt::AlignBottom | Qt::AlignHCenter, Qt::white}},
    {"W", {{202.5, 337.5}, Qt::AlignLeft | Qt::AlignVCenter, Qt::white}}
  };
  int directionOffset = 20;

  for (auto &item : directionInfo) {
    QString direction = item.first;
    auto &[range, alignmentFlag, color] = item.second;
    auto &[minRange, maxRange] = range;

    QRect textRect(x - innerCompass + directionOffset, y - innerCompass + directionOffset, innerCompass * 2 - 2 * directionOffset, innerCompass * 2 - 2 * directionOffset);

    bool isInRange = false;
    if (minRange > maxRange) {
      isInRange = bearingDeg >= minRange || bearingDeg <= maxRange;
    } else {
      isInRange = bearingDeg >= minRange && bearingDeg <= maxRange;
    }

    p.setOpacity(isInRange ? 1.0 : 0.2);
    p.setPen(QPen(color));
    p.drawText(textRect, alignmentFlag, direction);
  }
}

void AnnotatedCameraWidget::drawLeadInfo(QPainter &p) {
  // Declare the variables
  static QElapsedTimer timer;
  static bool isFiveSecondsPassed = false;
  constexpr int maxAccelDuration = 5000;

  // Constants for units and conversions
  QString accelerationUnit = " m/s²";
  leadDistanceUnit = mapOpen ? "m" : "meters";
  leadSpeedUnit = "m/2";

  float accelerationConversion = 1.0f;
  distanceConversion = 1.0f;
  speedConversion = 1.0f;

  if (!scene.use_si) {
    if (is_metric) {
      // Metric conversion
      leadSpeedUnit = "kph";
      speedConversion = MS_TO_KPH;
    } else {
      // US imperial conversion
      accelerationUnit = " ft/s²";
      leadDistanceUnit = mapOpen ? "ft" : "feet";
      leadSpeedUnit = "mph";

      accelerationConversion = METER_TO_FOOT;
      distanceConversion = METER_TO_FOOT;
      speedConversion = MS_TO_MPH;
    }
  }

  // Update acceleration
  double currentAcceleration = std::round(scene.acceleration * 100) / 100;
  static double maxAcceleration = 0.0;

  if (currentAcceleration > maxAcceleration && status == STATUS_ENGAGED) {
    maxAcceleration = currentAcceleration;
    isFiveSecondsPassed = false;
    timer.start();
  } else {
    isFiveSecondsPassed = timer.hasExpired(maxAccelDuration);
  }

  // Construct text segments
  auto createText = [&](const QString &title, const double data) {
    return title + QString::number(std::round(data * distanceConversion)) + " " + leadDistanceUnit;
  };

  // Create segments for insights
  QString accelText = QString("Accel: %1%2")
    .arg(currentAcceleration * accelerationConversion, 0, 'f', 2)
    .arg(accelerationUnit);

  QString maxAccSuffix = QString(mapOpen ? "" : " - Max: %1%2")
    .arg(maxAcceleration * accelerationConversion, 0, 'f', 2)
    .arg(accelerationUnit);

  QString obstacleText = createText(mapOpen ? " | Obstacle: " : "  |  Obstacle Factor: ", obstacleDistance);
  QString stopText = createText(mapOpen ? " - Stop: " : "  -  Stop Factor: ", scene.stopped_equivalence);
  QString followText = " = " + createText(mapOpen ? "Follow: " : "Follow Distance: ", scene.desired_follow);

  // Check if the longitudinal toggles have an impact on the driving logics
  auto createDiffText = [&](const double data, const double stockData) {
    double difference = std::round((data - stockData) * distanceConversion);
    return difference != 0 ? QString(" (%1%2)").arg(difference > 0 ? "+" : "").arg(difference) : QString();
  };

  // Prepare rectangle for insights
  p.save();
  QRect insightsRect(rect().left() - 1, rect().top() - 60, rect().width() + 2, 100);
  p.setBrush(QColor(0, 0, 0, 150));
  p.drawRoundedRect(insightsRect, 30, 30);
  p.setFont(InterFont(30, QFont::DemiBold));
  p.setRenderHint(QPainter::TextAntialiasing);

  // Calculate positioning for text drawing
  QRect adjustedRect = insightsRect.adjusted(0, 27, 0, 27);
  int textBaseLine = adjustedRect.y() + (adjustedRect.height() + p.fontMetrics().height()) / 2 - p.fontMetrics().descent();

  // Calculate the entire text width to ensure perfect centering
  int totalTextWidth = p.fontMetrics().horizontalAdvance(accelText)
                     + p.fontMetrics().horizontalAdvance(maxAccSuffix)
                     + p.fontMetrics().horizontalAdvance(obstacleText)
                     + p.fontMetrics().horizontalAdvance(createDiffText(obstacleDistance, obstacleDistanceStock))
                     + p.fontMetrics().horizontalAdvance(stopText)
                     + p.fontMetrics().horizontalAdvance(followText);

  int textStartPos = adjustedRect.x() + (adjustedRect.width() - totalTextWidth) / 2;

  // Draw the text
  auto drawText = [&](const QString &text, const QColor color) {
    p.setPen(color);
    p.drawText(textStartPos, textBaseLine, text);
    textStartPos += p.fontMetrics().horizontalAdvance(text);
  };

  drawText(accelText, Qt::white);
  drawText(maxAccSuffix, isFiveSecondsPassed ? Qt::white : Qt::red);
  drawText(obstacleText, Qt::white);
  drawText(createDiffText(obstacleDistance, obstacleDistanceStock), (obstacleDistance - obstacleDistanceStock) > 0 ? Qt::green : Qt::red);
  drawText(stopText, Qt::white);
  drawText(followText, Qt::white);

  p.restore();
}

PedalIcons::PedalIcons(QWidget *parent) : QWidget(parent), scene(uiState()->scene) {
  setFixedSize(btn_size, btn_size);

  brake_pedal_img = loadPixmap("../frogpilot/assets/other_images/brake_pedal.png", QSize(img_size, img_size));
  gas_pedal_img = loadPixmap("../frogpilot/assets/other_images/gas_pedal.png", QSize(img_size, img_size));
}

void PedalIcons::updateState() {
  acceleration = scene.acceleration;

  accelerating = acceleration > 0.25;
  decelerating = acceleration < -0.25;

  if (accelerating || decelerating) {
    update();
  }
}

void PedalIcons::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  p.setRenderHint(QPainter::Antialiasing);

  int totalWidth = 2 * img_size;
  int startX = (width() - totalWidth) / 2;

  int brakeX = startX + img_size / 2;
  int gasX = startX + img_size;

  float brakeOpacity = scene.standstill ? 1.0f : decelerating ? std::max(0.25f, std::abs(acceleration)) : 0.25f;
  float gasOpacity = accelerating ? std::max(0.25f, acceleration) : 0.25f;

  p.setOpacity(brakeOpacity);
  p.drawPixmap(brakeX, (height() - img_size) / 2, brake_pedal_img);

  p.setOpacity(gasOpacity);
  p.drawPixmap(gasX, (height() - img_size) / 2, gas_pedal_img);
}

void AnnotatedCameraWidget::drawSLCConfirmation(QPainter &p) {
  p.save();

  // Get screen size
  QSize size = this->size();

  // Configure the screen halves
  QRect leftRect(0, 0, size.width() / 2, size.height());
  QRect rightRect = leftRect.translated(size.width() / 2, 0);

  // Fill in the screen halves
  p.setOpacity(0.5);
  p.fillRect(leftRect, rightHandDrive ? redColor() : greenColor());
  p.fillRect(rightRect, rightHandDrive ? greenColor() : redColor());
  p.setOpacity(1.0);

  // Configure the font
  p.setFont(InterFont(75, QFont::Bold));
  p.setPen(Qt::white);

  // Configure the text
  QString unitText = is_metric ? "kph" : "mph";
  QString speedText = QString::number(std::nearbyint(scene.unconfirmed_speed_limit * (is_metric ? MS_TO_KPH : MS_TO_MPH))) + " " + unitText;
  QString confirmText = "Confirm speed limit\n" + speedText;
  QString ignoreText = "Ignore speed limit\n" + speedText;

  // Draw the text
  p.drawText(leftRect, Qt::AlignCenter | Qt::AlignTop, rightHandDrive ? ignoreText : confirmText);
  p.drawText(rightRect, Qt::AlignCenter | Qt::AlignTop, rightHandDrive ? confirmText : ignoreText);

  p.restore();
}

PersonalityButton::PersonalityButton(QWidget *parent) : QPushButton(parent), scene(uiState()->scene) {
  setFixedSize(btn_size * 1.5, btn_size * 1.5);

  // Configure the profile vector
  profile_data = {
    {QPixmap("../frogpilot/assets/other_images/aggressive.png"), "Aggressive"},
    {QPixmap("../frogpilot/assets/other_images/standard.png"), "Standard"},
    {QPixmap("../frogpilot/assets/other_images/relaxed.png"), "Relaxed"}
  };

  personalityProfile = params.getInt("LongitudinalPersonality");

  transitionTimer.start();

  connect(this, &QPushButton::clicked, this, &PersonalityButton::handleClick);
}

void PersonalityButton::checkUpdate() {
  // Sync with the steering wheel button
  personalityProfile = params.getInt("LongitudinalPersonality");
  updateState();
  paramsMemory.putBool("PersonalityChangedViaWheel", false);
}

void PersonalityButton::handleClick() {
  int mapping[] = {2, 0, 1};
  personalityProfile = mapping[personalityProfile];

  params.putInt("LongitudinalPersonality", personalityProfile);
  paramsMemory.putBool("PersonalityChangedViaUI", true);

  updateState();
}

void PersonalityButton::updateState() {
  // Start the transition
  transitionTimer.restart();
}

void PersonalityButton::paintEvent(QPaintEvent *) {
  // Declare the constants
  constexpr qreal fadeDuration = 1000.0;  // 1 second
  constexpr qreal textDuration = 3000.0;  // 3 seconds

  QPainter p(this);
  int elapsed = transitionTimer.elapsed();
  qreal textOpacity = qBound(0.0, 1.0 - ((elapsed - textDuration) / fadeDuration), 1.0);
  qreal imageOpacity = qBound(0.0, (elapsed - textDuration) / fadeDuration, 1.0);

  // Enable Antialiasing
  p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

  // Configure the button
  auto &[profile_image, profile_text] = profile_data[personalityProfile];
  QRect rect(0, 0, width(), height() + 95);

  // Draw the profile text with the calculated opacity
  if (textOpacity > 0.0) {
    p.setOpacity(textOpacity);
    p.setFont(InterFont(40, QFont::Bold));
    p.setPen(Qt::white);
    p.drawText(rect, Qt::AlignCenter, profile_text);
  }

  // Draw the profile image with the calculated opacity
  if (imageOpacity > 0.0) {
    drawIcon(p, QPoint((btn_size / 2) * 1.25, btn_size / 2 + 95), profile_image, Qt::transparent, imageOpacity);
  }
}

void AnnotatedCameraWidget::drawStatusBar(QPainter &p) {
  p.save();

  // Variable declarations
  static QElapsedTimer timer;
  static QString lastShownStatus;

  QString newStatus;

  static bool displayStatusText = false;

  constexpr qreal fadeDuration = 1500.0;
  constexpr qreal textDuration = 5000.0;

  // Draw status bar
  QRect currentRect = rect();
  QRect statusBarRect(currentRect.left() - 1, currentRect.bottom() - 50, currentRect.width() + 2, 100);
  p.setBrush(QColor(0, 0, 0, 150));
  p.setOpacity(1.0);
  p.drawRoundedRect(statusBarRect, 30, 30);

  std::map<int, QString> conditionalStatusMap = {
    {0, "Conditional Experimental Mode ready"},
    {1, "Conditional Experimental overridden"},
    {2, "Experimental Mode manually activated"},
    {3, "Conditional Experimental overridden"},
    {4, "Experimental Mode manually activated"},
    {5, "Conditional Experimental overridden"},
    {6, "Experimental Mode manually activated"},
    {7, "Experimental Mode activated for" + (mapOpen ? " intersection" : QString(" upcoming intersection"))},
    {8, "Experimental Mode activated for" + (mapOpen ? " turn" : QString(" upcoming turn"))},
    {9, "Experimental Mode activated due to" + (mapOpen ? "SLC" : QString(" no speed limit set"))},
    {10, "Experimental Mode activated due to" + (mapOpen ? " speed" : " speed being less than " + QString::number(conditionalSpeedLead) + (is_metric ? " kph" : " mph"))},
    {11, "Experimental Mode activated due to" + (mapOpen ? " speed" : " speed being less than " + QString::number(conditionalSpeed) + (is_metric ? " kph" : " mph"))},
    {12, "Experimental Mode activated for slower lead"},
    {13, "Experimental Mode activated for turn" + (mapOpen ? "" : QString(" / lane change"))},
    {14, "Experimental Mode activated for curve"},
    {15, "Experimental Mode activated for stop" + (mapOpen ? "" : QString(" sign / stop light"))},
  };

  QString roadName = roadNameUI ? QString::fromStdString(paramsMemory.get("RoadName")) : QString();

  // Update status text
  if (alwaysOnLateralActive) {
    newStatus = QString("Always On Lateral active") + (mapOpen ? "" : ". Press the \"Cruise Control\" button to disable");
  } else if (conditionalExperimental) {
    newStatus = conditionalStatusMap[status != STATUS_DISENGAGED ? conditionalStatus : 0];
  }

  // Append suffix to the status
  QString distanceSuffix = ". Long press the \"distance\" button to revert";
  QString lkasSuffix = ". Double press the \"LKAS\" button to revert";
  QString screenSuffix = ". Double tap the screen to revert";

  if (!alwaysOnLateralActive && !mapOpen && status != STATUS_DISENGAGED && !newStatus.isEmpty()) {
    if (conditionalStatus == 1 || conditionalStatus == 2) {
      newStatus += screenSuffix;
    } else if (conditionalStatus == 3 || conditionalStatus == 4) {
      newStatus += distanceSuffix;
    } else if (conditionalStatus == 5 || conditionalStatus == 6) {
      newStatus += lkasSuffix;
    }
  }

  // Check if status has changed or if the road name is empty
  if (newStatus != lastShownStatus || roadName.isEmpty()) {
    displayStatusText = true;
    lastShownStatus = newStatus;
    timer.restart();
  } else if (displayStatusText && timer.hasExpired(textDuration + fadeDuration)) {
    displayStatusText = false;
  }

  // Configure the text
  p.setFont(InterFont(40, QFont::Bold));
  p.setPen(Qt::white);
  p.setRenderHint(QPainter::TextAntialiasing);

  // Calculate text opacity
  static qreal roadNameOpacity;
  static qreal statusTextOpacity;
  int elapsed = timer.elapsed();
  if (displayStatusText) {
    statusTextOpacity = qBound(0.0, 1.0 - (elapsed - textDuration) / fadeDuration, 1.0);
    roadNameOpacity = 1.0 - statusTextOpacity;
  } else {
    roadNameOpacity = qBound(0.0, elapsed / fadeDuration, 1.0);
    statusTextOpacity = 0.0;
  }

  // Draw the status text
  p.setOpacity(statusTextOpacity);
  QRect textRect = p.fontMetrics().boundingRect(statusBarRect, Qt::AlignCenter | Qt::TextWordWrap, newStatus);
  textRect.moveBottom(statusBarRect.bottom() - 50);
  p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, newStatus);

  // Draw the road name with the calculated opacity
  if (!roadName.isEmpty()) {
    p.setOpacity(roadNameOpacity);
    textRect = p.fontMetrics().boundingRect(statusBarRect, Qt::AlignCenter | Qt::TextWordWrap, roadName);
    textRect.moveBottom(statusBarRect.bottom() - 50);
    p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, roadName);
  }

  p.restore();
}

void AnnotatedCameraWidget::drawTurnSignals(QPainter &p) {
  // Declare the turn signal size
  constexpr int signalHeight = 480;
  constexpr int signalWidth = 360;

  // Enable Antialiasing
  p.setRenderHint(QPainter::Antialiasing);

  // Calculate the vertical position for the turn signals
  int baseYPosition = (height() - signalHeight) / 2 + (alwaysOnLateral || conditionalExperimental || roadNameUI ? 225 : 300);
  // Calculate the x-coordinates for the turn signals
  int leftSignalXPosition = 75 + width() - signalWidth - 300 * (blindSpotLeft ? 0 : animationFrameIndex);
  int rightSignalXPosition = -75 + 300 * (blindSpotRight ? 0 : animationFrameIndex);

  // Draw the turn signals
  if (animationFrameIndex < signalImgVector.size()) {
    auto drawSignal = [&](bool signalActivated, int xPosition, bool flip, bool blindspot) {
      if (signalActivated) {
        // Get the appropriate image from the signalImgVector
        int uniqueImages = signalImgVector.size() / 4;  // Each image has a regular, flipped, and two blindspot versions
        int index = (blindspot ? 2 * uniqueImages : 2 * animationFrameIndex % totalFrames) + (flip ? 1 : 0);
        QPixmap &signal = signalImgVector[index];
        p.drawPixmap(xPosition, baseYPosition, signalWidth, signalHeight, signal);
      }
    };

    // Display the animation based on which signal is activated
    drawSignal(turnSignalLeft, leftSignalXPosition, false, blindSpotLeft);
    drawSignal(turnSignalRight, rightSignalXPosition, true, blindSpotRight);
  }
}
