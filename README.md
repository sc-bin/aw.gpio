aw.gpio
======
这是一个python库，功能是在全志系列芯片上控制gpio。基于寄存器控制的方式，因为操作/sys/class的方式会导致不能操作那些被驱动使用了的引脚。

已实现功能
------
- 读写gpio
- 输出软件pwm

支持芯片列表
------
- H616



example
======
gpio
------
控制PC13引脚输出高低电平 ↓
```
from aw.gpio.H616 import *
import time
PC13.init(PC13.OUT)
while True:
    PC13.value(1)
    time.sleep(1)
    PC13.value(0)
    time.sleep(1)
```
读取PC12引脚电平，并控制PC13引脚电平 ↓
```
from aw.gpio.H616 import *
import time
key = PC12
led = PC13
key.init(key.IN,key.PULL_UP)
led.init(led.OUT)
while True:
    time.sleep(0.1)
    if key.value():
        led.value(1)
    else:
        led.value(0)
```
pwm
------
输出一段1000hz，占空比变化的pwm。为了方便接入circuitPython，占空比设置范围为0-65535
```
from aw.gpio import * 
import time

led = Pwm(PC13)
led.frequency = 1000
while 1:
    for i in range(1,65535,500):
        led.duty_cycle = i
        time.sleep(0.01)
```

安装到开发板
------
1. 安装用于编译cpython的库
```
sudo apt install python-dev-is-python3
```
2. 运行setup.py安装
```
python setup.py install
```

