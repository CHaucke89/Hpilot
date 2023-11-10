"""Install exception handler for process crash."""
import os
import sentry_sdk
import traceback
from datetime import datetime
from enum import Enum
from sentry_sdk.integrations.threading import ThreadingIntegration

from openpilot.common.params import Params
from openpilot.system.hardware import HARDWARE, PC
from openpilot.system.swaglog import cloudlog
from openpilot.system.version import get_branch, get_commit, get_origin, get_version


class SentryProject(Enum):
  # python project
  SELFDRIVE = "https://5ad1714d27324c74a30f9c538bff3b8d@o4505034923769856.ingest.sentry.io/4505034930651136"
  # native project
  SELFDRIVE_NATIVE = "https://5ad1714d27324c74a30f9c538bff3b8d@o4505034923769856.ingest.sentry.io/4505034930651136"


CRASHES_DIR = '/data/community/crashes/'


def report_tombstone(fn: str, message: str, contents: str) -> None:
  cloudlog.error({'tombstone': message})

  with sentry_sdk.configure_scope() as scope:
    scope.set_extra("tombstone_fn", fn)
    scope.set_extra("tombstone", contents)
    sentry_sdk.capture_message(message=message)
    sentry_sdk.flush()


def capture_exception(*args, **kwargs) -> None:
  save_exception(traceback.format_exc())
  cloudlog.error("crash", exc_info=kwargs.get('exc_info', 1))

  try:
    sentry_sdk.capture_exception(*args, **kwargs)
    sentry_sdk.flush()  # https://github.com/getsentry/sentry-python/issues/291
  except Exception:
    cloudlog.exception("sentry exception")


def save_exception(exc_text):
  if not os.path.exists(CRASHES_DIR):
    os.makedirs(CRASHES_DIR)

  files = [
    os.path.join(CRASHES_DIR, datetime.now().strftime('%d-%m-%Y--%H-%M-%S.log')),
    os.path.join(CRASHES_DIR, 'error.txt')
  ]

  for file in files:
    with open(file, 'w') as f:
      f.write(exc_text)


def bind_user(**kwargs) -> None:
  sentry_sdk.set_user(kwargs)
  sentry_sdk.flush()


def capture_warning(warning_string, dongle_id):
  with sentry_sdk.configure_scope() as scope:
    scope.fingerprint = [warning_string, dongle_id]
  bind_user(id=dongle_id)
  sentry_sdk.capture_message(warning_string, level='warning')
  sentry_sdk.flush()


def set_tag(key: str, value: str) -> None:
  sentry_sdk.set_tag(key, value)


def init(project: SentryProject) -> bool:
  # forks like to mess with this, so double check
  frogpilot = "FrogAi" in get_origin(default="")
  if not frogpilot or PC:
    return False

  params = Params()
  dongle_id = params.get("DongleId", encoding='utf-8')
  installed = params.get("InstallDate")
  updated = params.get("Updated")

  integrations = []
  if project == SentryProject.SELFDRIVE:
    integrations.append(ThreadingIntegration(propagate_hub=True))
  else:
    sentry_sdk.utils.MAX_STRING_LENGTH = 8192

  sentry_sdk.init(project.value,
                  default_integrations=False,
                  release=get_version(),
                  integrations=integrations,
                  traces_sample_rate=1.0,
                  send_default_pii=True)

  sentry_sdk.set_user({"id": dongle_id})
  sentry_sdk.set_tag("serial", HARDWARE.get_serial())
  sentry_sdk.set_tag("branch", get_branch())
  sentry_sdk.set_tag("commit", get_commit())
  sentry_sdk.set_tag("updated", updated)
  sentry_sdk.set_tag("installed", installed)

  if project == SentryProject.SELFDRIVE:
    sentry_sdk.Hub.current.start_session()

  return True
