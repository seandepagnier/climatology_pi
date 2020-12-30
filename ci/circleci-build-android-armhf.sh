#!/bin/sh  -xe

#
# Build the Android artifacts inside the circleci linux container
#


set -xe

pwd

#not needed for CI built, the workflow starts up already in "project" directory.
# but required for local build.
#cd project

ls -la

#sudo chown -R 1000 /oesenc_pi/build_android_64_ci

sudo apt-get -q update
sudo apt-get -y install git cmake gettext unzip

# Get the OCPN Android build support package.
#NOT REQUIRED FOR LOCAL BUILD
echo "CIRCLECI_LOCAL: $CIRCLECI_LOCAL"
if [ -z "$CIRCLECI_LOCAL" ]; then
   wget https://github.com/bdbcat/OCPNAndroidCommon/archive/master.zip
fi
unzip -qq -o master.zip

pwd
ls -la

#change this for local build, so as not to overwrite any other generic buildin "build".
#sudo mkdir -p build_android_arm32
#cd build_android_arm32
mkdir -p build
cd build

rm -f CMakeCache.txt

cd ..

cmake  \
  -D_wx_selected_config=androideabi-qt-armhf \
  -DwxQt_Build=build_android_release_19_static_O3 \
  -DQt_Build=build_arm32_19_O3/qtbase \
  -DCMAKE_AR=/opt/android/android-ndk-r20/toolchains/llvm/prebuilt/linux-x86_64/bin/arm-linux-androideabi-ar \
  -DCMAKE_CXX_COMPILER=/opt/android/android-ndk-r20/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang++ \
  -DCMAKE_C_COMPILER=/opt/android/android-ndk-r20/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi21-clang \
  -DOCPN_Android_Common=OCPNAndroidCommon-master \
  -DPREFIX=/ \
  ..


make
make package

#  All below for local docker build
#ls -l

#xml=$(ls *.xml)
#tarball=$(ls *.tar.gz)
#tarball_basename=${tarball##*/}

#echo $xml
#echo $tarball
#echo $tarball_basename
#sudo sed -i -e "s|@filename@|$tarball_basename|" $xml


#tmpdir=repack.$$
#sudo rm -rf $tmpdir && sudo mkdir $tmpdir
#sudo tar -C $tmpdir -xf $tarball_basename
#sudo cp oesenc-plugin-android-armhf-16.xml metadata.xml
#sudo cp metadata.xml $tmpdir
#sudo tar -C $tmpdir -czf $tarball_basename .
#sudo rm -rf $tmpdir
