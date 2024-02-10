#include "selfdrive/frogpilot/ui/visual_settings.h"
#include "selfdrive/ui/ui.h"

FrogPilotVisualsPanel::FrogPilotVisualsPanel(SettingsWindow *parent) : FrogPilotListWidget(parent) {
  const std::vector<std::tuple<QString, QString, QString, QString>> visualToggles {
    {"CustomTheme", "Custom Themes", "Enable the ability to use custom themes.", "../frogpilot/assets/wheel_images/frog.png"},
    {"CustomColors", "Color Theme", "Switch out the standard openpilot color scheme with a custom color scheme.\n\nWant to submit your own color scheme? Post it in the 'feature-request' channel in the FrogPilot Discord!", ""},
    {"CustomIcons", "Icon Pack", "Switch out the standard openpilot icons with a set of custom icons.\n\nWant to submit your own icon pack? Post it in the 'feature-request' channel in the FrogPilot Discord!", ""},
    {"CustomSignals", "Turn Signals", "Add custom animations for your turn signals for a personal touch!\n\nWant to submit your own turn signal animation? Post it in the 'feature-request' channel in the FrogPilot Discord!", ""},
    {"CustomSounds", "Sound Pack", "Switch out the standard openpilot sounds with a set of custom sounds.\n\nWant to submit your own sound pack? Post it in the 'feature-request' channel in the FrogPilot Discord!", ""},

    {"AlertVolumeControl", "Alert Volume Control", "Control the volume level for each individual sound in openpilot.", "../frogpilot/assets/toggle_icons/icon_mute.png"},
    {"DisengageVolume", "Disengage Volume", "Related alerts:\n\nAdaptive Cruise Disabled\nParking Brake Engaged\nPedal Pressed\nSpeed too Low", ""},
    {"EngageVolume", "Engage Volume", "Related alerts:\n\nNNFF Torque Controller loaded", ""},
    {"PromptVolume", "Prompt Volume", "Related alerts:\n\nCar Detected in Blindspot\nLight turned green\nSpeed too Low\nSteer Unavailable Below 'X'\nTake Control, Turn Exceeds Steering Limit", ""},
    {"PromptDistractedVolume", "Prompt Distracted Volume", "Related alerts:\n\nPay Attention, Driver Distracted\nTouch Steering Wheel, Driver Unresponsive", ""},
    {"RefuseVolume", "Refuse Volume", "Related alerts:\n\nopenpilot Unavailable", ""},
    {"WarningSoftVolume", "Warning Soft Volume", "Related alerts:\n\nBRAKE!, Risk of Collision\nTAKE CONTROL IMMEDIATELY", ""},
    {"WarningImmediateVolume", "Warning Immediate Volume", "Related alerts:\n\nDISENGAGE IMMEDIATELY, Driver Distracted\nDISENGAGE IMMEDIATELY, Driver Unresponsive", ""},

    {"CameraView", "Camera View", "Choose your preferred camera view for the onroad UI. This is a visual change only and doesn't impact openpilot.", "../frogpilot/assets/toggle_icons/icon_camera.png"},

    {"CustomAlerts", "Custom Alerts", "Enable custom alerts for various logic or situational changes.", "../frogpilot/assets/toggle_icons/icon_green_light.png"},

    {"CustomUI", "Custom Onroad UI", "Customize the Onroad UI with some additional visual functions.", "../assets/offroad/icon_road.png"},
    {"AccelerationPath", "Acceleration Path", "Visualize the car's intended acceleration or deceleration with a color-coded path.", ""},
    {"BlindSpotPath", "Blind Spot Path", "Visualize your blind spots with a red path when another vehicle is detected nearby.", ""},

    {"QOLVisuals", "Quality of Life", "Miscellaneous quality of life changes to improve your overall openpilot experience.", "../frogpilot/assets/toggle_icons/quality_of_life.png"},
    {"DriveStats", "Drive Stats In Home Screen", "Display your device's drive stats in the home screen.", ""},
  };

  for (const auto &[param, title, desc, icon] : visualToggles) {
    ParamControl *toggle;

    if (param == "AlertVolumeControl") {
      FrogPilotParamManageControl *alertVolumeControlToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(alertVolumeControlToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(alertVolumeControlKeys.find(key.c_str()) != alertVolumeControlKeys.end());
        }
      });
      toggle = alertVolumeControlToggle;
    } else if (alertVolumeControlKeys.find(param) != alertVolumeControlKeys.end() && param != "WarningImmediateVolume") {
      toggle = new FrogPilotParamValueControl(param, title, desc, icon, 0, 100, std::map<int, QString>(), this, false, "%");
    } else if (param == "WarningImmediateVolume") {
      toggle = new FrogPilotParamValueControl(param, title, desc, icon, 25, 100, std::map<int, QString>(), this, false, "%");

    } else if (param == "CameraView") {
      std::vector<QString> cameraOptions{tr("Auto"), tr("Standard"), tr("Wide"), tr("Driver")};
      FrogPilotButtonParamControl *preferredCamera = new FrogPilotButtonParamControl(param, title, desc, icon, cameraOptions);
      toggle = preferredCamera;

    } else if (param == "CustomAlerts") {
      FrogPilotParamManageControl *customAlertsToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(customAlertsToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customAlertsKeys .find(key.c_str()) != customAlertsKeys .end());
        }
      });
      toggle = customAlertsToggle;

    } else if (param == "CustomTheme") {
      FrogPilotParamManageControl *customThemeToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(customThemeToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customThemeKeys.find(key.c_str()) != customThemeKeys.end());
        }
      });
      toggle = customThemeToggle;
    } else if (customThemeKeys.find(param) != customThemeKeys.end()) {
      std::vector<QString> themeOptions{tr("Stock"), tr("Frog"), tr("Tesla"), tr("Stalin")};
      FrogPilotButtonParamControl *themeSelection = new FrogPilotButtonParamControl(param, title, desc, icon, themeOptions);
      toggle = themeSelection;

      if (param == "CustomSounds") {
        QObject::connect(static_cast<FrogPilotButtonParamControl*>(toggle), &FrogPilotButtonParamControl::buttonClicked, [this](int id) {
          if (id == 1) {
            if (FrogPilotConfirmationDialog::yesorno("Do you want to enable the bonus 'Goat' sound effect?", this)) {
              params.putBoolNonBlocking("GoatScream", true);
            } else {
              params.putBoolNonBlocking("GoatScream", false);
            }
          }
        });
      }

    } else if (param == "CustomUI") {
      FrogPilotParamManageControl *customUIToggle = new FrogPilotParamManageControl(param, title, desc, icon, this);
      QObject::connect(customUIToggle, &FrogPilotParamManageControl::manageButtonClicked, this, [this]() {
        parentToggleClicked();
        for (auto &[key, toggle] : toggles) {
          toggle->setVisible(customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end());
        }
      });
      toggle = customUIToggle;

    } else if (param == "QOLVisuals") {
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

    QObject::connect(static_cast<FrogPilotParamValueControl*>(toggle), &FrogPilotParamValueControl::valueChanged, [this]() {
      updateToggles();
    });

    QObject::connect(toggle, &AbstractControl::showDescriptionEvent, [this]() {
      update();
    });

    QObject::connect(static_cast<FrogPilotParamManageControl*>(toggle), &FrogPilotParamManageControl::manageButtonClicked, [this]() {
      update();
    });
  }

  std::set<std::string> rebootKeys = {"DriveStats"};
  for (const std::string &key : rebootKeys) {
    QObject::connect(toggles[key], &ToggleControl::toggleFlipped, [this]() {
      if (FrogPilotConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
        Hardware::reboot();
      }
    });
  }

  QObject::connect(device(), &Device::interactiveTimeout, this, &FrogPilotVisualsPanel::hideSubToggles);
  QObject::connect(parent, &SettingsWindow::closeParentToggle, this, &FrogPilotVisualsPanel::hideSubToggles);
  QObject::connect(parent, &SettingsWindow::updateMetric, this, &FrogPilotVisualsPanel::updateMetric);

  hideSubToggles();
  updateMetric();
}

void FrogPilotVisualsPanel::updateToggles() {
  std::thread([this]() {
    paramsMemory.putBool("FrogPilotTogglesUpdated", true);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    paramsMemory.putBool("FrogPilotTogglesUpdated", false);
  }).detach();
}

void FrogPilotVisualsPanel::updateMetric() {
  bool previousIsMetric = isMetric;
  isMetric = params.getBool("IsMetric");

  if (isMetric != previousIsMetric) {
    double distanceConversion = isMetric ? INCH_TO_CM : CM_TO_INCH;
    double speedConversion = isMetric ? FOOT_TO_METER : METER_TO_FOOT;
  }

  if (isMetric) {
  } else {
  }

  previousIsMetric = isMetric;
}

void FrogPilotVisualsPanel::parentToggleClicked() {
  openParentToggle();
}

void FrogPilotVisualsPanel::hideSubToggles() {
  for (auto &[key, toggle] : toggles) {
    bool subToggles = alertVolumeControlKeys.find(key.c_str()) != alertVolumeControlKeys.end() ||
                      customAlertsKeys.find(key.c_str()) != customAlertsKeys.end() ||
                      customOnroadUIKeys.find(key.c_str()) != customOnroadUIKeys.end() ||
                      customThemeKeys.find(key.c_str()) != customThemeKeys.end() ||
                      modelUIKeys.find(key.c_str()) != modelUIKeys.end() ||
                      qolKeys.find(key.c_str()) != qolKeys.end();
    toggle->setVisible(!subToggles);
  }

  closeParentToggle();
}

void FrogPilotVisualsPanel::hideEvent(QHideEvent *event) {
  hideSubToggles();
}
