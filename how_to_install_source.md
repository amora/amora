# Software installing instructions #

## Cellphone client ##
### Software Dependencies ###
The client requires installing Python for S60 (version 1.4.4), and you can download it from [sourceforge](http://sourceforge.net/project/showfiles.php?group_id=154155). Pay attention that there are different versions of PyS60 for 2ed and 3ed devices.

To install, copy the 2 sis files (python and the script shell) to your
cellphone and select them using the standard file browser application
of cellphone. Alternatively, you can see other ways to install
[here](http://wiki.opensource.nokia.com/projects/Installing_PyS60).

### Installing ###
Create a folder in your cellphone memory card (E:\Python) or main
cellphone memory (C:\Python) and copy the contents of amora [client](http://amora.googlecode.com/files/amora-client-1.0svn.tar.gz)
file distribution to this folder.

**Important**: You must copy the contents of amora client to \Python directory not including amora-client folder itself. It must be **C|E:\Python\{client subdirs and files}** instead of **C|E:\Python\amora-client\{client subdirs folders and files}**.


**Attention**: there is a report of Amora client freezing when running in a N70 (S60 2ed) for a extended period. If you have this problem, a workaround is to turn off the stopwatch.


### Running ###
You must use python script shell (there is one icon for it in
'installed applications') to launch amora client (the file named
'amora.py'). In python script shell, select 'Options->Run script'
and select 'amora.py' in the list of available scripts.

Amora client should execute them, if you had followed all instructions.


**Tips**

  * Mouse cursor moving speed can be increased in Configuration screen -> M. delta (mouse delta). A suggested value when running Amora client inside Python Script Shell is 8

  * Image screenshot rotation (useful for devices with 240x320 resolution) can be activated in Configuration screen -> Image -> Rotate

## PC server ##
### Software dependencies ###

You can download the server source file [here](http://amora.googlecode.com/files/amora-server-1.0svn.tar.gz).


For server side, to compile the source code, you will need:

  * gcc compiler
  * Xlibs
  * XTest libs
  * BlueZ libs
  * Imlib2 libs

And also XTest extension enabled in you X server (default on Ubuntu). So,
in Ubuntu simply run in your terminal:

```
$ sudo apt-get install gcc libxext-dev libbluetooth2-dev libimlib2-dev libxtst-dev
```

### Installing ###
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

## Repository version ##

If you checked out the code from repository, you need first to generate the build system scripts. This one requires autotools packages (automake, etc).

You first run in the directory amora-server:

```
$ autoreconf -i --force
```

and then just follow previous instructions for tarball version.


## User group ##

Do you have questions, suggestions, bug reports or wish to share your experience with Amora? Please, visit Amora discussion group at: http://groups.google.com/group/amora-user