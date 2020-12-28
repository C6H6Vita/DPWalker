## DPWalker

A Dependency-Walker like application for Linux

### What is it?

This is  a free utility that scans Linux ELF-formated binaries(Executable, Shared library) and builds a hierarchical tree diagram of all dependent libraries. 

### Detailed Information

For each dependency libraries found, it lists detailed information  about each libraries including a full path to the file. It recursively search information for each libraries.

"Export Symbol" Menu displays all the functions that are exported by current selected module. It reviles detailed information about searched symbols: base address, visibility, size, library name, etc... 

"Import Symbol" Menu displays and which of those are actually being imported by other libraries. 

### How to build?

You need binutils binary, Qt5 development package (which include libQt5Widgets, libQt5Gui, libQt5Core) for depencency. And it uses qmake for build.

For Ubuntu-based distros, you can install these dependency by following command:

```sh
sudo apt install build-essentials binutils qtbase5-dev qmake
```

Then, run qmake by following command:

```sh
mkdir build && cd build
qmake ../DPWalker.pro
```

And run GNU make for obtain binary

```shell
make
```

Or you can use Qt Creator to build this software.

### How to use?

When running software with no args, it pop-ups file-open dialog. 

Select target ELF-format file(*.so, executable file), it begins parsing information, and displays module information at left TreeView widget. 

You can view "Import Symbols", and "Export Symbols" information when you just click the Library name.

### Tested environment

This software tested on Ubuntu 18.04

### Screenshot

![](/home/c6h6/install/DPWalker/MDResource/Screenshot.png)