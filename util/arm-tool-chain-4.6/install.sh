#/bin/sh
list="libc6-armel-cross_2.15-0ubuntu2~ppa1cross1.79~ppa1_all.deb 
libc6-dev-armel-cross_2.15-0ubuntu2~ppa1cross1.79~ppa1_all.deb 
libgmp3c2_2%3a4.3.2+dfsg-1ubuntu1_amd64.deb 
libmpfr1ldbl_2.4.2-3ubuntu1_amd64.deb 
libmpc2_0.8.1-1_amd64.deb 
linux-libc-dev-armel-cross_3.2.0-17.26~ppa1cross1.79~ppa1_all.deb 
binutils-arm-linux-gnueabi_2.22-5ubuntu1~ppa1cross1.79~ppa1_amd64.deb 
cpp-4.6-arm-linux-gnueabi_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_amd64.deb 
cpp-arm-linux-gnueabi_4%3a4.6.2-7_amd64.deb 
gcc-4.6-arm-linux-gnueabi_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_amd64.deb 
gcc-4.6-arm-linux-gnueabi-base_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_all.deb 
gcc-arm-linux-gnueabi_4%3a4.6.2-7_amd64.deb 
libstdc++6-armel-cross_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_all.deb 
libstdc++6-4.6-dev-armel-cross_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_all.deb 
g++-4.6-arm-linux-gnueabi_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_amd64.deb 
g++-arm-linux-gnueabi_4%3a4.6.2-7_amd64.deb 
gfortran-4.6-arm-linux-gnueabi_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_amd64.deb 
gfortran-arm-linux-gnueabi_4%3a4.6.2-7_amd64.deb 
libgcc1-armel-cross_1%3a4.6.2-14ubuntu2~ppa1cross1.79~ppa1_all.deb 
libgfortran3-armel-cross_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_all.deb 
libgomp1-armel-cross_4.6.2-14ubuntu2~ppa1cross1.60~ppa1_all.deb"
sudo dpkg -i $list
if [ $? -ne 0 ];then
	echo "fail"
fi
