#!/bin/bash

mount -o remount,rw /
mv /data/openpilot/tailscale/tailscaled.service /etc/systemd/system/tailscaled.service
systemctl enable --now tailscaled.service
sudo mount -o remount,r /
