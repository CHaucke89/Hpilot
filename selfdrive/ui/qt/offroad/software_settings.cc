#include "selfdrive/ui/qt/offroad/settings.h"

#include <cassert>
#include <cmath>
#include <string>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>

#include "common/params.h"
#include "common/util.h"
#include "selfdrive/ui/ui.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/widgets/controls.h"
#include "selfdrive/ui/qt/widgets/input.h"
#include "system/hardware/hw.h"


QMap<QString, QString> statesMap = {
  // Full U.S. state names and their codes
  {"AK", "Alaska"}, {"AL", "Alabama"}, {"AR", "Arkansas"},
  {"AS", "American Samoa"}, {"AZ", "Arizona"}, {"CA", "California"},
  {"CO", "Colorado"}, {"CT", "Connecticut"}, {"DC", "District of Columbia"},
  {"DE", "Delaware"}, {"FL", "Florida"}, {"GA", "Georgia"},
  {"GM", "Guam"}, {"HI", "Hawaii"}, {"IA", "Iowa"},
  {"ID", "Idaho"}, {"IL", "Illinois"}, {"IN", "Indiana"},
  {"KS", "Kansas"}, {"KY", "Kentucky"}, {"LA", "Louisiana"},
  {"MA", "Massachusetts"}, {"MD", "Maryland"}, {"ME", "Maine"},
  {"MI", "Michigan"}, {"MN", "Minnesota"}, {"MO", "Missouri"},
  {"MP", "Northern Mariana Islands"}, {"MS", "Mississippi"}, {"MT", "Montana"},
  {"NC", "North Carolina"}, {"ND", "North Dakota"}, {"NE", "Nebraska"},
  {"NH", "New Hampshire"}, {"NJ", "New Jersey"}, {"NM", "New Mexico"},
  {"NV", "Nevada"}, {"NY", "New York"}, {"OH", "Ohio"},
  {"OK", "Oklahoma"}, {"OR", "Oregon"}, {"PA", "Pennsylvania"},
  {"PR", "Puerto Rico"}, {"RI", "Rhode Island"}, {"SC", "South Carolina"},
  {"SD", "South Dakota"}, {"TN", "Tennessee"}, {"TX", "Texas"},
  {"UT", "Utah"}, {"VA", "Virginia"}, {"VI", "Virgin Islands"},
  {"VT", "Vermont"}, {"WA", "Washington"}, {"WI", "Wisconsin"},
  {"WV", "West Virginia"}, {"WY", "Wyoming"}
};

QMap<QString, QString> countriesMap = {
  // Full nation names and their codes
  {"AE", "United Arab Emirates"}, {"AF", "Afghanistan"}, {"AL", "Albania"},
  {"AM", "Armenia"}, {"AO", "Angola"}, {"AQ", "Antarctica"}, {"AR", "Argentina"},
  {"AT", "Austria"}, {"AU", "Australia"}, {"AZ", "Azerbaijan"}, {"BA", "Bosnia and Herzegovina"},
  {"BD", "Bangladesh"}, {"BE", "Belgium"}, {"BF", "Burkina Faso"}, {"BG", "Bulgaria"},
  {"BI", "Burundi"}, {"BJ", "Benin"}, {"BN", "Brunei"}, {"BO", "Bolivia"},
  {"BR", "Brazil"}, {"BS", "Bahamas"}, {"BT", "Bhutan"}, {"BW", "Botswana"},
  {"BY", "Belarus"}, {"BZ", "Belize"}, {"CA", "Canada"}, {"CD", "Congo (Kinshasa)"},
  {"CF", "Central African Republic"}, {"CG", "Congo (Brazzaville)"}, {"CH", "Switzerland"},
  {"CI", "Ivory Coast"}, {"CL", "Chile"}, {"CM", "Cameroon"}, {"CN", "China"},
  {"CO", "Colombia"}, {"CR", "Costa Rica"}, {"CU", "Cuba"}, {"CY", "Cyprus"},
  {"CZ", "Czech Republic"}, {"DE", "Germany"}, {"DJ", "Djibouti"}, {"DK", "Denmark"},
  {"DO", "Dominican Republic"}, {"DZ", "Algeria"}, {"EC", "Ecuador"}, {"EE", "Estonia"},
  {"EG", "Egypt"}, {"ER", "Eritrea"}, {"ES", "Spain"}, {"ET", "Ethiopia"},
  {"FI", "Finland"}, {"FJ", "Fiji"}, {"FK", "Falkland Islands"}, {"FR", "France"},
  {"GA", "Gabon"}, {"GB", "United Kingdom"}, {"GE", "Georgia"}, {"GH", "Ghana"},
  {"GL", "Greenland"}, {"GM", "Gambia"}, {"GN", "Guinea"}, {"GQ", "Equatorial Guinea"},
  {"GR", "Greece"}, {"GT", "Guatemala"}, {"GW", "Guinea Bissau"}, {"GY", "Guyana"},
  {"HN", "Honduras"}, {"HR", "Croatia"}, {"HT", "Haiti"}, {"HU", "Hungary"},
  {"ID", "Indonesia"}, {"IE", "Ireland"}, {"IL", "Israel"}, {"IN", "India"},
  {"IQ", "Iraq"}, {"IR", "Iran"}, {"IS", "Iceland"}, {"IT", "Italy"},
  {"JM", "Jamaica"}, {"JO", "Jordan"}, {"JP", "Japan"}, {"KE", "Kenya"},
  {"KG", "Kyrgyzstan"}, {"KH", "Cambodia"}, {"KP", "North Korea"}, {"KR", "South Korea"},
  {"KW", "Kuwait"}, {"KZ", "Kazakhstan"}, {"LA", "Laos"}, {"LB", "Lebanon"},
  {"LK", "Sri Lanka"}, {"LR", "Liberia"}, {"LS", "Lesotho"}, {"LT", "Lithuania"},
  {"LU", "Luxembourg"}, {"LV", "Latvia"}, {"LY", "Libya"}, {"MA", "Morocco"},
  {"MD", "Moldova"}, {"ME", "Montenegro"}, {"MG", "Madagascar"}, {"MK", "Macedonia"},
  {"ML", "Mali"}, {"MM", "Myanmar"}, {"MN", "Mongolia"}, {"MR", "Mauritania"},
  {"MW", "Malawi"}, {"MX", "Mexico"}, {"MY", "Malaysia"}, {"MZ", "Mozambique"},
  {"NA", "Namibia"}, {"NC", "New Caledonia"}, {"NE", "Niger"}, {"NG", "Nigeria"},
  {"NI", "Nicaragua"}, {"NL", "Netherlands"}, {"NO", "Norway"}, {"NP", "Nepal"},
  {"NZ", "New Zealand"}, {"OM", "Oman"}, {"PA", "Panama"}, {"PE", "Peru"},
  {"PG", "Papua New Guinea"}, {"PH", "Philippines"}, {"PK", "Pakistan"}, {"PL", "Poland"},
  {"PR", "Puerto Rico"}, {"PS", "West Bank"}, {"PT", "Portugal"}, {"PY", "Paraguay"},
  {"QA", "Qatar"}, {"RO", "Romania"}, {"RS", "Serbia"}, {"RU", "Russia"},
  {"RW", "Rwanda"}, {"SA", "Saudi Arabia"}, {"SB", "Solomon Islands"}, {"SD", "Sudan"},
  {"SE", "Sweden"}, {"SI", "Slovenia"}, {"SK", "Slovakia"}, {"SL", "Sierra Leone"},
  {"SN", "Senegal"}, {"SO", "Somalia"}, {"SR", "Suriname"}, {"SS", "South Sudan"},
  {"SV", "El Salvador"}, {"SY", "Syria"}, {"SZ", "Swaziland"}, {"TD", "Chad"},
  {"TF", "French Southern Territories"}, {"TG", "Togo"}, {"TH", "Thailand"}, {"TJ", "Tajikistan"},
  {"TL", "East Timor"}, {"TM", "Turkmenistan"}, {"TN", "Tunisia"}, {"TR", "Turkey"},
  {"TT", "Trinidad and Tobago"}, {"TW", "Taiwan"}, {"TZ", "Tanzania"}, {"UA", "Ukraine"},
  {"UG", "Uganda"}, {"US", "United States"}, {"UY", "Uruguay"}, {"UZ", "Uzbekistan"},
  {"VE", "Venezuela"}, {"VN", "Vietnam"}, {"VU", "Vanuatu"}, {"YE", "Yemen"},
  {"ZA", "South Africa"}, {"ZM", "Zambia"}, {"ZW", "Zimbabwe"},
};

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

  // offline maps button
  mapsBtn = new ButtonControl(tr("Offline Maps"), tr("SELECT"));
  connect(mapsBtn, &ButtonControl::clicked, [this] {
    QStringList locationNames{"United States"}, stateNames, nationNames;
    QMap<QString, QString> locationMap;

    for (const QString &stateCode : statesMap.keys()) {
      QString stateName = statesMap[stateCode];
      stateNames.append(stateName);
      locationMap[stateName] = stateCode;
    }

    for (const QString &countryCode : countriesMap.keys()) {
      QString countryName = countriesMap[countryCode];
      nationNames.append(countryName);
      locationMap[countryName] = countryCode;
    }

    std::sort(stateNames.begin(), stateNames.end());
    std::sort(nationNames.begin(), nationNames.end());

    locationNames += stateNames + nationNames;

    QString currentSelection = QString::fromStdString(params.get("MapSelected"));
    QString currentLocationName = locationMap.key(currentSelection, QString());

    QString selectedLocationName = MultiOptionDialog::getSelection(tr("Select a location to download"), locationNames, currentLocationName, this);

    if (!selectedLocationName.isEmpty()) {
      QString selectedCode = locationMap[selectedLocationName];
      QJsonObject json;
      json.insert(nationNames.contains(selectedLocationName) ? "nations" : "states", QJsonArray{selectedCode});

      params.put("MapSelected", selectedLocationName.toStdString());
      Params{"/dev/shm/params"}.put("OSMDownloadLocations", QJsonDocument(json).toJson(QJsonDocument::Compact).toStdString());
      mapsBtn->setValue(selectedLocationName);
    }
  });
  addItem(mapsBtn);

  fs_watch = new ParamWatcher(this);
  QObject::connect(fs_watch, &ParamWatcher::paramChanged, [=](const QString &param_name, const QString &param_value) {
    updateLabels();
  });

  connect(uiState(), &UIState::offroadTransition, [=](bool offroad) {
    is_onroad = !offroad;
    updateLabels();
  });

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

  mapsBtn->setValue(QString::fromStdString(params.get("MapSelected")));

  // current + new versions
  versionLbl->setText(QString::fromStdString(params.get("UpdaterCurrentDescription")));
  versionLbl->setDescription(QString::fromStdString(params.get("UpdaterCurrentReleaseNotes")));

  installBtn->setVisible(!is_onroad && params.getBool("UpdateAvailable"));
  installBtn->setValue(QString::fromStdString(params.get("UpdaterNewDescription")));
  installBtn->setDescription(QString::fromStdString(params.get("UpdaterNewReleaseNotes")));

  update();
}
