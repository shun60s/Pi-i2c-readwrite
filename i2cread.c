/***************************************************************************************/
/* I2C multibyte data read, C source code, for Raspberry Pi                            */
/*                                                                                     */
/* This is based on i2cread.c of which author is penkob. (2013 year)                   */
/*                                                                                     */
/* Check condition:                                                                    */
/*  Hardware: Raspberry Pi 1 Model B  (2012 year)                                      */
/*  OS: Linux raspberrypi 3.10.25                                                      */
/*  CC: gcc version 4.6.3 (Debian 4.6.3-14+rpi1)                                       */
/*  I2C development environment should be installed.                                   */
/* Build:                                                                              */
/*  gcc -o i2cread i2cread.c                                                           */
/* Usage:                                                                              */
/*  ./i2cread <i2c-bus> <chip-address> <register-sub-address> <number>                 */
/*  <number> is byte number of multibyte to read. Maximum number is define MAX_NUM.    */
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
        unsigned char dat[MAX_NUM];
        char i2c_dev_fn[64];

        if (argc != 5)
        {
                fprintf(stderr, "sudo ./i2cread <i2c-bus> <chip-address> <register-sub-address> <number>\n");
                fprintf(stderr, "example:  sudo ./i2cread 1 0x1a 0xd9 4 \n");
                return 1;
        }

        bus =  (unsigned char)strtol(argv[1], NULL, 0);
        slave = (unsigned char)strtol(argv[2], NULL, 0);
        reg = (unsigned char)strtol(argv[3], NULL, 0);
        num=  (unsigned char)strtol(argv[4], NULL, 0);
        if(num > MAX_NUM) num=MAX_NUM;
        
        sprintf(i2c_dev_fn, "/dev/i2c-%d", bus);
        if ((fd = open(i2c_dev_fn, O_RDWR)) < 0) {
                printf("Faild to open i2c port\n");
                return 1;
        }

        if (ioctl(fd, I2C_SLAVE, slave) < 0) {
                printf("Unable to get bus access to talk to slave\n");
                return 1;
        }

        /* write address */
        if ((write(fd, &reg, 1)) != 1) {
                fprintf(stderr, "Error reading to i2c slave\n");
                return 1;
        }

        /* read data */
        if (read(fd, dat, num) != num) {
                fprintf(stderr, "Error reading from i2c slave\n");
                return 1;
        }

        close(fd);

        /* show read data */
        for(i=0;i<num;++i) {
                printf("%02x\n", dat[i]);
        }
 }