#pragma once

#include <cmath>

#include <QTimer>

#include "selfdrive/ui/qt/widgets/controls.h"

class FrogPilotConfirmationDialog : public ConfirmationDialog {
  Q_OBJECT

public:
  explicit FrogPilotConfirmationDialog(const QString &prompt_text, const QString &confirm_text,
                              const QString &cancel_text, const bool rich, QWidget* parent);
  static bool toggle(const QString &prompt_text, const QString &confirm_text, QWidget *parent);
  static bool toggleAlert(const QString &prompt_text, const QString &button_text, QWidget *parent);
  static bool yesorno(const QString &prompt_text, QWidget *parent);
};

class FrogPilotListWidget : public QWidget {
  Q_OBJECT
public:
  explicit FrogPilotListWidget(QWidget *parent = 0) : QWidget(parent), outer_layout(this) {
    outer_layout.setMargin(0);
    outer_layout.setSpacing(0);
    outer_layout.addLayout(&inner_layout);
    inner_layout.setMargin(0);
    inner_layout.setSpacing(25); // default spacing is 25
    outer_layout.addStretch();
  }
  inline void addItem(QWidget *w) { inner_layout.addWidget(w); }
  inline void addItem(QLayout *layout) { inner_layout.addLayout(layout); }
  inline void setSpacing(int spacing) { inner_layout.setSpacing(spacing); }

private:
  void paintEvent(QPaintEvent *) override {
    QPainter p(this);
    p.setPen(Qt::gray);

    int visibleWidgetCount = 0;
    std::vector<QRect> visibleRects;

    for (int i = 0; i < inner_layout.count(); ++i) {
      QWidget *widget = inner_layout.itemAt(i)->widget();
      if (widget && widget->isVisible()) {
        visibleWidgetCount++;
        visibleRects.push_back(inner_layout.itemAt(i)->geometry());
      }
    }

    for (int i = 0; i < visibleWidgetCount - 1; ++i) {
      int bottom = visibleRects[i].bottom() + inner_layout.spacing() / 2;
      p.drawLine(visibleRects[i].left() + 40, bottom, visibleRects[i].right() - 40, bottom);
    }
  }
  QVBoxLayout outer_layout;
  QVBoxLayout inner_layout;
};

class FrogPilotDualParamControl : public QFrame {
  Q_OBJECT

public:
  FrogPilotDualParamControl(ParamControl *control1, ParamControl *control2, QWidget *parent = nullptr, bool split=false)
    : QFrame(parent) {
    QHBoxLayout *hlayout = new QHBoxLayout(this);

    control1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    control1->setMaximumWidth(split ? 850 : 700);

    control2->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    control2->setMaximumWidth(split ? 700 : 850);

    hlayout->addWidget(control1);
    hlayout->addWidget(control2);
  }
};

class FrogPilotButtonControl : public AbstractControl {
  Q_OBJECT

public:
  FrogPilotButtonControl(const QString &title, const QString &text, const QString &desc = "", QWidget *parent = nullptr);
  inline void setText(const QString &text) { btn.setText(text); }
  inline QString text() const { return btn.text(); }

signals:
  void clicked();

public slots:
  void setEnabled(bool enabled) { btn.setEnabled(enabled); }

private:
  QPushButton btn;
};

class FrogPilotButtonIconControl : public AbstractControl {
  Q_OBJECT

public:
  FrogPilotButtonIconControl(const QString &title, const QString &text, const QString &desc = "", const QString &icon = "", QWidget *parent = nullptr);
  inline void setText(const QString &text) { btn.setText(text); }
  inline QString text() const { return btn.text(); }

signals:
  void clicked();

public slots:
  void setEnabled(bool enabled) { btn.setEnabled(enabled); }

private:
  QPushButton btn;
};

class FrogPilotButtonParamControl : public ParamControl {
  Q_OBJECT
public:
  FrogPilotButtonParamControl(const QString &param, const QString &title, const QString &desc, const QString &icon,
                              const std::vector<QString> &button_texts, const int minimum_button_width = 225)
    : ParamControl(param, title, desc, icon) {
    const QString style = R"(
      QPushButton {
        border-radius: 50px;
        font-size: 40px;
        font-weight: 500;
        height:100px;
        padding: 0 25 0 25;
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

    key = param.toStdString();
    int value = atoi(params.get(key).c_str());

    button_group = new QButtonGroup(this);
    button_group->setExclusive(true);
    for (size_t i = 0; i < button_texts.size(); i++) {
      QPushButton *button = new QPushButton(button_texts[i], this);
      button->setCheckable(true);
      button->setChecked(i == value);
      button->setStyleSheet(style);
      button->setMinimumWidth(minimum_button_width);
      hlayout->addWidget(button);
      button_group->addButton(button, i);
    }

    QObject::connect(button_group, QOverload<int, bool>::of(&QButtonGroup::buttonToggled), [=](int id, bool checked) {
      if (checked) {
        params.put(key, std::to_string(id));
        refresh();
        emit buttonClicked(id);
      }
    });

    toggle.hide();
  }

  void setEnabled(bool enable) {
    for (auto btn : button_group->buttons()) {
      btn->setEnabled(enable);
    }
  }

signals:
  void buttonClicked(int id);

private:
  std::string key;
  Params params;
  QButtonGroup *button_group;
};

class FrogPilotButtonsControl : public ParamControl {
  Q_OBJECT
public:
  FrogPilotButtonsControl(const QString &title, const QString &desc, const QString &icon,
                          const std::vector<QString> &button_texts, const int minimum_button_width = 225)
    : ParamControl("", title, desc, icon) {
    const QString style = R"(
      QPushButton {
        border-radius: 50px;
        font-size: 40px;
        font-weight: 500;
        height: 100px;
        padding: 0 25px 0 25px;
        color: #E4E4E4;
        background-color: #393939;
      }
      QPushButton:pressed {
        background-color: #33Ab4C;
      }
      QPushButton:disabled {
        color: #33E4E4E4;
      }
    )";

    button_group = new QButtonGroup(this);

    for (size_t i = 0; i < button_texts.size(); i++) {
      QPushButton *button = new QPushButton(button_texts[i], this);
      button->setStyleSheet(style);
      button->setMinimumWidth(minimum_button_width);
      hlayout->addWidget(button);
      button_group->addButton(button, static_cast<int>(i));

      connect(button, &QPushButton::clicked, this, [this, i]() {
        emit buttonClicked(static_cast<int>(i));
      });
    }

    toggle.hide();
  }

signals:
  void buttonClicked(int id);

private:
  QButtonGroup *button_group;
};

class FrogPilotButtonsParamControl : public ParamControl {
  Q_OBJECT
public:
  FrogPilotButtonsParamControl(const QString &param, const QString &title, const QString &desc, const QString &icon,
                               const std::vector<std::pair<QString, QString>> &button_params)
    : ParamControl(param, title, desc, icon) {
    const QString style = R"(
      QPushButton {
        border-radius: 50px;
        font-size: 40px;
        font-weight: 500;
        height:100px;
        padding: 0 25 0 25;
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

    button_group = new QButtonGroup(this);
    button_group->setExclusive(true);

    for (const auto &param_pair : button_params) {
      const QString &param_toggle = param_pair.first;
      const QString &button_text = param_pair.second;

      QPushButton *button = new QPushButton(button_text, this);
      button->setCheckable(true);

      bool value = params.getBool(param_toggle.toStdString());
      button->setChecked(value);
      button->setStyleSheet(style);
      button->setMinimumWidth(225);
      hlayout->addWidget(button);

      QObject::connect(button, &QPushButton::toggled, this, [=](bool checked) {
        if (checked) {
          for (const auto &inner_param_pair : button_params) {
            const QString &inner_param = inner_param_pair.first;
            params.putBool(inner_param.toStdString(), inner_param == param_toggle);
          }
          refresh();
          emit buttonClicked();
        }
      });

      button_group->addButton(button);
    }

    toggle.hide();
  }

  void setEnabled(bool enable) {
    for (auto btn : button_group->buttons()) {
      btn->setEnabled(enable);
    }
  }

signals:
  void buttonClicked();

private:
  Params params;
  QButtonGroup *button_group;
};

class FrogPilotParamManageControl : public ParamControl {
  Q_OBJECT

public:
  FrogPilotParamManageControl(const QString &param, const QString &title, const QString &desc, const QString &icon, QWidget *parent = nullptr, bool hideToggle = false)
    : ParamControl(param, title, desc, icon, parent),
      hideToggle(hideToggle),
      key(param.toStdString()),
      manageButton(new ButtonControl(tr(""), tr("MANAGE"), tr(""))) {
    hlayout->insertWidget(hlayout->indexOf(&toggle) - 1, manageButton);

    connect(this, &ToggleControl::toggleFlipped, this, [this](bool state) {
      refresh();
    });

    connect(manageButton, &ButtonControl::clicked, this, &FrogPilotParamManageControl::manageButtonClicked);

    if (hideToggle) {
      toggle.hide();
    }
  }

  void refresh() {
    ParamControl::refresh();
    manageButton->setVisible(params.getBool(key) || hideToggle);
  }

  void showEvent(QShowEvent *event) override {
    ParamControl::showEvent(event);
    refresh();
  }

signals:
  void manageButtonClicked();

private:
  bool hideToggle;
  std::string key;
  Params params;
  ButtonControl *manageButton;
};

class FrogPilotParamToggleControl : public ParamControl {
  Q_OBJECT
public:
  FrogPilotParamToggleControl(const QString &param, const QString &title, const QString &desc,
                              const QString &icon, const std::vector<QString> &button_params,
                              const std::vector<QString> &button_texts, QWidget *parent = nullptr,
                              const int minimum_button_width = 225)
    : ParamControl(param, title, desc, icon, parent) {

    key = param.toStdString();

    connect(this, &ToggleControl::toggleFlipped, this, [this](bool state) {
      refreshButtons(state);
    });

    const QString style = R"(
      QPushButton {
        border-radius: 50px;
        font-size: 40px;
        font-weight: 500;
        height:100px;
        padding: 0 25 0 25;
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

    button_group = new QButtonGroup(this);
    button_group->setExclusive(false);
    this->button_params = button_params;

    for (int i = 0; i < button_texts.size(); ++i) {
      QPushButton *button = new QPushButton(button_texts[i], this);
      button->setCheckable(true);
      button->setStyleSheet(style);
      button->setMinimumWidth(minimum_button_width);
      button_group->addButton(button, i);

      connect(button, &QPushButton::clicked, [this, i](bool checked) {
        params.putBool(this->button_params[i].toStdString(), checked);
        button_group->button(i)->setChecked(checked);
        emit buttonClicked(checked);
      });

      hlayout->insertWidget(hlayout->indexOf(&toggle) - 1, button);
    }
  }

  void refresh() {
    bool state = params.getBool(key);
    if (state != toggle.on) {
      toggle.togglePosition();
    }

    refreshButtons(state);
    updateButtonStates();
  }

  void refreshButtons(bool state) {
    for (QAbstractButton *button : button_group->buttons()) {
      button->setVisible(state);
    }
  }

  void updateButtonStates() {
    for (int i = 0; i < button_group->buttons().size(); ++i) {
      bool checked = params.getBool(button_params[i].toStdString());
      QAbstractButton *button = button_group->button(i);
      if (button) {
        button->setChecked(checked);
      }
    }
  }

  void showEvent(QShowEvent *event) override {
    refresh();
    QWidget::showEvent(event);
  }

signals:
  void buttonClicked(const bool checked);

private:
  std::string key;
  Params params;
  QButtonGroup *button_group;
  std::vector<QString> button_params;
};

class FrogPilotParamValueControl : public ParamControl {
  Q_OBJECT

public:
  FrogPilotParamValueControl(const QString &param, const QString &title, const QString &desc, const QString &icon,
                             const float &minValue, const float &maxValue, const std::map<int, QString> &valueLabels,
                             QWidget *parent = nullptr, const bool &loop = true, const QString &label = "",
                             const float &division = 1.0f, const float &intervals = 1.0f)
      : ParamControl(param, title, desc, icon, parent),
        minValue(minValue), maxValue(maxValue), valueLabelMappings(valueLabels), loop(loop), labelText(label),
        division(division), previousValue(0.0f), value(0.0f) {
    key = param.toStdString();

    valueLabel = new QLabel(this);
    hlayout->addWidget(valueLabel);

    QPushButton *decrementButton = createButton("-", this);
    QPushButton *incrementButton = createButton("+", this);

    hlayout->addWidget(decrementButton);
    hlayout->addWidget(incrementButton);

    countdownTimer = new QTimer(this);
    countdownTimer->setInterval(150);
    countdownTimer->setSingleShot(true);

    connect(countdownTimer, &QTimer::timeout, this, &FrogPilotParamValueControl::handleTimeout);

    connect(decrementButton, &QPushButton::pressed, this, [=]() { updateValue(-intervals); });
    connect(incrementButton, &QPushButton::pressed, this, [=]() { updateValue(intervals); });

    connect(decrementButton, &QPushButton::released, this, &FrogPilotParamValueControl::restartTimer);
    connect(incrementButton, &QPushButton::released, this, &FrogPilotParamValueControl::restartTimer);

    toggle.hide();
  }

  void restartTimer() {
    countdownTimer->stop();
    countdownTimer->start();
  }

  void handleTimeout() {
    previousValue = value;
  }

  void updateValue(float increment) {
    if (std::fabs(previousValue - value) > 5.0f && std::fmod(value, 5.0f) == 0.0f) {
      increment *= 5;
    }
    value += increment;

    if (loop) {
      if (value < minValue) value = maxValue;
      else if (value > maxValue) value = minValue;
    } else {
      value = std::max(minValue, std::min(maxValue, value));
    }

    params.putFloat(key, value);
    refresh();
    emit valueChanged(value);
  }

  void refresh() {
    value = params.getFloat(key);

    QString text;
    auto it = valueLabelMappings.find(value);

    if (division > 1.0f) {
      text = QString::number(value / division, 'g', division >= 10.0f ? 4 : 3);
    } else {
      if (it != valueLabelMappings.end()) {
        text = it->second;
      } else {
        text = QString::number(value, 'g', 4);
      }
    }

    if (!labelText.isEmpty()) {
      text += labelText;
    }

    valueLabel->setText(text);
    valueLabel->setStyleSheet("QLabel { color: #E0E879; }");
  }

  void updateControl(float newMinValue, float newMaxValue, const QString &newLabel, float newDivision = 1.0f) {
    minValue = newMinValue;
    maxValue = newMaxValue;
    labelText = newLabel;
    division = newDivision;
  }

  void showEvent(QShowEvent *event) override {
    refresh();
    previousValue = value;
  }

signals:
  void valueChanged(float value);

private:
  bool loop;
  float division;
  float maxValue;
  float minValue;
  float previousValue;
  float value;
  QLabel *valueLabel;
  QString labelText;
  std::map<int, QString> valueLabelMappings;
  std::string key;
  Params params;
  QTimer *countdownTimer;

  QPushButton *createButton(const QString &text, QWidget *parent) {
    QPushButton *button = new QPushButton(text, parent);
    button->setFixedSize(150, 100);
    button->setAutoRepeat(true);
    button->setAutoRepeatInterval(150);
    button->setAutoRepeatDelay(500);
    button->setStyleSheet(R"(
      QPushButton {
        border-radius: 50px;
        font-size: 50px;
        font-weight: 500;
        height: 100px;
        padding: 0 25 0 25;
        color: #E4E4E4;
        background-color: #393939;
      }
      QPushButton:pressed {
        background-color: #4a4a4a;
      }
    )");
    return button;
  }
};

class FrogPilotParamValueToggleControl : public ParamControl {
  Q_OBJECT

public:
  FrogPilotParamValueToggleControl(const QString &param, const QString &title, const QString &desc, const QString &icon,
                          const int &minValue, const int &maxValue, const std::map<int, QString> &valueLabels,
                          QWidget *parent = nullptr, const bool &loop = true, const QString &label = "", const int &division = 1,
                          const std::vector<QString> &button_params = std::vector<QString>(), const std::vector<QString> &button_texts = std::vector<QString>(),
                          const int minimum_button_width = 225)
    : ParamControl(param, title, desc, icon, parent),
      minValue(minValue), maxValue(maxValue), valueLabelMappings(valueLabels), loop(loop), labelText(label), division(division) {
        key = param.toStdString();

        const QString style = R"(
          QPushButton {
            border-radius: 50px;
            font-size: 40px;
            font-weight: 500;
            height:100px;
            padding: 0 25 0 25;
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

        button_group = new QButtonGroup(this);
        button_group->setExclusive(false);

        std::map<QString, bool> paramState;
        for (const QString &button_param : button_params) {
          paramState[button_param] = params.getBool(button_param.toStdString());
        }

        for (int i = 0; i < button_texts.size(); ++i) {
          QPushButton *button = new QPushButton(button_texts[i], this);
          button->setCheckable(true);
          button->setChecked(paramState[button_params[i]]);
          button->setStyleSheet(style);
          button->setMinimumWidth(minimum_button_width);
          button_group->addButton(button, i);

          connect(button, &QPushButton::clicked, [this, button_params, i](bool checked) {
            params.putBool(button_params[i].toStdString(), checked);
            button_group->button(i)->setChecked(checked);
          });

          hlayout->addWidget(button);
        }

        valueLabel = new QLabel(this);
        hlayout->addWidget(valueLabel);

        QPushButton *decrementButton = createButton("-", this);
        QPushButton *incrementButton = createButton("+", this);

        hlayout->addWidget(decrementButton);
        hlayout->addWidget(incrementButton);

        connect(decrementButton, &QPushButton::clicked, this, [=]() {
          updateValue(-1);
        });

        connect(incrementButton, &QPushButton::clicked, this, [=]() {
          updateValue(1);
        });

        toggle.hide();
      }

  void updateValue(int increment) {
    value = value + increment;

    if (loop) {
      if (value < minValue) value = maxValue;
      else if (value > maxValue) value = minValue;
    } else {
      value = std::max(minValue, std::min(maxValue, value));
    }

    params.putInt(key, value);
    refresh();
    emit valueChanged(value);
  }

  void refresh() {
    value = params.getInt(key);

    QString text;
    auto it = valueLabelMappings.find(value);
    if (division > 1) {
      text = QString::number(value / (division * 1.0), 'g');
    } else {
      text = it != valueLabelMappings.end() ? it->second : QString::number(value);
    }
    if (!labelText.isEmpty()) {
      text += labelText;
    }
    valueLabel->setText(text);
    valueLabel->setStyleSheet("QLabel { color: #E0E879; }");
  }

  void updateControl(int newMinValue, int newMaxValue, const QString &newLabel, int newDivision) {
    minValue = newMinValue;
    maxValue = newMaxValue;
    labelText = newLabel;
    division = newDivision;
  }

  void showEvent(QShowEvent *event) override {
    refresh();
  }

signals:
  void valueChanged(int value);

private:
  bool loop;
  int division;
  int maxValue;
  int minValue;
  int value;
  QButtonGroup *button_group;
  QLabel *valueLabel;
  QString labelText;
  std::map<int, QString> valueLabelMappings;
  std::string key;
  Params params;

  QPushButton *createButton(const QString &text, QWidget *parent) {
    QPushButton *button = new QPushButton(text, parent);
    button->setFixedSize(150, 100);
    button->setAutoRepeat(true);
    button->setAutoRepeatInterval(150);
    button->setAutoRepeatDelay(350);
    button->setStyleSheet(R"(
      QPushButton {
        border-radius: 50px;
        font-size: 50px;
        font-weight: 500;
        height: 100px;
        padding: 0 25 0 25;
        color: #E4E4E4;
        background-color: #393939;
      }
      QPushButton:pressed {
        background-color: #4a4a4a;
      }
    )");
    return button;
  }
};
