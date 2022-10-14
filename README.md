# C micro controller- STC89C52RC

## 单片机内部三大资源

1. FLASH：程序存储空间，ROM
2. RAM：内存
3. SFR：特殊功能寄存器，Special function register

## 单片机最小系统（三要素）

1. 电源电路
2. 复位电路
3. 晶振电路

## 51 单片机

兼容 Intel 的 MCS-51 体系架构的一系列单片机。

### STC89C52RC

- 8K FLASH
- 512B RAM
- 32 个 IO 口
- 3 个定时器
- 1 个 UART
- 8 个中断源

一个 byte 是 8 bit，一个bit控制一个 IO 口，byte 控制所有的 IO 口。

## 构建

1. 下载 PlatformIO toolchain 后配置 sdcc 地址:

```
$ which sdcc
~/.platformio/packages/toolchain-sdcc/bin/sdcc
```

2. 编译

```bash
sdcc main.c
```

3. 确认串口地址

```bash
ls /dev/tty.wchusbser*
```

4. 烧录到单片机

```bash
stcgal -P stc89 -p /dev/tty.wchusbserial14220 main.ihx
```