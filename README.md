# Palmr

A repo for setting up and building PalmOS applications.

## Setup
You must have [XQuartz](https://www.xquartz.org) and [Vagrant](https://www.vagrantup.com) installed prior to continuing.

### Build Environment
1. From within the root of the repository, run `vagrant up`. This will setup *most* of the build environment for you!
2. Once the box is up, SSH into it using `vagrant ssh`.
3. Once inside of the VM, run `sudo apt-get install xbase-clients pilrcui`
4. After installation is complete, you may exit the SSH session.
5. Run `vagrant ssh -c pose` and wait for the emulator to show up on your screen!

> *Note:* Step 2 seems like a bug since those things are actually included in the vagrant setup information, but for whatever reason don't work unless the user installs them directly. Also, all of the Vagrant commands seem to execute with a locale in Polish; I don't know why and I haven't fixed it.

### Emulator
So the Palm emulator is a bit strange in that it doesn't come with any OS pre-installed into it, so we have to provide a base ROM for it.

1. Run `vagrant ssh -c pose` to boot up the emulator.
2. Right click in the emulator window and select the "new" Options
3. In the New Session window, select the empty ROM file drop down and select "Other..."
4. This will give you a crappy file viewer that can browse the local file system of the VM. You want to navigate to `/vagrant/ROMS` (`/vagrant` is actually where our repo gets mounted to the remote file system.)
5. Select `Palm OS 3.5-en-color.rom` and click OK
6. The other settings don't really matter, just hit OK on the new session screen, and watch your beautiful Palm device boot up.

## Compiling & Running
Since your machine doesn't have the required compilers and associated tools, we need to execute all build commands on the virtual machine, thankfully, Vagrant makes that pretty easy.

### Compiling
1. Run `vagrant ssh -c "cd /vagrant/app && make clean && make"`

### Running
1. Run `vagrant ssh -c "pose -psf [saved session file here] -load_apps /vagrant/app/palmr.prc"`
![](images/spin-up-session-med.gif)
