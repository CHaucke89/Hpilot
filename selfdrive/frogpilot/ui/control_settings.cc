#include "selfdrive/frogpilot/ui/control_settings.h"
#include "selfdrive/ui/ui.h"

FrogPilotControlsPanel::FrogPilotControlsPanel(SettingsWindow *parent) : FrogPilotListWidget(parent) {
  const std::vector<std::tuple<QString, QString, QString, QString>> controlToggles {
    {"AlwaysOnLateral", "Always on Lateral", "Maintain openpilot lateral control when the brake or gas pedals are used.\n\nDeactivation occurs only through the 'Cruise Control' button.", "../frogpilot/assets/toggle_icons/icon_always_on_lateral.png"},

    {"ConditionalExperimental", "Conditional Experimental Mode", "Automatically switches to 'Experimental Mode' under predefined conditions.", "../frogpilot/assets/toggle_icons/icon_conditional.png"},
    {"CECurves", "Curve Detected Ahead", "Switch to 'Experimental Mode' when a curve is detected.", ""},
    {"CENavigation", "Navigation Based", "Switch to 'Experimental Mode' based on navigation data. (i.e. Intersections, stop signs, etc.)", ""},
    {"CESlowerLead", "Slower Lead Detected Ahead", "Switch to 'Experimental Mode' when a slower lead vehicle is detected ahead.", ""},
    {"CEStopLights", "Stop Lights and Stop Signs", "Switch to 'Experimental Mode' when a stop light or stop sign is detected.", ""},
    {"CESignal", "Turn Signal When Below Highway Speeds", "Switch to 'Experimental Mode' when using turn signals below highway speeds to help assit with turns.", ""},

    {"CustomPersonalities", "Custom Driving Personalities", "Customize the driving personality profiles to your driving style.", "../frogpilot/assets/toggle_icons/icon_custom.png"},

    {"LateralTune", "Lateral Tuning", "Modify openpilot's steering behavior.", "../frogpilot/assets/toggle_icons/icon_lateral_tune.png"},

    {"LongitudinalTune", "Longitudinal Tuning", "Modify openpilot's acceleration and braking behavior.", "../frogpilot/assets/toggle_icons/icon_longitudinal_tune.png"},
    {"AccelerationProfile", "Acceleration Profile", "Change the acceleration rate to be either sporty or eco-friendly.", ""},
    {"AggressiveAcceleration", "Aggressive Acceleration With Lead", "Increase acceleration aggressiveness when following a lead vehicle from a stop.", ""},

    {"QOLControls", "Quality of Life", "Miscellaneous quality of life changes to improve your overall openpilot experience.", "../frogpilot/assets/toggle_icons/quality_of_life.png"},
  };

  for (const auto &[param, title, desc, icon] : controlToggles) {
    ParamControl *toggle;

    if (param == "AlwaysOnLateral") {
      std::vector<QString> aolToggles{"AlwaysOnLateralMain"};
      std::vector<QString> aolToggleNames{tr("Enable On Cruise Main")};
      toggle = new FrogPilotParamToggleControl(param, title, desc, icon, aolToggles, aolToggleNames);

      QObject::connect(static_cast<FrogPilotParamToggleControl*>(toggle), &FrogPilotParamToggleControl::buttonClicked, [this](const bool checked) {
        if (checked) {
          FrogPilotConfirmationDialog::toggleAlert("WARNING: This is very experimental and isn't guaranteed to work. If you run into any issues, please report it in the FrogPilot Discord!",
          "I understand the risks.", this);
        }
        if (FrogPilotConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
          Hardware::reboot();
        }
      });

    } else if (param == "ConditionalExperimental") {
      FrogPilotParamManageControl *conditionalExperimentalToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(conditionalExperimentalToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        conditionalSpeedsImperial->setVisible(!isMetric);
        conditionalSpeedsMetric->setVisible(isMetric);
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(conditionalExperimentalKeys.find(key.c_str()) != conditionalExperimentalKeys.end());
        }
      });
      toggle = conditionalExperimentalToggle;
    } else if (param == "CECurves") {
      FrogPilotParamValueControl *CESpeedImperial = new FrogPilotParamValueControl("CESpeed", "Below", "Switch to 'Experimental Mode' below this speed in absence of a lead vehicle.", "", 0, 99,
                                                                                   std::map<int, QString>(), this, false, " mph");
      FrogPilotParamValueControl *CESpeedLeadImperial = new FrogPilotParamValueControl("CESpeedLead", "  w/Lead", "Switch to 'Experimental Mode' below this speed when following a lead vehicle.", "", 0, 99,
                                                                                       std::map<int, QString>(), this, false, " mph");
      conditionalSpeedsImperial = new FrogPilotDualParamControl(CESpeedImperial, CESpeedLeadImperial, this);
      addItem(conditionalSpeedsImperial);

      FrogPilotParamValueControl *CESpeedMetric = new FrogPilotParamValueControl("CESpeed", "Below", "Switch to 'Experimental Mode' below this speed in absence of a lead vehicle.", "", 0, 150,
                                                                                 std::map<int, QString>(), this, false, " kph");
      FrogPilotParamValueControl *CESpeedLeadMetric = new FrogPilotParamValueControl("CESpeedLead", "  w/Lead", "Switch to 'Experimental Mode' below this speed when following a lead vehicle.", "", 0, 150,
                                                                                     std::map<int, QString>(), this, false, " kph");
      conditionalSpeedsMetric = new FrogPilotDualParamControl(CESpeedMetric, CESpeedLeadMetric, this);
      addItem(conditionalSpeedsMetric);

      std::vector<QString> curveToggles{"CECurvesLead"};
      std::vector<QString> curveToggleNames{tr("With Lead")};
      toggle = new FrogPilotParamToggleControl(param, title, desc, icon, curveToggles, curveToggleNames);
    } else if (param == "CEStopLights") {
      std::vector<QString> stopLightToggles{"CEStopLightsLead"};
      std::vector<QString> stopLightToggleNames{tr("With Lead")};
      toggle = new FrogPilotParamToggleControl(param, title, desc, icon, stopLightToggles, stopLightToggleNames);

    } else if (param == "CustomPersonalities") {
      FrogPilotParamManageControl *customPersonalitiesToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(customPersonalitiesToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(false);
        }
        aggressiveProfile->setVisible(true);
        standardProfile->setVisible(true);
        relaxedProfile->setVisible(true);
      });
      toggle = customPersonalitiesToggle;

      FrogPilotParamValueControl *aggressiveFollow = new FrogPilotParamValueControl("AggressiveFollow", "Follow",
                                                                                    "Set the 'Aggressive' personality' following distance. "
                                                                                    "Represents seconds to follow behind the lead vehicle.\n\nStock: 1.25 seconds.",
                                                                                    "../frogpilot/assets/other_images/aggressive.png",
                                                                                    10, 50, std::map<int, QString>(), this, false, " sec", 10);
      FrogPilotParamValueControl *aggressiveJerk = new FrogPilotParamValueControl("AggressiveJerk", " Jerk",
                                                                                  "Configure brake/gas pedal responsiveness for the 'Aggressive' personality. "
                                                                                  "Higher values yield a more 'relaxed' response.\n\nStock: 0.5.",
                                                                                  "",
                                                                                  1, 50, std::map<int, QString>(), this, false, "", 10);
      aggressiveProfile = new FrogPilotDualParamControl(aggressiveFollow, aggressiveJerk, this, true);
      addItem(aggressiveProfile);

      FrogPilotParamValueControl *standardFollow = new FrogPilotParamValueControl("StandardFollow", "Follow",
                                                                                  "Set the 'Standard' personality following distance. "
                                                                                  "Represents seconds to follow behind the lead vehicle.\n\nStock: 1.45 seconds.",
                                                                                  "../frogpilot/assets/other_images/standard.png",
                                                                                  10, 50, std::map<int, QString>(), this, false, " sec", 10);
      FrogPilotParamValueControl *standardJerk = new FrogPilotParamValueControl("StandardJerk", " Jerk",
                                                                                "Adjust brake/gas pedal responsiveness for the 'Standard' personality. "
                                                                                "Higher values yield a more 'relaxed' response.\n\nStock: 1.0.",
                                                                                "",
                                                                                1, 50, std::map<int, QString>(), this, false, "", 10);
      standardProfile = new FrogPilotDualParamControl(standardFollow, standardJerk, this, true);
      addItem(standardProfile);

      FrogPilotParamValueControl *relaxedFollow = new FrogPilotParamValueControl("RelaxedFollow", "Follow",
                                                                                 "Set the 'Relaxed' personality following distance. "
                                                                                 "Represents seconds to follow behind the lead vehicle.\n\nStock: 1.75 seconds.",
                                                                                 "../frogpilot/assets/other_images/relaxed.png",
                                                                                 10, 50, std::map<int, QString>(), this, false, " sec", 10);
      FrogPilotParamValueControl *relaxedJerk = new FrogPilotParamValueControl("RelaxedJerk", " Jerk",
                                                                               "Set brake/gas pedal responsiveness for the 'Relaxed' personality. "
                                                                               "Higher values yield a more 'relaxed' response.\n\nStock: 1.0.",
                                                                               "", 1, 50,
                                                                               std::map<int, QString>(), this, false, "", 10);
      relaxedProfile = new FrogPilotDualParamControl(relaxedFollow, relaxedJerk, this, true);
      addItem(relaxedProfile);

    } else if (param == "LateralTune") {
      FrogPilotParamManageControl *lateralTuneToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(lateralTuneToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(lateralTuneKeys.find(key.c_str()) != lateralTuneKeys.end());
        }
      });
      toggle = lateralTuneToggle;

    } else if (param == "LongitudinalTune") {
      FrogPilotParamManageControl *longitudinalTuneToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(longitudinalTuneToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(longitudinalTuneKeys.find(key.c_str()) != longitudinalTuneKeys.end());
        }
      });
      toggle = longitudinalTuneToggle;
    } else if (param == "AccelerationProfile") {
      std::vector<QString> profileOptions{tr("Standard"), tr("Eco"), tr("Sport"), tr("Sport+")};
      FrogPilotButtonParamControl *profileSelection = new FrogPilotButtonParamControl(param, title, desc, icon, profileOptions);
      toggle = profileSelection;

      QObject::connect(static_cast<FrogPilotButtonParamControl*>(toggle), &FrogPilotButtonParamControl::buttonClicked, [this](int id) {
        if (id == 3) {
          FrogPilotConfirmationDialog::toggleAlert("WARNING: This maxes out openpilot's acceleration from 2.0 m/s to 4.0 m/s and may cause oscillations when accelerating!",
          "I understand the risks.", this);
        }
      });

    } else if (param == "QOLControls") {
      FrogPilotParamManageControl *qolToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(qolToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(qolKeys.find(key.c_str()) != qolKeys.end());
        }
      });
      toggle = qolToggle;

    } else {
      toggle = new ParamControl(param, title, desc, icon, this);
    }

    addItem(toggle);
    toggles[param.toStdString()] = toggle;

    QObject::connect(toggle, &ToggleControl::toggleFlipped, [this]() {
      updateToggles();
    });

    QObject::connect(static_cast<FrogPilotParamValueControl*>(toggle), &FrogPilotParamValueControl::buttonPressed, [this]() {
      updateToggles();
    });

    QObject::connect(toggle, &AbstractControl::showDescriptionEvent, [this]() {
      update();
    });

    QObject::connect(static_cast<FrogPilotParamManageControl*>(toggle), &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
      update();
    });
  }

  std::set<std::string> rebootKeys = {"AlwaysOnLateral"};
  for (const std::string &key : rebootKeys) {
    QObject::connect(toggles[key], &ToggleControl::toggleFlipped, [this]() {
      if (FrogPilotConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
        Hardware::reboot();
      }
    });
  }

  conditionalExperimentalKeys = {"CECurves", "CECurvesLead", "CESlowerLead", "CENavigation", "CEStopLights", "CESignal"};
  fireTheBabysitterKeys = {};
  laneChangeKeys = {};
  lateralTuneKeys = {};
  longitudinalTuneKeys = {"AccelerationProfile", "AggressiveAcceleration"};
  mtscKeys = {};
  qolKeys = {};
  speedLimitControllerKeys = {};
  visionTurnControlKeys = {};

  QObject::connect(device(), &Device::interactiveTimeout, this, &FrogPilotControlsPanel::hideSubToggles);
  QObject::connect(parent, &SettingsWindow::closeParentToggle, this, &FrogPilotControlsPanel::hideSubToggles);
  QObject::connect(parent, &SettingsWindow::updateMetric, this, &FrogPilotControlsPanel::updateMetric);
  QObject::connect(uiState(), &UIState::offroadTransition, this, [this](bool offroad) {if (!offroad) updateCarToggles();});

  hideSubToggles();
  updateMetric();
}

void FrogPilotControlsPanel::updateToggles() {
  std::thread([this]() {
    paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    paramsMemory.putBool("FrogPilotTogglesUpdated", false);
  }).detach();
}

void FrogPilotControlsPanel::updateCarToggles() {
}

void FrogPilotControlsPanel::updateMetric() {
  bool previousIsMetric = isMetric;
  isMetric = params.getBool("IsMetric");

  if (isMetric != previousIsMetric) {
    double distanceConversion = isMetric ? FOOT_TO_METER : METER_TO_FOOT;
    double speedConversion = isMetric ? MILE_TO_KM : KM_TO_MILE;
    params.putIntNonBlocking("CESpeed", std::nearbyint(params.getInt("CESpeed") * speedConversion));
    params.putIntNonBlocking("CESpeedLead", std::nearbyint(params.getInt("CESpeedLead") * speedConversion));
  }

  if (isMetric) {
  } else {
  }

  previousIsMetric = isMetric;
}

void FrogPilotControlsPanel::parentToggleClicked() {
  aggressiveProfile->setVisible(false);
  conditionalSpeedsImperial->setVisible(false);
  conditionalSpeedsMetric->setVisible(false);
  standardProfile->setVisible(false);
  relaxedProfile->setVisible(false);

  this->openParentToggle();
}

void FrogPilotControlsPanel::hideSubToggles() {
  aggressiveProfile->setVisible(false);
  conditionalSpeedsImperial->setVisible(false);
  conditionalSpeedsMetric->setVisible(false);
  standardProfile->setVisible(false);
  relaxedProfile->setVisible(false);

  for (auto &[key, toggle] : toggles) {
    bool subToggles = conditionalExperimentalKeys.find(key.c_str()) != conditionalExperimentalKeys.end() ||
                      fireTheBabysitterKeys.find(key.c_str()) != fireTheBabysitterKeys.end() ||
                      laneChangeKeys.find(key.c_str()) != laneChangeKeys.end() ||
                      lateralTuneKeys.find(key.c_str()) != lateralTuneKeys.end() ||
                      longitudinalTuneKeys.find(key.c_str()) != longitudinalTuneKeys.end() ||
                      mtscKeys.find(key.c_str()) != mtscKeys.end() ||
                      qolKeys.find(key.c_str()) != qolKeys.end() ||
                      speedLimitControllerKeys.find(key.c_str()) != speedLimitControllerKeys.end() ||
                      visionTurnControlKeys.find(key.c_str()) != visionTurnControlKeys.end();
    toggle->setVisible(!subToggles);
  }

  this->closeParentToggle();
}

void FrogPilotControlsPanel::hideEvent(QHideEvent *event) {
  hideSubToggles();
}
