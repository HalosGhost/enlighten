enlighten
=========

Enlighten is a *very* small C utility to control the backlight brightness in
Linux.

Features
--------

* Set brightness to a particular level!
* Retrieve current brightness level!
* Increase *and* Decrease brightness level relative to the current level!
* Use percentage of maximum brightness *or* absolute steps!
* Specify backlight device in an environment variable!

Non-Features
------------

* No auto-dimming
* No brightness level save/restore (note, systemd `supports this <https://wiki.archlinux.org/index.php/Backlight#systemd-backlight_service>`_)
* Cannot order pizzas
* Cannot read mail (`yet <http://catb.org/jargon/html/Z/Zawinskis-Law.html>`_)

ToDo
----

* Add man page

Install
-------

Dependancies: clang

Manual installation:
::

    ./configure --device=<device>
    make
    sudo make install

Replace <device> with the name of your backlight device. It should be listed in
:code:`/sys/class/backlight`. Intel graphics for example calls the device :code:`intel_backlight`.

Arch Linux package: https://aur.archlinux.org/packages/enlighten-git/

Usage
-----

For usage information type :code:`enlighten --help`
