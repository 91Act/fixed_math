# Description

This is yet another Q31.32 fixed point project.

Involved math functions: sin, cos, tan, asin, acos, atan, exp, log, pow, sqrt. Most of them are based on LUT.

# Usage

|File|Description|
|-|-|
|fixed_math.h|Provides c-style functions.|
|fixed_number.h|The c++ wrapper class.|
|lut.py|Python script for LUT generation.|

# Build

LLVM is a recommanded toolchain and GCC should also work well.

To build for the platforms which are not support `__int128`, such as MSVC or 32bit arch platforms, you may need to clone a [boost](https://github.com/boostorg/boost) repo into this project to be an alternative.
