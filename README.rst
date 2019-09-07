enlighten
=========

Enlighten is a *very* small C utility to control the backlight brightness in
Linux.

Features
--------

* Leverage backlight sysfs interfaces!
* Set brightness to a particular level!
* Retrieve current brightness level!
* Increase *and* Decrease brightness level relative to the current level!
* Use percentage of maximum brightness *or* absolute steps!
* Specify backlight device in an environment variable!
* Specify a list of directories to search for candidate devices in an environment variable!
* Specify minimum and maximum thresholds in environment variables!
* Specify smooth transition settings in environment variables!
* List all relevent environment configuration at runtime!

Non-Features
------------

* No auto-dimming
* No brightness level save/restore (note, systemd `supports this <https://wiki.archlinux.org/index.php/Backlight#systemd-backlight_service>`_)
* Cannot order pizzas
* Cannot read mail (`yet <http://catb.org/jargon/html/Z/Zawinskis-Law.html>`_)

Install
-------

Dependencies: clang

Manual installation:
::

    ./configure --device=<device>
    make
    sudo make install

Replace <device> with the name of your backlight device. It should be listed in
:code:`/sys/class/backlight`. Intel graphics for example calls the device :code:`intel_backlight`.

Arch Linux package: https://aur.archlinux.org/packages/enlighten-git/

Run as regular user
-------------------

Given that you have installed and loaded the prepackaged backlight udev rule, or that you have manually changed the group permissions on the ``brightness`` file of your device.
Add your user to the video group to run enlighten as a regular user::

    usermod -a -G video <username>

Usage
-----

For usage information type :code:`enlighten help`
