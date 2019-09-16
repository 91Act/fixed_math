#!/usr/bin/python

import math

def tofixed(value):
    ival = int(value)
    fval = value - ival

    return "static_cast<fixed_point>(0x%016X)/* %11.8f */" % ((ival << 32) + int(fval * (1 << 32)) & 0xFFFFFFFFFFFFFFFF, value)

with open("lfixed_lut.h", "w") as file:
    file.write("#pragma once\n")
    file.write("\nstatic fixed_point fixed_lut_cos[] = \n")
    file.write("{\n")
    for i in range(0, 1024):
        if i % 16 == 0: file.write("    ")
        file.write(tofixed(math.cos(math.pi * i / 1024.0)) + ", ")
        if i % 16 == 15: file.write("\n")
    file.write("};\n")

    file.write("\nstatic fixed_point fixed_lut_asin[] = \n")
    file.write("{\n")
    for i in range(0, 1024):
        if i % 16 == 0: file.write("    ")
        file.write(tofixed(math.asin(-1 + 2 * i / 1024.0)) + ", ")
        if i % 16 == 15: file.write("\n")
    file.write("};\n")

    file.write("\nstatic fixed_point fixed_lut_atan[] = \n")
    file.write("{\n")
    for i in range(0, 1024):
        if i % 16 == 0: file.write("    ")
        file.write(tofixed(math.atan(-1 + 2 * i / 1024.0)) + ", ")
        if i % 16 == 15: file.write("\n")
    file.write("};\n")

    file.write("\nstatic fixed_point fixed_lut_exp[] = \n")
    file.write("{\n")
    for i in range(0, 1024):
        if i % 16 == 0: file.write("    ")
        file.write(tofixed(math.exp(i / 1024.0)) + ", ")
        if i % 16 == 15: file.write("\n")
    file.write("};\n")

    file.write("\nstatic fixed_point fixed_lut_ln[] = \n")
    file.write("{\n")
    for i in range(0, 1024):
        if i % 16 == 0: file.write("    ")
        file.write(tofixed(math.log(1 + i / 1024.0)) + ", ")
        if i % 16 == 15: file.write("\n")
    file.write("};\n")
