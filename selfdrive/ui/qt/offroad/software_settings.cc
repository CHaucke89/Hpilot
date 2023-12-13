#include "selfdrive/ui/qt/offroad/settings.h"

#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include <QDebug>
#include <QLabel>
#include <QProcess>

#include "common/params.h"
#include "common/util.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "system/hardware/hw.h"


void SoftwarePanel::checkForUpdates() {
  std::system("pkill -SIGUSR1 -f selfdrive.updated");
}

SoftwarePanel::SoftwarePanel(QWidget* parent) : ListWidget(parent) {
  onroadLbl = new QLabel(tr("Updates are only downloaded while the car is off."));
  onroadLbl->setStyleSheet("font-size: 50px; font-weight: 400; text-align: left; padding-top: 30px; padding-bottom: 30px;");
  addItem(onroadLbl);

  // current version
  versionLbl = new LabelControl(tr("Current Version"), "");
  addItem(versionLbl);

  // update scheduler
  std::vector<QString> scheduleOptions{tr("Manually"), tr("Daily"), tr("Weekly")};
  ButtonParamControl *preferredSchedule = new ButtonParamControl("UpdateSchedule", tr("Update Scheduler"),
                                          tr("Choose the update frequency for FrogPilot's automatic updates.\n\n"
                                          "This feature will handle the download, installation, and device reboot for a seamless 'Set and Forget' experience.\n\n"
                                          "Weekly updates start at midnight every Sunday."),
                                          "",
                                          scheduleOptions);
  schedule = params.getInt("UpdateSchedule");
  addItem(preferredSchedule);

  std::map<int, QString> timeLabels;
  for (int i = 0; i < 24; ++i) {
    QString hour = QString::number(i % 12 == 0 ? 12 : i % 12);
    QString meridiem = i < 12 ? "AM" : "PM";
    timeLabels[i] = QString("%1:00 %2").arg(hour, meridiem);
  }

  updateTime = new ParamValueControl("UpdateTime", tr("Update Time"), tr("Set your desired update time for automatic updates."), "", 0, 23, timeLabels);
  time = params.getInt("UpdateTime");
  deviceShutdown = params.getInt("DeviceShutdown") * 3600;
  addItem(updateTime);

  // download update btn
  downloadBtn = new ButtonControl(tr("Download"), tr("CHECK"));
  connect(downloadBtn, &ButtonControl::clicked, [=]() {
    downloadBtn->setEnabled(false);
    if (downloadBtn->text() == tr("CHECK")) {
      checkForUpdates();
    } else {
      std::system("pkill -SIGHUP -f selfdrive.updated");
    }
  });
  addItem(downloadBtn);

  // install update btn
  installBtn = new ButtonControl(tr("Install Update"), tr("INSTALL"));
  connect(installBtn, &ButtonControl::clicked, [=]() {
    installBtn->setEnabled(false);
    params.putBool("DoReboot", true);
  });
  addItem(installBtn);

  // branch selecting
  targetBranchBtn = new ButtonControl(tr("Target Branch"), tr("SELECT"));
  connect(targetBranchBtn, &ButtonControl::clicked, [=]() {
    auto current = params.get("GitBranch");
    QStringList branches = QString::fromStdString(params.get("UpdaterAvailableBranches")).split(",");
    for (QString b : {current.c_str(), "devel-staging", "devel", "nightly", "master-ci", "master"}) {
      auto i = branches.indexOf(b);
      if (i >= 0) {
        branches.removeAt(i);
        branches.insert(0, b);
      }
    }

    QString cur = QString::fromStdString(params.get("UpdaterTargetBranch"));
    QString selection = MultiOptionDialog::getSelection(tr("Select a branch"), branches, cur, this);
    if (!selection.isEmpty()) {
      params.put("UpdaterTargetBranch", selection.toStdString());
      targetBranchBtn->setValue(QString::fromStdString(params.get("UpdaterTargetBranch")));
      checkForUpdates();
    }
  });
  if (!params.getBool("IsTestedBranch")) {
    addItem(targetBranchBtn);
  }

  // uninstall button
  auto uninstallBtn = new ButtonControl(tr("Uninstall %1").arg(getBrand()), tr("UNINSTALL"));
  connect(uninstallBtn, &ButtonControl::clicked, [&]() {
    if (ConfirmationDialog::confirm(tr("Are you sure you want to uninstall?"), tr("Uninstall"), this)) {
      params.putBool("DoUninstall", true);
    }
  });
  addItem(uninstallBtn);

  // error log button
  errorLogBtn = new ButtonControl(tr("Error Log"), tr("VIEW"), "View the error log for debugging purposes when openpilot crashes.");
  connect(errorLogBtn, &ButtonControl::clicked, [=]() {
    const std::string txt = util::read_file("/data/community/crashes/error.txt");
    ConfirmationDialog::rich(QString::fromStdString(txt), this);
  });
  addItem(errorLogBtn);

  fs_watch = new ParamWatcher(this);
  QObject::connect(fs_watch, &ParamWatcher::paramChanged, [=](const QString &param_name, const QString &param_value) {
    schedule = params.getInt("UpdateSchedule");
    time = params.getInt("UpdateTime");
    updateLabels();
  });

  connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    is_onroad = !offroad;
    updateLabels();
  });

  QObject::connect(uiState(), &UIState::uiUpdate, this, &SoftwarePanel::automaticUpdate);

  updateLabels();
}

void SoftwarePanel::showEvent(QShowEvent *event) {
  // nice for testing on PC
  installBtn->setEnabled(true);

  updateLabels();
}

void SoftwarePanel::updateLabels() {
  // add these back in case the files got removed
  fs_watch->addParam("LastUpdateTime");
  fs_watch->addParam("UpdateFailedCount");
  fs_watch->addParam("UpdaterState");
  fs_watch->addParam("UpdateAvailable");

  fs_watch->addParam("UpdateSchedule");
  fs_watch->addParam("UpdateTime");

  if (!isVisible()) {
    return;
  }

  // updater only runs offroad
  onroadLbl->setVisible(is_onroad);
  downloadBtn->setVisible(!is_onroad);

  // download update
  QString updater_state = QString::fromStdString(params.get("UpdaterState"));
  bool failed = std::atoi(params.get("UpdateFailedCount").c_str()) > 0;
  if (updater_state != "idle") {
    downloadBtn->setEnabled(false);
    downloadBtn->setValue(updater_state);
  } else {
    if (failed) {
      downloadBtn->setText(tr("CHECK"));
      downloadBtn->setValue(tr("failed to check for update"));
    } else if (params.getBool("UpdaterFetchAvailable")) {
      downloadBtn->setText(tr("DOWNLOAD"));
      downloadBtn->setValue(tr("update available"));
    } else {
      QString lastUpdate = tr("never");
      auto tm = params.get("LastUpdateTime");
      if (!tm.empty()) {
        lastUpdate = timeAgo(QDateTime::fromString(QString::fromStdString(tm + "Z"), Qt::ISODate));
      }
      downloadBtn->setText(tr("CHECK"));
      downloadBtn->setValue(tr("up to date, last checked %1").arg(lastUpdate));
    }
    downloadBtn->setEnabled(true);
  }
  targetBranchBtn->setValue(QString::fromStdString(params.get("UpdaterTargetBranch")));

  // current + new versions
  versionLbl->setText(QString::fromStdString(params.get("UpdaterCurrentDescription")));
  versionLbl->setDescription(QString::fromStdString(params.get("UpdaterCurrentReleaseNotes")));

  installBtn->setVisible(!is_onroad && params.getBool("UpdateAvailable"));
  installBtn->setValue(QString::fromStdString(params.get("UpdaterNewDescription")));
  installBtn->setDescription(QString::fromStdString(params.get("UpdaterNewReleaseNotes")));

  updateTime->setVisible(params.getInt("UpdateSchedule"));

  update();
}

void SoftwarePanel::automaticUpdate() {
  static int timer = 0;
  static std::chrono::system_clock::time_point lastOffroadTime;

  if (!is_onroad) {
    if (timer == 0) {
      lastOffroadTime = std::chrono::system_clock::now();
    }
    std::chrono::duration<int> elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - lastOffroadTime);
    timer = elapsedTime.count();
  } else {
    timer = 0;
  }

  const bool isWifiConnected = (*uiState()->sm)["deviceState"].getDeviceState().getNetworkType() == cereal::DeviceState::NetworkType::WIFI;
  if (schedule == 0 || is_onroad || !isWifiConnected) {
    return;
  }

  static bool isDownloadCompleted = false;
  if (isDownloadCompleted && installBtn->isVisible()) {
    if (timer > deviceShutdown) {
      params.putBool("DoShutdown", true);
    } else {
      params.putBool("DoReboot", true);
    }
    return;
  }

  std::chrono::system_clock::time_point currentTime = std::chrono::system_clock::now();
  std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);
  std::tm now = *std::localtime(&currentTimeT);

  if (now.tm_hour < time) {
    return;
  }

  std::string lastUpdateStr = params.get("Updated");
  std::tm lastUpdate = {};
  std::istringstream iss(lastUpdateStr);

  if (iss >> std::get_time(&lastUpdate, "%Y-%m-%d %H:%M:%S")) {
    lastUpdate.tm_year -= 1900;
    lastUpdate.tm_mon -= 1;
  }
  std::time_t lastUpdateTimeT = std::mktime(&lastUpdate);

  if (!isDownloadCompleted) {
    std::chrono::system_clock::time_point lastUpdateDay = std::chrono::system_clock::from_time_t(lastUpdateTimeT);
    std::chrono::hours durationSinceLastUpdate = std::chrono::duration_cast<std::chrono::hours>(currentTime - lastUpdateDay);
    int daysSinceLastUpdate = durationSinceLastUpdate.count() / 24;

    if ((schedule == 1 && daysSinceLastUpdate > 0) || schedule == 2 && (now.tm_yday / 7) != (std::localtime(&lastUpdateTimeT)->tm_yday / 7)) {
      std::system("pkill -SIGHUP -f selfdrive.updated");
      isDownloadCompleted = true;
    }
  }
}
