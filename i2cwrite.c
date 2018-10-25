/***************************************************************************************/
/* I2C multibyte data write, C source code, for Raspberry Pi                           */
/*                                                                                     */
/* This is based on i2cwrite.c of which author is penkob. (2013 year)                  */
/*                                                                                     */
/* Check condition:                                                                    */
/*  Hardware: Raspberry Pi 1 Model B  (2012 year)                                      */
/*  OS: Linux raspberrypi 3.10.25                                                      */
/*  CC: gcc version 4.6.3 (Debian 4.6.3-14+rpi1)                                       */
/*  I2C development environment should be installed.                                   */
/* Build:                                                                              */
/*  gcc -o i2cwrite i2cwrite.c                                                         */
/* Usage:                                                                              */
/*  sudo ./i2cwrite <i2c-bus> <chip-address> <register-sub-address> <data> <data> ...  */
/*  <data> <data> ... are multibyte data to write. Maximum length is define MAX_NUM.   */
/***************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define MAX_NUM 100

int main(int argc, char **argv)
{
        int fd,i;
        unsigned char bus, slave, reg, num;
        unsigned char dat[MAX_NUM+1];
        char i2c_dev_fn[64];

        if (argc < 5  ||  argc > (MAX_NUM+4) )
        {
                fprintf(stderr, "sudo ./i2cwrite <i2c-bus> <chip-address> <register-sub-address> <data> <data> <data> <data>....\n");
                fprintf(stderr, "example:  sudo ./i2cwrite 1 0x1a 0xd9 0x00 0x00 0x00 0x48 \n");
                return 1;
        }

        bus = (unsigned char)strtol(argv[1], NULL, 0);
        slave =(unsigned char)strtol(argv[2], NULL, 0);
        reg = (unsigned char)strtol(argv[3], NULL, 0);
        num=  argc - 4; /*  number of <data> */

        dat[0]= reg;
        for(i=0;i<num;++i){
              dat[i+1]= (unsigned char)strtol(argv[i+4], NULL, 0);
        }

        sprintf(i2c_dev_fn, "/dev/i2c-%d", bus);
        if ((fd = open(i2c_dev_fn, O_RDWR)) < 0) {
                printf("Faild to open i2c port\n");
                return 1;
        }

        if (ioctl(fd, I2C_SLAVE, slave) < 0) {
                printf("Unable to get bus access to talk to slave\n");
                return 1;
        }

        num=num+1;  /* reg-sub-address + datas */
        /* write reg-sub-address and datas */
        if (write(fd, dat, num) != num) {
                fprintf(stderr, "Error writing into i2c slave\n");
                return 1;
        }

       close(fd);
 }
