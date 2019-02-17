#!/bin/sh
echo -n | openssl s_client -connect scan.coverity.com:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | sudo tee -a /etc/ssl/certs/ca-

# Set up compilers
if [ ! -z "${MATRIX_CC}" ]; then
    eval "${MATRIX_CC}"
fi

# build libgtest & libgtest_main
sudo mkdir /usr/src/gtest/build
cd /usr/src/gtest/build
sudo cmake .. -DBUILD_SHARED_LIBS=1
sudo make -j4
sudo ln -s /usr/src/gtest/build/libgtest.so /usr/lib/libgtest.so
sudo ln -s /usr/src/gtest/build/libgtest_main.so /usr/lib/libgtest_main.so

cd ~/
git clone https://github.com/TES3MP/CrabNet
cd CrabNet
cmake . -DCRABNET_ENABLE_DLL=OFF -DCRABNET_ENABLE_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release
make -j3

