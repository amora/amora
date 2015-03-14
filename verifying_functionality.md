# Introduction #

This is thought as testing procedure to make sure that amora is working
correctly after changes to the code have been made.

See also [issue 53](http://code.google.com/p/amora/issues/detail?id=53)

# Basic usage #
## Bluetooth not found ##
  * Make sure the Bluetooth dongle is not plugged in. (If it's internal try to go into aircraft mode. If that's not available, go to _Starting the server_.)
  * Start amorad. It should say
```
No bluetooth device/dongle available. Aborting...
```
> and exit with return code 255. You can check that by typing
```
echo $?
```
> immediately afterwards.

## Bluetooth dongle removal ##

  * Plug in the Bluetooth dongle if not already plugged in. Make sure that the appropriate drivers are loaded, e.g. by calling
```
lsmod
```
  * Start amorad again. It should say
```
Initialization done, waiting cellphone connection...
Entering main loop...
```
  * Now remove the dongle, amora server should say
```
Done, we are closing now.
```


## Starting the server and connecting ##
  * Plug in the Bluetooth dongle if not already plugged in. Make sure that the appropriate drivers are loaded, e.g. by calling
```
lsmod
```
  * Start amorad again. It should say
```
Initialization done, waiting cellphone connection...
Entering main loop...
```
  * Now start amora on your mobile phone.
  * It should show the sleeping penguin.
  * Press the **Options** soft-key.
  * Choose **Search devices**. (If the phone ask if it should enable bluetooth, answer _yes_.)
  * If your computer is in the list, it should have a small bluetooth icon right of the name.
  * If it's not in the list, choose "More devices" and wait until you see your computer.
  * Select you computer. A list of ports should show up, containing at least one item saying
```
16: Amora: assistant
```
  * Select it. Several things should now happen:
    * The penguin should awake. The amora logo is in full color and has a small bluetooth logo besides.
    * amorad should have given some output including the bluetooth MAC address of your mobile phone:
```
Accepted connection. Client is 00:11:22:33:44:55
```
## Using Amora ##
  * Press the **Options** soft-key again.
  * Select **Start**. You now should see two things:
    * The help page on the mobile phone.
    * A clock running in the top left of your mobile phone display.
  * Use the joystick on the mobile phone. The cursor on your computer should move.
  * Press the main button of the phone. A left mouse click should have happened on the computer.
  * Press **1** on the phone. Use the joystick on the phone. The computer should do a drag (or mark text).
  * Press **1** again. The computer should do a drop (as in _drag and drop_) or stop marking text.
  * Press **2** to make a screenshot. On the mobile phone you should a screen shot of the window the mouse cursor was over on the computer.
  * Press the **Option** softkey (not labeled this time). You should get the menu.
  * Select **Auto screen** and move the mouse cursor over some other window and press the phones main button. Now this window should automatically show up on the phone as screen shot.
  * Press the **Option** softkey (not labeled this time). You should get the menu.
  * Select **Help**. You should get a notice that the joystick moves the mouse and see the help screen on phone shortly after this notice.
  * Move the mouse cursor into some text input window and click into the text.
  * Press the keys **4** and **6**. The text cursor should move left and right.
  * Move the mouse over some window or textbox whose content is longer than the space to display it.
  * Press the keys **7** and **`*`**. The content should move up and down.
  * Move the mouse somewhere where you can enter text.
  * Press the key **5**. Some newline should have been entered.
  * Press the keys **0** and **`#`**. A blank and an "f" should have been typed in that window.