#!/bin/sh
#

echo "Removing SVN files and zipping"

rm -rvf `find ./ -name .svn -print`

APP_PACKAGE=mechcamera_demo
DIR_PACKAGE=camera

rm -rvf ${APP_PACKAGE}.tar.gz
tar -cvf ${APP_PACKAGE}.tar --directory=../ ${DIR_PACKAGE}
gzip ${APP_PACKAGE}.tar

# End of Script