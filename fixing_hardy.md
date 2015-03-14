#A hacker way to fix Ubuntu hardy for Amora

# Introduction #

Ubuntu hardy has a faulty bluez package for months, its a well known [issue](https://bugs.launchpad.net/ubuntu/+source/bluez-utils/+bug/227429) which fix was not released yet for Hardy (but for Intrepid yes).

This makes several bluetooth client apps break, and so Amora too.

There is a way to workaround it, by installing Intrepid files. But be **warned** that it can bring problems to your system (mainly when upgrading, I guess).


# Details #

Download the [packages](http://amora.googlecode.com/files/hardy_blue_bug.tar.bz2) and extract it in a directory.

Install them in this order:

  * lsb-base: its a dependency

  * bluez-utils: has the fix

  * libbluetooth: updated bluetooth package

  * libbluetooth-dev: has the development packages (optional)

A faster way is to execute this script:

```
mkdir temp
cd temp
wget http://amora.googlecode.com/files/hardy_blue_bug.tar.bz2
tar -jxvf hardy_blue_bug.tar.bz2
sudo dpkg -i  lsb-base_3.2-14ubuntu1_all.deb bluez-utils_3.36-1ubuntu1_i386.deb libbluetooth2_3.36-1_i386.deb libbluetooth-dev_3.36-1_i386.deb 
```

After installing it, restart the computer. Amora server should be capable of work fine and cellphones will connect to your computer.


# ATTENTION #

I'm no responsible for any damage in you computer and this is a hard hack, so be warned.