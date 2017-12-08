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
