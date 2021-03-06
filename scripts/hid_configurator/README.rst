.. _configuration_channel:

Configuration channel for nRF52 Desktop
#######################################

Overview
********

This application is an example of controlling nRF52 Desktop firmware parameters at runtime.

It uses HID feature reports to communicate with firmware.

Right now, it can be used to change following parameters:

* mouse optical sensor resolution (CPI)
* optical sensor power saving modes:
	* downshift time from "Run" to "Rest1" mode
	* downshift time from "Rest1" to "Rest2" mode
	* downshift time from "Rest2" to "Rest3" mode

Supported transports:

* USB

Usage
*****
On Windows:

	py -3 configurator.py --cpi 12000

On Linux:

	python3 configurator.py --cpi 12000

To get a list of possible arguments, call with `--help` option.

Installation
************
To setup dependencies for the configurator program, run:
On Windows:

	py -3 -m pip install -r requirements.txt

On Linux:

	pip3 install --user -r requirements.txt

This installs:

* HIDAPI library - on Windows provided by `hidapi.dll` or `hidapi-x64.dll`
* pyhidapi Python wrapper - installed from GitHub repository with additional patches included. The version on PyPI is obsolete and will not work.

On Linux, to call the Python script without root rights,
install the provided udev rule `99-hid.rules` by copying it to
`/etc/udev/rules.d` and replugging the device.

Note:
************
When using Python Launcher for Windows, `--user` installation option will not work, since DLL files will be copied to `user\AppData\Roaming\Python\PythonX` which is not searched for DLL libraries by default. You can either install without `--user` option or copy the DLL library next to the program being run. To uninstall all files, run

	py -3 -m pip uninstall hid

