# OreSat Linux

[![Issues](https://img.shields.io/github/issues/oresat/oresat-linux)](https://github.com/oresat/oresat-linux/issues)
[![Documentation Status](https://readthedocs.org/projects/oresat-linux/badge/?version=latest)](https://oresat-linux.readthedocs.io/en/latest/?badge=latest)

This repo has all the OreSat Linux system level documenation and utilities for
OreSat Linux boards.

## Utilites

- **Image Builder:** Used to build OreSat Linux images.
- **Octavo Boot:** Used to flash a OreSat Linux board's eMMC over USB.
- **Octavo EEPROM:** Used to flash a new OreSat Linux board's EEPROM.

## OreSat Linux Images

Images are hosted at [https://images.oresat.org/](https://images.oresat.org/)

## System Level Documenation

See [OreSat Linux ReadTheDocs](https://oresat-linux.readthedocs.io/en/latest/)

Manually building the System level docs:

- Install dependencies
  - On Arch Linux: `$ sudo pacman -S python-sphinx python-sphinx-rtd-theme`
  - On Debian Linux: `$ sudo apt install python3-sphinx python3-sphinx-rtd-theme`
  - With pip: `$ pip install sphinx sphinx-rtd-theme`
- `$ make html -C docs/`
- open `docs/build/html/index.html` in a browser.

## Other OreSat Linux Repos

See [docs/repos.rst](docs/repos.rst) for a list of other OreSat Linux repos.
