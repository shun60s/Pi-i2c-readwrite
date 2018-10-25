# I2C multibyte data read/write for Raspberry Pi    

A primitive C source code to read/write multibyte data by I2C for Raspberry Pi 1.   

## content    

multibyte data read.   
```
sudo ./i2cread <i2c-bus> <chip-address> <register-sub-address> <number>  
```
number is mutibyte data length to read.  
  
multibyte data write.   
```
sudo ./i2cwrite <i2c-bus> <chip-address> <register-sub-address> <data> <data> <data> <data>... 
```
data ... is multibyte data to write.   
  
Maximum length to read/write is defined as MAX_NUM in the source. 

## Document  

See comments in the source.  

