/*
 * plaindac.c  --  Sipmle cmdline application for controlling PCM51XX from userspace
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



#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "plaindac.h"





void getVolume(int fd){

    float vol;

    char bufferL[1];
    char bufferR[1];
    
    bufferL[0]=0x3d;
    bufferR[0]=0x3e;
    
    write(fd, bufferL, 1);
    read(fd, bufferL,1);
    
    write(fd, bufferR, 1);
    read(fd, bufferR,1);
    

    printf("LEFT: -%02d dB\n", (int)bufferL[0]);
    printf("RIGHT: -%02d dB\n", (int)bufferR[0]);

}




void setBalance(int balance){

}




void setVolume(int volume, int fd){
    
    //we want to have a range from 0-100 for volume control
    float multi = 2.55;
    volume =(int)volume*multi;
    
    //stop at 255
    if (volume > 255) {
        volume = 255;
    }
    
    char bufL[10];
    char bufR[10];
    bufL[0] = 0x3d;
    bufR[0] = 0x3e;
    
    //toggle all bits and store in buffer
    bufL[1] = ~volume;
    bufR[1] = ~volume;
    
    //write buffer to I2C dev
    write(fd, bufL, 2);
    write(fd, bufR, 2);
    
    
}






void transmit_registers(cfg_reg *r, int n, int fd)
{
    int i = 0;
    while (i < n) {
        write(fd,(unsigned char *)&r[i], 2);
        i++;
    }
}




int main(int argc, char *argv[])
{
    extern char *optarg;
    extern int optind;
    
    int c;
    int err = 0;
    int startflag=0;
    int pauseflag=0;
    int stopflag=0;
    int volflag=0;

    int volume = 55;
    
    static char usage[] = "usage: %s [-up] -v volume\n";
    
    while ((c = getopt(argc, argv, "upv:")) != -1)
    switch (c) {
        
        case 'u':
            startflag = 1;
            break;
        
        case 'p':
            pauseflag = 1;
            break;
        
        case 'v':
            volflag = 1;
            volume = (int) strtol(optarg, NULL, 10);
            break;

        case '?':
            printf("USAGE:\n -u -> upstart\n -p -> pause\n -v [volume number from 0 to 100] -> set volume\n");
            err = 1;
            break;
    }

/*
    printf("pauseflag = %d\n", pauseflag);
    printf("startflag = %d\n", startflag);
    printf("volume = %d\n", volume);
*/
    
    

    

    printf("**** PlainDAC CONTROL PROGRAM ****\n");

    
    char *fileName = "/dev/i2c-1";
    int file;
    int  address = 0x4d;
    char buf[10];
    
    if ((file = open(fileName, O_RDWR)) < 0) {					// Open port for reading and writing
        printf("Failed to open i2c port\n");
        exit(1);
    }
    
    if (ioctl(file, I2C_SLAVE, address) < 0) {					// Set the port options and set the address
        printf("Unable to get bus access to talk to slave\n");  // of the device we wish to speak to
        exit(1);                                                // I2C_SLAVE_FORCE if device is already used by kernel driver
    }
    
 
    if (startflag) {
        transmit_registers(initSeq, sizeof(initSeq)/sizeof(initSeq[0]),file);
        printf("UPSTART SENT\n");
    }
    else if (pauseflag) {
        transmit_registers(stopSeq, sizeof(stopSeq)/sizeof(stopSeq[0]),file);
        printf("STOP SENT\n");
    }
    
    if (volflag){
        setVolume(volume,file);
    }
    
    
    getVolume(file);
    

    exit(EXIT_SUCCESS);
}
