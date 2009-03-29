#!/usr/bin/make -f
#
# Description:
#
#   A makefile that automates the process of building a MinGW cross compiler.
#
# Usage:
#
#    make -f xmingw32.mak all test
#
#   or simply
#
#    ./xmingw32.mak all test
#
# License:
#
#   Copyright (C) 2003  José Fonseca
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License
#   as published by the Free Software Foundation; either version 2
#   of the License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License, http://www.gnu.org/copyleft/gpl.html
#   for more details.
#
# Author:
#
#   José Fonseca
#
# Changelog:
#
#   * Wed, 15 Sep 2004 14:46:48 +0100
#    - Updated to the latest versions (binutils-2.15.90-20040222-1,
#    gcc-3.3.3-20040217-1, mingw-runtime-3.3 and w32api-2.5).
#
#   * Mon, 23 Jun 2003 11:57:39 +0100
#    - Updated to the latest versions (binutils-2.13.90-20030111-1,
#    gcc-3.2.3-20030504-1, mingw-runtime-3.0 and w32api-2.3).
#    - Use of patches instead of hard-coded sed commands.
#    - Automatic detection of the installation directory.
#    - Small reorganization.
#
#   * before
#    - lost in time...


# Target definition

TARGET = mingw32


# Versions defintions

#BINUTILS_VERSION = 2.17.50-20060824-1
BINUTILS_VERSION = 2.19.1
GCC_VERSION = 3.4.5-20060117-2
MINGW_RUNTIME_VERSION = 3.15.2
W32API_VERSION = 3.13


# Installation directory

ifeq ($(shell whoami),root)
PREFIX=/usr/local/xmingw32
else
PREFIX=$(PWD)/../xmingw32
endif


# Choose one of the SourceForge mirrors.

#BASE_URL = ftp://ftp.sf.net/pub/sourceforge/mingw/
#BASE_URL = http://telia.dl.sourceforge.net/sourceforge/mingw/
#BASE_URL = http://unc.dl.sourceforge.net/sourceforge/mingw/
#BASE_URL = http://belnet.dl.sourceforge.net/sourceforge/mingw/
#BASE_URL = http://heanet.dl.sourceforge.net/sourceforge/mingw/
#BASE_URL = http://surfnet.dl.sourceforge.net/sourceforge/mingw/
BASE_URL = http://garr.dl.sourceforge.net/sourceforge/mingw/

# Some shortcut variables
#
# NOTE: The upstream packaging is not very consistent so often these have to be
# tweaked for each specific version.

BINUTILS_TARBALL = binutils-$(BINUTILS_VERSION)-src.tar.gz
#GCC_TARBALL = gcc-$(GCC_VERSION)-src.tar.gz
GCC_TARBALL = gcc-core-$(GCC_VERSION)-src.tar.gz gcc-g++-$(GCC_VERSION)-src.tar.gz
MINGW_RUNTIME_TARBALL = mingwrt-$(MINGW_RUNTIME_VERSION)-mingw32-dev.tar.gz
W32API_TARBALL = w32api-$(W32API_VERSION)-mingw32-dev.tar.gz

BINUTILS_PATCH = binutils-$(BINUTILS_VERSION).patch
GCC_PATCH = gcc-$(GCC_VERSION).patch

BINUTILS_DIR = binutils-$(BINUTILS_VERSION)
GCC_DIR = gcc-$(GCC_VERSION)


# Configure flags
#
# NOTE: Some of these flags require certain versions. See the upstream
# gcc-*-build.sh scripts for more information.

CONFIGURE_FLAGS = \
	--target=$(TARGET) \
	--prefix=$(PREFIX)

BINUTILS_CONFIGURE_FLAGS = \
	$(CONFIGURE_FLAGS)

GCC_CONFIGURE_FLAGS = \
	$(CONFIGURE_FLAGS) \
	--enable-languages=c,c++ \
	--disable-nls \
	--disable-shared \
	--enable-sjlj-exceptions \
	--enable-threads \
	--disable-win32-registry


# Build flags

CFLAGS = -O2
LDFLAGS = -s
DEBUG_FLAGS = -g0

export CFLAGS LDFLAGS DEBUG_FLAGS


# Set the path environment variable

PATH := $(PREFIX)/bin:$(PATH)

export PATH


# Default target

all: download binutils mingw-runtime w32api gcc scripts

.PHONY: all


# Download the tarballs

WGET = wget
WGET_FLAGS = -T 10 --passive-ftp

SOURCES = \
	$(BINUTILS_TARBALL) \
	$(GCC_TARBALL) \
	$(MINGW_RUNTIME_TARBALL) \
	$(W32API_TARBALL)

download: $(SOURCES)

$(SOURCES):
	$(WGET) $(WGET_FLAGS) $(BASE_URL)$@

.PHONY: download


# Create the installation directories

$(PREFIX):
	mkdir -p $@

$(PREFIX)/$(TARGET):
	mkdir -p $@


# Unpack, patch, build and install the binutils

$(BINUTILS_DIR): $(BINUTILS_TARBALL)
	gzip -dc $(BINUTILS_TARBALL) | tar xf -
	[ -f $(BINUTILS_PATCH) ] && patch -p0 -i $(BINUTILS_PATCH) || true
	touch $@

$(BINUTILS_DIR)/Makefile: $(BINUTILS_DIR)
	cd $(BINUTILS_DIR) && \
		./configure $(BINUTILS_CONFIGURE_FLAGS) > ../binutils-configure.log 2>&1

binutils: .binutils-stamp

.binutils-stamp: $(BINUTILS_DIR)/Makefile $(PREFIX)
	cd $(BINUTILS_DIR) && \
		make > ../binutils-build.log 2>&1
	cd $(BINUTILS_DIR) && \
		make install > ../binutils-install.log 2>&1
	touch $@

.PHONY: binutils


# Install the MinGW C runtime headers and libraries

mingw-runtime: .mingw-runtime-stamp

.mingw-runtime-stamp: $(RUNTIME_TARBALL) $(PREFIX)/$(TARGET)
	gzip -dc $(MINGW_RUNTIME_TARBALL) | tar xf - -C $(PREFIX)/$(TARGET) > mingw-runtime-install.log 2>&1
	touch $@

.PHONY: mingw-runtime


# Install the MinGW Win32 API headers and libraries

w32api: .w32api-stamp

.w32api-stamp: $(W32API_TARBALL) $(PREFIX)/$(TARGET)
	gzip -dc $(W32API_TARBALL) | tar xf - -C $(PREFIX)/$(TARGET) > w32api-install.log 2>&1
	touch $@

.PHONY: w32api


# Unpack, patch, build and install the gcc

$(GCC_DIR): $(GCC_TARBALL)
	$(foreach TARBALL,$(GCC_TARBALL), gzip -dc $(TARBALL) | tar xf - ;)
	[ -f $(GCC_PATCH) ] && patch -p0 -i $(GCC_PATCH) || true
	touch $@

$(GCC_DIR)/Makefile: $(GCC_DIR)
	cd $(GCC_DIR) && \
		./configure $(GCC_CONFIGURE_FLAGS) > ../gcc-configure.log 2>&1

gcc: .gcc-stamp

.gcc-stamp: .binutils-stamp .mingw-runtime-stamp .w32api-stamp $(GCC_DIR)/Makefile $(PREFIX)
	#cd $(GCC_DIR)/gcc && \
	#	make installdirs > ../gcc-build.log 2>&1
	cd $(GCC_DIR) && \
		make >> ../gcc-build.log 2>&1
	cd $(GCC_DIR) && \
		make install > ../gcc-install.log 2>&1
	touch $@

.PHONY: gcc


# Creates scripts to facilitate the use of the cross compiler toolchain

scripts: xmingw32.sh xmingw32.bat

xmingw32.sh: xmingw32.sh.in
	sed -e s%@target@%$(TARGET)%g -e s%@prefix@%$(PREFIX)%g $< > $@

xmingw32.bat: xmingw32.bat.in
	sed -e s%@target@%$(TARGET)%g -e s%@prefix@%$(PREFIX)%g $< > $@

.PHONY: scripts


# A small test case

test: test1.exe test2.exe

test1.exe: hello.c
	$(TARGET)-gcc -o $@ $<

test2.exe: hello.cxx
	$(TARGET)-g++ -o $@ $<

.PHONY: test


# Cleans the build trees

clean:
	rm -f .*-stamp
	rm -f *.log
	rm -rf $(BINUTILS_DIR) $(GCC_DIR)
	rm -f xmingw32.sh xmingw32.bat
	rm -f test*.exe

.PHONY: clean


# Uninstall the cross compiler (Use with care)

uninstall:
	rm -rf $(PREFIX)

.PHONY: uninstall
