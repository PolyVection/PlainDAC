/*
 * plaindac.h  --  Sipmle cmdline application for controlling PCM51XX from userspace
 *
 * Copyright (C) 2015 Philip Voigt <info@polyvection.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


typedef unsigned char cfg_u8;

typedef union {
    struct {
        cfg_u8 offset;
        cfg_u8 value;
    };
    struct {
        cfg_u8 command;
        cfg_u8 param;
    };
} cfg_reg;



cfg_reg initSeq[] = {
    
    //EXIT SHUTDOWN STATE

    { 0x00, 0x00 }, // SELECT PAGE 0
    { 0x03, 0x00 }, // UNMUTE
    { 0x02, 0x00 }, // DISABLE STBY
    { 0x0d, 0x10 }, // BCK as SRC for PLL
    { 0x25, 0x08 }, // IGNORE MISSING MCLK
    { 0x3d, 0x55 }, // DIGITAL VOLUME L
    { 0x3e, 0x55 }, // DIGITAL VOLUME R
    { 0x08, 0x08 }, // GPIO4 OUTPUT ENABLE
    { 0x53, 0x02 }, // GPIO4 REGISTER OUTPUT
    { 0x56, 0x08 }, // GPIO4 HIGH
    
};


cfg_reg stopSeq[] = {

    { 0x56, 0x00 }, // GPIO4 LOW
    
};
