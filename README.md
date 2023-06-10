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

Clone the git repository

```bash
$ git clone https://github.com/oresat/oresat-linux
````

Install dependencies

**NOTE**: May need to replace `pip` with `pip3` on your system (it varies based of OS and distro).

```bash
$ pip install -r docs/requirements.txt
```

Build the docs

```bash
$ make html -C docs
```

Open `docs/build/html/index.html` in a browser.

## Other OreSat Linux Repos

See [docs/repos.rst](docs/repos.rst) for a list of other OreSat Linux repos.
