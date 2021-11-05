% CINFO(1) Version\-VERSION | General Commands Manual

# NAME

cinfo - system information utility

# SYNOPSIS

**cinfo** [**-a**] [**-v**]

# DESCRIPTION

**cinfo** is a fast and minimal system information tool for linux-based operating systems

# OPTIONS

without given setting, start colored verion

**-a**
: start plain ascii version

**-v**
: prints version information

# CUSTOMIZATION

**cinfo** can be customized by creating a custom config.h and (re)compiling  the  sourc code.

**PKGS\***
: values to configure package manager counting

**INFO\***
: values to configure information in general

**SHELL\***
: values to configure shell information

**MEMORY\***
: values to configure memory information

**COLOR\***
: values to configure color table

**ASCII\***
: values to configure ascii table

**LABEL\***
: values to configure labels for ascii and color table

# EXAMPLES

| **cinfo**
| **cinfo** -a
| **cinfo** -v

# AUTHORS

**cinfo** was written by mrdotx <*klassiker@gmx.de*>

# LICENSE

See the LICENSE.md file for the terms of redistribution.
