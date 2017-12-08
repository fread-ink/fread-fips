# fread.ink print screen tool

A tool for printing text and graphics to the eink display. The image data is printed directly to the display's framebuffer.

## Installation

1. Download the `fips` executable from the [latest release](https://github.com/fread-ink/fread-fips/releases/latest). It is compiled for the fread.ink firmware.
1. Copy the `fips` executable to the device, e.g. via [USB networking](https://github.com/fread-ink/fread-vagrant#enabling-usb-networking).

## Usage

### Display an image

`./fips -g image_path [-f]`

where:
- _-g_ is the parameter for displaying a PNG image
- _image_path_ is the path to the image's file
- _-f_ is an optional parameter for making a full update of the display, the default is partial update

### Clear the display

`./fips -c`

where:
- _-c_ is the parameter for clearing the display

## Build

The easiest way to build the tool from sources is to use the toolchain in the [fread-vagrant](https://github.com/fread-ink/fread-vagrant) VM.

1. Copy the `fips.c` source file to the running `fread-vagrant` VM.
1. SSH to the `fread-vagrant` VM.
1. Execute `sudo apt-get install libpng12-dev` to install the include files for `libpng`. This is required for the tool to compile to its dependent libraries.
1. Copy the `libpng12.so.0.50.0` and `libz.so.1.2.8` shared libraries to the same folder where `fips.c` was copied. This is required for the tool to link to its dependent libraries. You can find these libraries in the [fread-userland](https://github.com/fread-ink/fread-userland) VM or on a device with the fread.ink firmware already installed.
1. Execute the following command:

```sh
arm-linux-gnueabihf-gcc -std=gnu99 -I/usr/include -I/usr/include/libpng fips.c -o fips -l:libpng12.so.0.50.0 -l:libz.so.1.2.8
```

The `fips` executable will be generated in the same folder. You can now copy it to a device which has the fread.ink firmware installed.
