# C-read-write-WAV-file  
C/C++读写WAV文件  
改编自https://github.com/lyc18/C-CPP-read-write-WAV  
支持了头部不是标准44字节的WAV文件，以及IEEE float格式存储的数据（音频格式代码audioFormat==03）  
但是在写WAV文件的时候丢掉了非必要部分，如专辑等信息，代码可能还有bug，如果有人发现bug请提issue，并且最好提供测试样本。  
