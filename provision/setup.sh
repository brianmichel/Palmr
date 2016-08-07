# Performing Basic System Updates
echo "Updating Apt Sources..."
echo "deb http://archive.debian.org/debian/ etch main non-free contrib" > /etc/apt/sources.list
echo "Updating Apt & Installing Build Essentials..."
apt-get update
apt-get install -y gcc-2.95 g++-2.95 alien lynx unzip tar cogito git-core curl build-essential
echo "Putting GCC 2.95 In Place..."
cd /usr/bin && ls -la *gcc* && echo "--------------------" && rm gcc && ln -s gcc-2.95 gcc && ls -la *gcc*

# Setting up Palm Specific Stuff
echo "Making Palm Development Directory..."
rm -rf ~/dev && mkdir ~/dev && cd ~/dev
echo "Cloning Palmdev Resources..."
rm -rf palmdev && git clone git://github.com/wolf3d/palmdev /root/dev/palmdev
echo "Switching to Palm SDK 4.0 Setup Folder..."
cd ~/dev/palmdev/setup.4.0/ && ls -l
echo "Untarring PalmOS SDK 4.0..."
tar xvzf ~/dev/palmdev/setup.4.0/sdk40.tar.gz
echo "Installing PalmOS SDK 4.0"
alien -i -v palmos-sdk-4.0-1.noarch.rpm
echo "Moving Palmdev Folder To Local Location..."
rm -rf /usr/local/palmdev && cd /opt && mv palmdev /usr/local/ && cd /usr/local/palmdev && ln -s sdk-4 sdk

# Install Common Utilities Required Across Palm Development
echo "Installing Palm PRC Tools..."
apt-get install prc-tools
echo "Untarring PilRC Source Code & Compiling..."
apt-get install pilrc

# Install UI Form View and PalmOS Emulator
echo "Installing PilRCUI Form Viewer..."
apt-get install pilrcui
echo "Installing PalmOS Emulator..."
apt-get install pose
# This is weird, but the other version of POSE will not run, however,
# The included version of POSE will not work correctly because of missing deps.
# So we install POSE from apt, then install our POSE over it. Whatever.
alien -i -v ~/dev/palmdev/common/pose-3.5-2.i386.rpm
apt-get install pose-skins
apt-get install xbase-clients

echo "**** Setup Complete ****"
