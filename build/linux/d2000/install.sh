#!/bin/bash
lines=43
more <<"EOF"
platform will install add /opt/staros.xyz/star5gcore location.
EOF
agreed=
while [ x$agreed = x ]; do
    echo
    echo "Do you agree to install the star5gcore system? [yes or no] "
    read reply leftover
    case $reply in
    y* | Y*)
        agreed=1;;
    n* | N*)
    echo "If you don't agree you can't install this software";
    exit 1;;
    esac
done
if [ -d "/opt/staros.xyz/star5gcore" ] ; then
    rm -rf /opt/staros.xyz/star5gcore/
    echo "build path.."
else
    echo "build path..."
fi
echo "Unpacking..."
tail -n +$lines $0 >/tmp/tmp.tar.gz
tar xzf /tmp/tmp.tar.gz
mkdir -p /opt/staros.xyz/
mkdir -p /opt/staros.xyz/star5gcore
mkdir -p /opt/staros.xyz/star5gcore/logs/mp/{s1,s2,s3}
/bin/cp -rf tmp/* /opt/staros.xyz/star5gcore/
/bin/cp -rf tmp/conf/banner.txt /opt/staros.xyz/dipc/conf/
/bin/cp /opt/staros.xyz/star5gcore/libs/libs5gmodule.so /opt/staros.xyz/dipc/system/dmodules/
/bin/cp /opt/staros.xyz/star5gcore/bin/star5gcore /usr/bin/
/bin/cp /opt/staros.xyz/star5gcore/bin/star5gcore-manager /etc/init.d/
chmod a+x /usr/bin/star5gcore
chmod a+x /etc/init.d/star5gcore-manager
systemctl daemon-reload
rm -rf /tmp/tmp.tar.gz
rm -rf tmp
echo "Successful"
exit 0
