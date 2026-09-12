
import openpilot.cereal.messaging as messaging
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.sunnypilot.widgets.list_view import toggle_item_sp
from openpilot.selfdrive.ui.sunnypilot.layouts.settings.steering import SteeringLayout
from openpilot.system.ui.cloudypilot.widgets.list_view import LineSeparatorCP
from openpilot.system.ui.cloudypilot.widgets.list_view import option_item_cp



class SteeringLayoutCP(SteeringLayout):
  def __init__(self):
    super().__init__()

  def _initialize_items(self):
    items = super()._initialize_items()

    self._sr_toggle = toggle_item_sp(
      param="UseCustomSR",
      title=lambda: tr("Enable Custom Steer Ratio"),
      description=lambda: tr("Enable this to use a custom fixed steer ratio value instead of the learned value."),
      callback=self._on_sr_toggled,
    )

    self._custom_sr = option_item_cp(
      param="CustomSR",
      title=lambda: tr("Custom Steer Ratio"),
      min_value=1000,
      max_value=2000,
      value_change_step=5,
      use_float_scaling=True,
      description=lambda: tr("Set a custom steer ratio value."),
      label_callback=lambda sr: f'{sr / 100.0:.2f}',
      reset_enabled=True,
    )

    items += [
      self._sr_toggle,
      self._custom_sr,
      LineSeparatorCP(40),
    ]
    return items

  def _learned_steer_ratio(self) -> float | None:
    dat = ui_state.params.get("LiveParametersV2")
    if dat is None:
      return None
    try:
      return messaging.log_from_bytes(dat).vehicleParameters.steerRatio
    except Exception:
      return None

  def _on_sr_toggled(self, state: bool) -> None:
    # Seed the custom value from the learned steer ratio the first time it's toggled on
    # if no custom value has been set previously
    if not state or ui_state.params.get("CustomSR") is not None:
      return
    sr = self._learned_steer_ratio()
    if sr is None:
      return
    self._custom_sr.action_item.set_value(int(round(sr * 100)))

  def _update_state(self):
    super()._update_state()

    self._sr_toggle.action_item.set_enabled(True)
    self._custom_sr.set_visible(self._sr_toggle.action_item.get_state())
