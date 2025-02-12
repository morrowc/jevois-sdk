#!/bin/sh
# USAGE: jevois-build.sh [destdir]
#
# This script compiles the platform operating system and all its packages, then installs the resulting bootloaders and
# root filesystem to /var/lib/jevois-build (or destdir, if specified). After running jevois-build.sh, you should run
# jevois-flash-card to flash a microSD card.

destdir=$1
if [ "X${destdir}" = "X" ]; then destdir="/var/lib/jevois-build"; fi

./build.sh && ./build.sh pack

if [ $? -ne 0 ]; then exit 1; fi

tmp=`mktemp -dp .`
mkdir ${tmp}/boot

# caution: this is generated by build.sh pack:
cp tools/pack/chips/sun8iw5p1/bin/boot0_sdcard_sun8iw5p1.bin ${tmp}/boot/boot0_sdcard.fex
cp tools/pack/out/sys_config.bin ${tmp}/boot/sys_config.bin
./tools/pack/pctools/linux/mod_update/update_boot0 ${tmp}/boot/boot0_sdcard.fex ${tmp}/boot/sys_config.bin SDMMC_CARD
cp tools/pack/chips/sun8iw5p1/bin/u-boot-sun8iw5p1.bin ${tmp}/boot/u-boot.fex
./tools/pack/pctools/linux/mod_update/update_uboot ${tmp}/boot/u-boot.fex ${tmp}/boot/sys_config.bin 

sudo mkdir -p ${destdir}/boot
sudo cp ${tmp}/boot/u-boot.fex ${tmp}/boot/boot0_sdcard.fex ${destdir}/boot/
sudo cp ${tmp}/boot/sys_config.bin ${destdir}/boot/script.bin
sudo cp linux-3.4/arch/arm/boot/uImage ${destdir}/boot/
sudo cp out/sun8iw5p1/linux/common/rootfs.ext4 ${destdir}/boot/
sudo cp jevois-build/microsd-readme.txt ${destdir}/boot/
sudo cp jevois-build/uEnv.txt ${destdir}/boot/

/bin/rm -rf "${tmp}"
