# OreSat Linux

[![Issues](https://img.shields.io/github/issues/oresat/oresat-linux)](https://github.com/oresat/oresat-linux/issues)

This repo has all the image builder, system level documenation, and general
utils for OreSat Linux software developlement.

## System level documenation

For general design and reasoning behind it for all the Linux boards on OreSat.

### Building Sphinx Docs

- Install `python-sphinx` and `python-sphinx-rtd-theme`
- `$ make -C docs/`

## Building OreSat Linux images

See [images/readme.md](images/README.md)


## Other OreSat Linux Repos

### System level Software

Common software that is on all Linux Boards

- [oresat-linux-manager]: The front end interface for all OreSat Linux
boards. Convert CANopen message to DBus message and vice versa.
- [oresat-linux-updater]: The serive that allow the board to be updated.
Mostly a wrapper ontop of `apt`.

### GPS / ADCS Board Repos

- [oresat-gps-hardware]: Hardware design for the GPS board.
- [oresat-gps-software]: SDR GPS service.
- [oresat-adcs-software]: A ADCS Manager serive that control the ADCS subsystem.

### Star Tracker Board Repos

- [oresat-star-tracker]: Hardware design for the Star Tracker board.
- [oresat-star-tracker-software]: The Star Tracker service.
- [oresat-linux-prucam]: A kernel module for interfacing to camera with a PRU.
Also includes a python3 library wrapper.

### OreSat Live Board Repos

- [oresat-dxwifi-hardware]: Hardware design for the OreSat Live board.
- [oresat-dxwifi-software]:

### CFC (Cirrus Flux Camera) Board Repos

- [oresat-cfc-hardware]: Hardware design for the CFC board.

<!-- OreSat repos -->
[oresat-linux-manager]:https://github.com/oresat/oresat-linux-manager
[oresat-linux-updater]:https://github.com/oresat/oresat-linux-updater
[oresat-adcs-software]:https://github.com/oresat/oresat-adcs-software
[oresat-gps-software]:https://github.com/oresat/oresat-gps-software
[oresat-gps-hardware]:https://github.com/oresat/oresat-gps-hardware
[oresat-star-tracker]:https://github.com/oresat/oresat-star-tracker
[oresat-star-tracker-software]:https://github.com/oresat/oresat-star-tracker-software
[oresat-dxwifi-hardware]:https://github.com/oresat/oresat-dxwifi-hardware
[oresat-dxwifi-software]:https://github.com/oresat/oresat-dxwifi-software
[oresat-cfc-hardware]:https://github.com/oresat/oresat-cfc-hardware
[oresat-linux-prucam]:https://github.com/oresat/oresat-linux-prucam
