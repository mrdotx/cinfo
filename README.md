# cinfo

a fast and minimal system information tool for linux-based operating systems

![screenshot](screenshot.png)

## installation

edit config.mk to match your local setup (cinfo is installed into
the /usr/local namespace by default)

afterwards enter the following command to build and install cinfo (if
necessary as root):

```bash
make clean install
```

## run

```bash
cinfo
```

## manual

you can view the manual by running `man cinfo.1`, or `man afetch`
if you've already ran `make clean install`.
