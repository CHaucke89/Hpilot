import json
import os
from openpilot.common.file_helpers import mkdirs_exists_ok
from openpilot.system.hardware import PC


class MissingAuthConfigError(Exception):
  pass


if PC:
  CONFIG_DIR = os.path.expanduser('~/.comma')
else:
  CONFIG_DIR = "/tmp/.comma"

def get_token():
  try:
    with open(os.path.join(CONFIG_DIR, 'auth.json')) as f:
      auth = json.load(f)
      return auth['access_token']
  except Exception:
    return None


def set_token(token):
  mkdirs_exists_ok(CONFIG_DIR)
  with open(os.path.join(CONFIG_DIR, 'auth.json'), 'w') as f:
    json.dump({'access_token': token}, f)


def clear_token():
  try:
    os.unlink(os.path.join(CONFIG_DIR, 'auth.json'))
  except FileNotFoundError:
    pass
