#pragma once

#include "common/params.h"
#include "selfdrive/ui/qt/network/wifi_manager.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "selfdrive/ui/ui.h"

static const QString buttonStyle = R"(
  QPushButton {
    border-radius: 50px;
    font-size: 40px;
    font-weight: 500;
    height: 100px;
    padding: 0 20 0 20;
    margin: 15px;
    color: #E4E4E4;
    background-color: #393939;
  }
  QPushButton:pressed {
    background-color: #4a4a4a;
  }
  QPushButton:checked:enabled {
    background-color: #33Ab4C;
  }
  QPushButton:disabled {
    color: #33E4E4E4;
  }
)";

class FrogPilotButtonParamControl : public QPushButton {
  Q_OBJECT

public:
  FrogPilotButtonParamControl(const QString &param, const QString &label, const int minimumButtonWidth = 225)
    : QPushButton(), key(param.toStdString()), params(), 
      value(params.getBool(key)) {
    setCheckable(true);
    setChecked(value);
    setStyleSheet(buttonStyle);
    setMinimumWidth(minimumButtonWidth);
    setText(label);

    QObject::connect(this, &QPushButton::toggled, this, [this](bool checked) {
      params.putBoolNonBlocking(key, checked);
      paramsMemory.putBoolNonBlocking("FrogPilotTogglesUpdated", true);
      if (false) {
        if (ConfirmationDialog::toggle("Reboot required to take effect.", "Reboot Now", this)) {
          Hardware::reboot();
        }
      }
    });
  }

private:
  const std::string key;
  Params params;
  Params paramsMemory{"/dev/shm/params"};
  bool value;
};

class ParamValueControl : public AbstractControl {
protected:
  ParamValueControl(const QString &name, const QString &description, const QString &iconPath)
    : AbstractControl(name, description, iconPath) {
    label.setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    label.setStyleSheet("color: #e0e879");
    label.setFixedWidth(170);

    setupButton(btnMinus, "-", -1);
    setupButton(btnPlus, "+", 1);

    hlayout->addWidget(&label);
    hlayout->addWidget(&btnMinus);
    hlayout->addWidget(&btnPlus);
  }

  void setupButton(QPushButton &btn, const QString &text, int delta) {
    btn.setStyleSheet(R"(
      QPushButton {
        background-color: #393939;
        color: #E4E4E4;
        border-radius: 50px;
        font-size: 50px;
        font-weight: 500;
        padding: 0;
      }
      QPushButton:pressed {
        background-color: #4a4a4a;
        color: #E4E4E4;
      }
    )");
    btn.setText(text);
    btn.setFixedSize(110, 100);
    btn.setAutoRepeat(true);
    btn.setAutoRepeatInterval(150);
    connect(&btn, &QPushButton::clicked, [this, delta]() { updateValue(delta); });
  }

  QPushButton btnMinus, btnPlus;
  QLabel label;
  Params params;
  Params paramsMemory{"/dev/shm/params"};
  bool isMetric = params.getBool("IsMetric");

  virtual void updateValue(int delta) = 0;
  virtual void refresh() = 0;
};

class FrogPilotPanel : public QWidget {
  Q_OBJECT

public:
  explicit FrogPilotPanel(QWidget *parent = nullptr) : QWidget(parent) {}
  QFrame *horizontalLine(QWidget *parent = nullptr) const;
  QFrame *whiteHorizontalLine(QWidget *parent = nullptr) const;

  Params params;
  Params paramsMemory{"/dev/shm/params"};
  bool isMetric = params.getBool("IsMetric");

protected:
  QVBoxLayout *mainLayout;
  std::map<std::string, std::vector<QWidget*>> childControls;

  ParamControl *createParamControl(const QString &key, const QString &label, const QString &desc, const QString &icon, QWidget *parent);
  QWidget *addSubControls(const QString &parentKey, QVBoxLayout *layout, const std::vector<std::tuple<QString, QString, QString>> &controls);
  QWidget *createDualParamControl(ParamValueControl *control1, ParamValueControl *control2);
  void addControl(const QString &key, const QString &label, const QString &desc, QVBoxLayout *layout, const QString &icon = "../assets/offroad/icon_blank.png");
  void createSubControl(const QString &key, const QString &label, const QString &desc, const QString &icon, const std::vector<QWidget*> &subControls, const std::vector<std::tuple<QString, QString, QString>> &additionalControls = {});
  void createSubButtonControl(const QString &parentKey, const std::vector<QPair<QString, QString>> &buttonKeys, QVBoxLayout *subControlLayout);
  void setInitialToggleStates();
  void setParams();
};

class FrogPilotControlsPanel : public FrogPilotPanel {
  Q_OBJECT

public:
  explicit FrogPilotControlsPanel(QWidget *parent = nullptr);
};

class FrogPilotVehiclesPanel : public FrogPilotPanel {
  Q_OBJECT

public:
  explicit FrogPilotVehiclesPanel(QWidget *parent = nullptr);
};

class FrogPilotVisualsPanel : public FrogPilotPanel {
  Q_OBJECT

public:
  explicit FrogPilotVisualsPanel(QWidget *parent = nullptr);
};

#define ParamController(className, paramName, labelText, descText, iconPath, getValueStrFunc, newValueFunc) \
class className : public ParamValueControl { \
  Q_OBJECT \
public: \
  className() : ParamValueControl(labelText, descText, iconPath) { \
    if (std::string(#className) == "DeviceShutdown" || std::string(#className) == "StoppingDistance" || std::string(#className) == "WheelIcon") { \
      label.setFixedWidth(225); \
    } \
    if (std::string(#className) == "ConditionalSpeed" || std::string(#className) == "ConditionalSpeedLead") { \
      label.setFixedWidth(180); \
    } \
    refresh(); \
  } \
private: \
  void refresh() override { \
    label.setText(getValueStr()); \
  } \
  void updateValue(int delta) override { \
    int value = params.getInt(paramName); \
    value = newValue(value + delta); \
    params.putIntNonBlocking(paramName, value); \
    paramsMemory.putBoolNonBlocking("FrogPilotTogglesUpdated", true); \
    refresh(); \
  } \
  QString getValueStr() { getValueStrFunc; } \
  int newValue(int v) { newValueFunc; } \
};

ParamController(AccelerationProfile, "AccelerationProfile", "   Acceleration Profile", "Change the rate at which openpilot accelerates with either a sporty or more eco friendly profile.", "../assets/offroad/icon_blank.png",
  const int profile = params.getInt("AccelerationProfile");
  return profile == 1 ? "Eco" : profile == 2 ? "Normal" : "Sport";,
  return std::clamp(v, 1, 3);
)

ParamController(AggressiveJerk, "AggressiveJerk", "Jerk Value", "Set the jerk value for the 'Aggressive Personality'.\n\nValue represents the responsiveness of the brake/gas pedals.\n\nHigher value = Less responsive/more 'relaxed'\n\nStock has a value of 0.5.", "../assets/offroad/icon_blank.png",
  return QString::number(params.getInt("AggressiveJerk") / 10.0);,
  return std::clamp(v, 1, 50);
)

ParamController(AggressiveFollow, "AggressiveFollow", "Time", "Set the following distance for the 'Aggressive Personality'.\n\nValue represents the time (in seconds) to follow the lead vehicle.\n\nStock has a value of 1.25.", "../assets/aggressive.png",
  return QString::number(params.getInt("AggressiveFollow") / 10.0) + " sec";,
  return std::clamp(v, 10, 50);
)

ParamController(ConditionalSpeed, "ConditionalSpeed", "Below", "Switch to 'Experimental Mode' below this speed when there is no lead vehicle.", "../assets/offroad/icon_blank.png",
  const int speed = params.getInt("ConditionalSpeed");
  return speed == 0 ? "Off" : QString::number(speed) + (isMetric ? " kph" : " mph");,
  return std::clamp(v, 0, isMetric ? 150 : 99);
)

ParamController(ConditionalSpeedLead, "ConditionalSpeedLead", "With Lead", "Switch to 'Experimental Mode' below this speed when there is a lead vehicle.", "../assets/offroad/icon_blank.png",
  const int speedLead = params.getInt("ConditionalSpeedLead");
  return speedLead == 0 ? "Off" : QString::number(speedLead) + (isMetric ? " kph" : " mph");,
  return std::clamp(v, 0, isMetric ? 150 : 99);
)

ParamController(RelaxedJerk, "RelaxedJerk", "Jerk Value", "Set the jerk value for the 'Relaxed Personality'.\n\nValue represents the responsiveness of the brake/gas pedals.\n\nHigher value = Less responsive/more 'relaxed'\n\nStock has a value of 1.0.", "../assets/offroad/icon_blank.png",
  return QString::number(params.getInt("RelaxedJerk") / 10.0);,
  return std::clamp(v, 1, 50);
)

ParamController(RelaxedFollow, "RelaxedFollow", "Time", "Set the following distance for the 'Relaxed Personality'.\n\nValue represents the time (in seconds) to follow the lead vehicle.\n\nStock has a value of 1.75.", "../assets/relaxed.png",
  return QString::number(params.getInt("RelaxedFollow") / 10.0) + " sec";,
  return std::clamp(v, 10, 50);
)

ParamController(StandardJerk, "StandardJerk", "Jerk Value", "Set the jerk value for the 'Standard Personality'.\n\nValue represents the responsiveness of the brake/gas pedals.\n\nHigher value = Less responsive/more 'relaxed'\n\nStock has a value of 1.0.", "../assets/offroad/icon_blank.png",
  return QString::number(params.getInt("StandardJerk") / 10.0);,
  return std::clamp(v, 1, 50);
)

ParamController(StandardFollow, "StandardFollow", "Time", "Set the following distance for the 'Standard Personality'.\n\nValue represents the time (in seconds) to follow the lead vehicle.\n\nStock has a value of 1.45.", "../assets/standard.png",
  return QString::number(params.getInt("StandardFollow") / 10.0) + " sec";,
  return std::clamp(v, 10, 50);
)
