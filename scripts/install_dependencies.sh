#!/bin/sh
echo "Installing dependent libraries via apt-get. Please enter the sudo password..."

sudo apt-get update

sudo apt-get -y install libx11-dev
sudo apt-get -y install libxext-dev libxrender-dev libxtst-dev libxt-dev
sudo apt-get -y install libcups2-dev
sudo apt-get -y install libfreetype6-dev
sudo apt-get -y install libasound2-dev
sudo apt-get -y install libfontconfig1-dev
sudo apt-get -y install libffi-dev
