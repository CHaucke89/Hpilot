#pragma once

#include <set>

#include "selfdrive/frogpilot/ui/frogpilot_ui_functions.h"
#include "selfdrive/ui/qt/offroad/settings.h"
#include "selfdrive/ui/ui.h"

class FrogPilotControlsPanel : public FrogPilotListWidget {
  Q_OBJECT

public:
  explicit FrogPilotControlsPanel(SettingsWindow *parent);

signals:
  void closeParentToggle();
  void closeSubParentToggle();
  void openParentToggle();
  void openSubParentToggle();

private:
  void hideEvent(QHideEvent *event);
  void hideSubSubToggles();
  void hideSubToggles();
  void parentToggleClicked();
  void subParentToggleClicked();
  void updateCarToggles();
  void updateMetric();
  void updateState(const UIState &s);
  void updateToggles();

  FrogPilotDualParamControl *aggressiveProfile;
  FrogPilotDualParamControl *conditionalSpeedsImperial;
  FrogPilotDualParamControl *conditionalSpeedsMetric;
  FrogPilotDualParamControl *standardProfile;
  FrogPilotDualParamControl *relaxedProfile;

  std::set<QString> conditionalExperimentalKeys = {"CECurves", "CECurvesLead", "CESlowerLead", "CENavigation", "CEStopLights", "CESignal"};
  std::set<QString> fireTheBabysitterKeys = {};
  std::set<QString> laneChangeKeys = {};
  std::set<QString> lateralTuneKeys = {};
  std::set<QString> longitudinalTuneKeys = {"AccelerationProfile", "DecelerationProfile", "AggressiveAcceleration"};
  std::set<QString> mtscKeys = {};
  std::set<QString> qolKeys = {"DisableOnroadUploads"};
  std::set<QString> speedLimitControllerKeys = {};
  std::set<QString> speedLimitControllerControlsKeys = {};
  std::set<QString> speedLimitControllerQOLKeys = {};
  std::set<QString> speedLimitControllerVisualsKeys = {};
  std::set<QString> visionTurnControlKeys = {};

  std::map<std::string, ParamControl*> toggles;

  Params params;
  Params paramsMemory{"/dev/shm/params"};

  bool isMetric = params.getBool("IsMetric");
  bool started = false;
};
