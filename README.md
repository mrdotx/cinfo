# cinfo

a fast and minimal system information tool for linux-based operating systems

![screenshot](screenshot.png)

## Installation

[![Packaging status](https://repology.org/badge/vertical-allrepos/cinfo.svg)](https://repology.org/project/cinfo/versions)

Edit **config.mk** to match your local setup (cinfo is installed into
the /usr/local namespace by default)

Copy the **config.def.h** to **config.h** and edit the new file according to your needs.
Especially if you want to install cinfo on an non-Arch based distro!

Afterwards enter the following command to build and install cinfo (if
necessary as root):

```bash
make clean install
```

## Run

```bash
cinfo
```

## Manual

You can view the manual by running `man cinfo` or `man cinfo.1`,
if you've already ran `make clean install`.
