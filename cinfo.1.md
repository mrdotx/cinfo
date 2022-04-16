% CINFO(1) Version\-VERSION | cinfo Manual

# NAME

cinfo - system information utility

# SYNOPSIS

**cinfo** [**-a**] [**-c**] [**-i**]

# DESCRIPTION

**cinfo** is a fast and minimal system information tool for linux-based operating systems

# OPTIONS

Without given setting, start colored verion.

**-a**
: Start plain ascii version.

**-c**
: Clear cached files.

**-i**
: Shows informations about execution times of the individual functions for optimization and the version

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

| Value | Condition         | Description              |
| :---- | :---------------- | :----------------------- |
| auto  | total => 1024 MiB | Shows result in Gibibyte |
|       | total < 1024 MiB  | Shows result in Mebibyte |
| GiB   |                   | Shows result in Gibibyte |
| MiB   |                   | Shows result in Mebibyte |

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
| **cinfo** -i

# BUGS

See GitHub Issues: *https://github.com/mrdotx/cinfo/issues*

# AUTHORS

**cinfo** was written by mrdotx <*klassiker@gmx.de*>

# SEE ALSO

**htop**(1), **free**(1), **neofetch**(1), **polybar**(1)

# LICENSE

See the *LICENSE.md* file for the terms of redistribution.
