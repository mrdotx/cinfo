% CINFO(1) Version\-VERSION | cinfo Manual

# NAME

cinfo - system information utility

# SYNOPSIS

**cinfo** [**-a**] [**-c**] [**-d**] [**-i**]

# DESCRIPTION

**cinfo** is a fast and minimal system information tool for linux-based operating systems

# OPTIONS

Without given settings, start colored version. If the environment variable **NO_COLOR** is set, start plain ascii version.

**-a**
: Start plain ascii version.

**-c**
: Start colored version.

**-d**
: Delete cached files.

**-i**
: Displays information about execution times of the individual functions for optimization and the version number of the tool

# CUSTOMIZATION

**cinfo** can be customized by creating/editing a custom config.h and (re-)compiling the source code.

***PKGS\****
: Values to configure package manager counting.

***CPU\****
: Values to configure cpu information.

***MEMORY\****
: Values to configure memory information.

**MEMORY_METHOD**

| Value | Calculation                             | Comparable                  |
| :---: | :-------------------------------------- | :-------------------------- |
| 0     | total - available                       | htop = 3.1.0, btop, polybar |
| 1     | total + shared - free - buffer - cached | htop < 3.1.0, neofetch      |
| 2     | total - free - buffer - cached          | htop > 3.1.0, free          |

**MEMORY_UNIT**

| Value | Condition         | Description                 |
| :---- | :---------------- | :-------------------------- |
| auto  | total => 1024 MiB | Displays result in Gibibyte |
|       | total < 1024 MiB  | Displays result in Mebibyte |
| GiB   |                   | Displays result in Gibibyte |
| MiB   |                   | Displays result in Mebibyte |

***SHELL\****
: Values to configure shell information.

***INFO\****
: Values to configure information in general.

***LABEL\****
: Values to configure labels for ascii and color table.

***COLOR\****
: Values to configure color table.

***ASCII\****
: Values to configure ascii table.

***CACHE\****
: Values to configure the cache paths.

# EXAMPLES

| **cinfo**
| **cinfo** -a
| **cinfo** -c
| **cinfo** -d
| **cinfo** -i

# BUGS

See GitHub Issues: *https://github.com/mrdotx/cinfo/issues*

# AUTHORS

**cinfo** was written by mrdotx <*klassiker@gmx.de*>

# SEE ALSO

**htop**(1), **free**(1), **neofetch**(1), **polybar**(1)

# LICENSE

See the *LICENSE.md* file for the terms of redistribution.
