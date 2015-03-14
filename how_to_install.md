# Software installing instructions: Amora 1.1 #

## Cellphone S60 3ed ##
### If you have Python installed ###

**ATTENTION**: the installing SIS package is **only** for S60 3ed, if you have a S60 2ed., please use client [source](http://amora.googlecode.com/files/amora-client-1.1-0.tar.gz) version and check further instructions at [hacker](http://code.google.com/p/amora/wiki/how_to_install_source) page.

**ATTENTION**: Amora client needs to be installed in the **same** drive where you installed Python for S60, i.e. you must install it in drive 'E' if you had installed Python in drive 'E'.

**ATTENTION**: Ubuntu Hardy (8.04) has a faulty bluez-utils package that breaks several bluetooth client apps (including Amora). You can see more information [here](http://groups.google.com/group/amora-user/t/d075c2aa2a1915b) and [here](https://bugs.launchpad.net/ubuntu/+source/bluez-utils/+bug/227429). There is a really not-recomended way to [workaround](http://code.google.com/p/amora/wiki/fixing_hardy) it.


Simply download and install [amora-version.sis](http://amora.googlecode.com/files/amora-client-1.1.0.sis) to your cellphone. Assuming that
you had sent the file to your cellphone using bluetooth as a message:

![http://amora.googlecode.com/svn/wiki/install01.jpg](http://amora.googlecode.com/svn/wiki/install01.jpg)


Selecting it will display the questions: Install Amora: assistant?

![http://amora.googlecode.com/svn/wiki/install02.jpg](http://amora.googlecode.com/svn/wiki/install02.jpg)


Select Yes. Then it will display a security warning (its fine, choose continue
in the screens bellow)

![http://amora.googlecode.com/svn/wiki/install03.jpg](http://amora.googlecode.com/svn/wiki/install03.jpg)

![http://amora.googlecode.com/svn/wiki/install04.jpg](http://amora.googlecode.com/svn/wiki/install04.jpg)

![http://amora.googlecode.com/svn/wiki/install05.jpg](http://amora.googlecode.com/svn/wiki/install05.jpg)


Of course you can send the SIS file to cellphone in several different ways too
(using USB cable, copying to memory card, etc) and later select it using the
S60 file browser.

![http://amora.googlecode.com/svn/wiki/run01.jpg](http://amora.googlecode.com/svn/wiki/run01.jpg)

![http://amora.googlecode.com/svn/wiki/run02.jpg](http://amora.googlecode.com/svn/wiki/run02.jpg)

![http://amora.googlecode.com/svn/wiki/run03.jpg](http://amora.googlecode.com/svn/wiki/run03.jpg)

**tips**

  * Mouse cursor moving speed can be increased in Configuration screen -> M. delta (mouse delta). A suggested value when running Amora client is to increase it to 5 pixels.

  * Image screenshot rotation (useful for devices with 240x320 resolution) can be activated in Configuration screen -> Image -> Rotate

  * When controlling the desktop (a.k.a. 'started mode') you can reset as also toggle the stopwatch. If you dont want to see the stopwatch, just turn it off in Configuration -> Clock

  * Autoscreen mode will trigger a screenshot for each mouse click (with the cellphone joystick). This allow you to advance to the next slide in a presentation and at the **same** time get a screenshot

  * Autoscreen has a default wait value of 2s (between the mouse click and the screenshot). If your computer cannot advance to the next slide within 2s, you can change the wait time in Configuration -> A. screen (ranging from 1.0s up to 5.0)



If in early  steps of installing you see this image dialog

![http://amora.googlecode.com/svn/wiki/without01.jpg](http://amora.googlecode.com/svn/wiki/without01.jpg)

this means that you **don't** have Python installed. Just advance to next section
where you can find instructions.


### If you DONT have Python installed ###

Download the file [amora-version-deps.sis](http://amora.googlecode.com/files/amora-client-deps-1.1.0.sis) to your cellphone. It has Python (required dependency) and Python Scriptshell (optional) bundled together with Amora client for Nokia S60 smartphones.

Just follow through the screens selecting **continue**, as bellow:

![http://amora.googlecode.com/svn/wiki/without02.jpg](http://amora.googlecode.com/svn/wiki/without02.jpg)

![http://amora.googlecode.com/svn/wiki/without03.jpg](http://amora.googlecode.com/svn/wiki/without03.jpg)

![http://amora.googlecode.com/svn/wiki/without06.jpg](http://amora.googlecode.com/svn/wiki/without06.jpg)

![http://amora.googlecode.com/svn/wiki/without07.jpg](http://amora.googlecode.com/svn/wiki/without07.jpg)

![http://amora.googlecode.com/svn/wiki/without08.jpg](http://amora.googlecode.com/svn/wiki/without08.jpg)


### Symbian certificate control ###

It might happen Amora installation fail in silence thanks to Symbian certificate test (currently the SIS is signed with a fake certicate). As a workaround, you can turn certificate check acessing 'Application Manager' app and changing the setting as bellow:


![http://amora.googlecode.com/svn/wiki/sec01.jpg](http://amora.googlecode.com/svn/wiki/sec01.jpg)

![http://amora.googlecode.com/svn/wiki/sec02.jpg](http://amora.googlecode.com/svn/wiki/sec02.jpg)

![http://amora.googlecode.com/svn/wiki/sec03.jpg](http://amora.googlecode.com/svn/wiki/sec03.jpg)



## PC server ##

### Ubuntu users ###

Just download and install the  [Debian](http://amora.googlecode.com/files/amora-server_1.1-1_i386.deb) package. To install, run in a terminal:

```
$ sudo dpkg -i amora-server-version.deb
```

And if it installed fine, you should be able to start the server running a
terminal:

```
$ amorad
```

### Mandriva Users ###

Amora server was included in Mandriva Cooker official repository.

### Fedora Users ###

Fedora has Amora server [packaged](https://admin.fedoraproject.org/pkgdb/packages/name/amora).

### Debian Sid Users ###

Debian Unstable has Amora server [packaged](http://packages.debian.org/sid/amora-server).

### Installing from source ###

#### Software dependencies ####

For server side, to compile the [source](http://amora.googlecode.com/files/amora-server-1.1.tar.gz) code, you will need:

  * gcc compiler
  * Xlibs
  * XTest libs
  * BlueZ libs
  * Imlib2 libs
  * D-BUS libs

And also XTest extension enabled in you X server (default on Ubuntu). So,
in Ubuntu simply run in your terminal:

```
$ sudo apt-get install gcc libxext-dev libbluetooth2-dev libimlib2-dev libxtst-dev libdbus-1-dev
```

**remark**: you might need 'pkg-config' if you don't have it installed already.


#### Installing ####

After installing the dependencies, extract server app tarball with:

```
$ tar -zxvf amora-server-x.y.tar.gz
```

where _x.y_ is the **current** download version.

Change to the extracted directory and execute the following commands:

```
$ mkdir build

$ cd build

$ ../configure

$ make
```

It will generate an executable file named 'amorad', you must
start the application (and make sure that the bluetooth dongle is
connected to the computer):

```
$ ./amorad
```

## User group ##

Do you have questions, suggestions, bug reports or wish to share your experience with Amora? Please, visit Amora discussion group at: http://groups.google.com/group/amora-user