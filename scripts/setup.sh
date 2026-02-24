tee /etc/NetworkManager/conf.d/unmanaged-wlan0.conf > /dev/null << 'EOF'
[keyfile]
unmanaged-devices=interface-name:wlan0
EOF
systemctl restart NetworkManager

tee /usr/local/sbin/makeIBSS.sh > /dev/null << 'EOF'
#!/bin/sh
set -eu

IF=wlan0
SSID="AutoNet"
FREQ=2412
BSSID="02:12:34:56:78:9a"

ip link set "$IF" down || true
iw dev "$IF" set type ibss
ip link set "$IF" up
iw dev "$IF" set power_save off || true
iw dev "$IF" ibss join "$SSID" "$FREQ" fixed-freq "$BSSID"
EOF
chmod +x /usr/local/sbin/makeIBSS.sh

tee /etc/systemd/system/ibss-wlan0.service >/dev/null <<'EOF'
[Unit]
Description=Configure wlan0 in IBSS (ad-hoc)
After=network-pre.target NetworkManager.service
Wants=network-pre.target

[Service]
Type=oneshot
ExecStart=/usr/local/sbin/makeIBSS.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
EOF
systemctl daemon-reload
systemctl enable --now ibss-wlan0.service

