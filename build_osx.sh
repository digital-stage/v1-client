#!/bin/sh

echo "Do you want to install jack via brew?"
read -p "Continue (y/n)?" choice
case "$choice" in 
  y|Y ) brew install pkgconfig libxml2 glibmm libxml++ liblo libsndfile gtkmm3 eigen boost gsl fftw libsamplerate cpprestsdk nlohmann-json openssl coreutils qt jack;;
  n|N ) brew install pkgconfig libxml2 glibmm libxml++ liblo libsndfile gtkmm3 eigen boost gsl fftw libsamplerate cpprestsdk nlohmann-json openssl coreutils qt;;
  * ) echo "invalid";;
esac
git submodule update --init --recursive
mkdir -p bin
cd bin
QT_DIR=$(brew --prefix qt) cmake ./../
make
