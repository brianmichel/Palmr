# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|
  config.vm.box = "nexces/debian-etch"

  config.vm.provider "virtualbox" do |vb|
    vb.gui = true
    vb.customize ["modifyvm", :id, "--usb", "on"]
    vb.customize ["modifyvm", :id, "--usbehci", "off"]
  end

  # Install xfce and virtualbox additions
  config.vm.provision "shell", inline: "echo \"deb http://archive.debian.org/debian/ etch main non-free contrib\" > /etc/apt/sources.list"
  config.vm.provision "shell", inline: "apt-get update"
  config.vm.provision "shell", inline: "apt-get install -y gcc-2.95 g++-2.95 alien lynx unzip tar cogito git-core curl build-essential"
  config.vm.provision "shell", inline: "cd /usr/bin && ls -la *gcc* && echo \"--------------------\" && rm gcc && ln -s gcc-2.95 gcc && ls -la *gcc*"
  config.vm.provision "shell", inline: "mkdir ~/dev && cd ~/dev"
  config.vm.provision "shell", inline: "git clone git://github.com/wolf3d/palmdev"

  # Permit anyone to start the GUI
  config.vm.provision "shell", inline: "sudo sed -i 's/allowed_users=.*$/allowed_users=anybody/' /etc/X11/Xwrapper.config"
  config.ssh.forward_x11 = true
end
