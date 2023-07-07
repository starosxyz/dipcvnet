#!/bin/bash
lines=41
more <<"EOF"
platform will install add /opt/staros.xyz/dipcvnet location.
EOF
agreed=
while [ x$agreed = x ]; do
    echo
    echo "Do you agree to install the dipcvnet system? [yes or no] "
    read reply leftover
    case $reply in
    y* | Y*)
        agreed=1;;
    n* | N*)
    echo "If you don't agree you can't install this software";
    exit 1;;
    esac
done
if [ -d "/opt/staros.xyz/dipcvnet" ] ; then
    rm -rf /opt/staros.xyz/dipcvnet/
    echo "build path.."
else
    echo "build path..."
fi
echo "Unpacking..."
tail -n +$lines $0 >/tmp/tmp.tar.gz
tar xzf /tmp/tmp.tar.gz
mkdir -p /opt/staros.xyz/
mkdir -p /opt/staros.xyz/dipcvnet
mkdir -p /opt/staros.xyz/dipcvnet/logs/mp/{s1,s2,s3}
mkdir -p /opt/staros.xyz/conf/conf_star5gcore_package
/bin/cp -rf tmp/* /opt/staros.xyz/dipcvnet/
/bin/cp /opt/staros.xyz/dipcvnet/bin/x86_dipcvnet /usr/local/bin/
chmod a+x /usr/local/bin/x86_dipcvnet

systemctl daemon-reload
rm -rf /tmp/tmp.tar.gz
rm -rf tmp
echo "Successful"
exit 0
