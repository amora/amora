Introduction
============

**Deprecated**: the project is kept online for historic reasons, as the server no longer work on current Linux Bluetooth stack and the devices that could run the client are long gone.

You got a cellphone with bluetooth and still need to manually control slides when making a presentation?

Don't worry anymore, Amora (A mobile remote assistant) implements control of mouse and most of keyboard shortcuts (ESC, ENTER, SPACE, arrow keys, etc) making easy this task. Not only that, but you can take screenshots of the active window in your current graphic session (i.e. see thumbnails of PC windows in your cellphone screen).

Currently, the client is implemented in Python for S60 (Nokia cellphones) and the server is written in C programming language using Xlib and XTest to send events for X session and POSIX socket API for I/O.

For bluetooth SDP I used BlueZ (so for while, this part of server is Linux only), there are plans to port the server to run in other Unix architectures (FreeBSD, Solaris, Mac OSX).

Amora 1.1 Released
------------------

Check downloads section and wiki articles explaining how to install.

**Attention**: Amora client needs to be installed in the **same** drive where you installed Python for S60, i.e. you must install it in drive 'E' if you had installed Python in drive 'E'.

**Attention**: Ubuntu Hardy (8.04) has a faulty bluez-utils package that breaks several bluetooth client apps (including Amora). You can see more information [here](http://groups.google.com/group/amora-user/t/d075c2aa2a1915b) and [here](https://bugs.launchpad.net/ubuntu/+source/bluez-utils/+bug/227429). There is a way to fix it, more details in [install](http://code.google.com/p/amora/wiki/how_to_install)


Credits
-------

Several people contributed in a way or another to make amora even better, have a look [here](https://github.com/xtaran/amora/wiki/credits)


**Sponsor**

> Special thanks to [INdT](http://www.indt.org.br) (Instituto Nokia de Tecnologia) that allowed me to apply some time working on this project.


Screenshots
-----------

You can see a screenshot of cellphone client (running in 3 cellphones with
different screen resolutions (N95, N5500, E61) all connected at **same** time
controlling 1 computer. Pay attention to the new stopwatch (added in
Amora 1.0) and image rotated in N95 (option available in 'Configuration'
menu option)

![wiki/cell_models.jpg](https://github.com/xtaran/amora/wiki/cell_models.jpg)


And here some screenshots of client version 0.8 (tested only in N93 and E61):

![wiki/connected.jpg](https://github.com/xtaran/amora/wiki/connected.jpg)


Wallpaper for not connected status:

![wiki/run03.jpg](https://github.com/xtaran/amora/wiki/run03.jpg)


Screenshot capture in action: you can control and see any active window in Linux desktop:

![wiki/slides.jpg](https://github.com/xtaran/amora/wiki/slides.jpg)
