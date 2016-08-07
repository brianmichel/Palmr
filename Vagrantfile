# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure('2') do |config|
  config.vm.box = 'nexces/debian-etch'

  config.vm.provider 'virtualbox' do |vb|
    vb.gui = true
    vb.customize ['modifyvm', :id, '--usb', 'on']
    vb.customize ['modifyvm', :id, '--usbehci', 'off']
  end

  # Permit anyone to start the GUI
  config.vm.provision 'shell', inline: "sudo sed -i 's/allowed_users=.*$/allowed_users=anybody/' /etc/X11/Xwrapper.config"
  config.ssh.forward_x11 = true

  # Setup Locale
  config.vm.provision 'Setup Palm Development Environment', type: 'shell', path: 'provision/setup.sh'
end
