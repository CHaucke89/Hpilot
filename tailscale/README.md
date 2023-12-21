# Tailscale on Openpilot

## Setup
* Enable ssh in openpilot
* ssh into the comma device
* Run `sudo tailscale/setup.sh`
* Run `sudo tailscale/tailscale up`
* Open the login url in your browser

After logging in you should see tici in your devices on the admin dashboard. You
can now connect to your device using the tailscale ip address on any device
connected to your tailscale network.
