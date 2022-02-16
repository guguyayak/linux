# nfsd
```
#!/bin/bash

INSTALL_DIR=$(dirname "$0")

if [ ! -d "/usr/lib/systemd/scripts/" ]; then
    mkdir -p /usr/lib/systemd/scripts/
fi
if [ ! -e "/usr/lib/systemd/system/bind_nfsd_to_CPU.service" ]; then
    cp $INSTALL_DIR/bind_nfsd_to_CPU.service $SYSTEM_DIR
fi

grep "Wants=bind_nfsd_to_CPU.service" $SYSTEM_DIR/nfs-server.service > /dev/null
if [ $? -ne 0 ]; then
    sed -i '/\[Service\]/i\Wants=bind_nfsd_to_CPU.service\nBefore=bind_nfsd_to_CPU.service\n' \
        $SYSTEM_DIR/nfs-server.service
fi

systemctl daemon-reload
```
