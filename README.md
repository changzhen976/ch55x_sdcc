# CH55x SDK for SDCC ![Linux build](https://github.com/changzhen976/ch55x_sdcc/actions/workflows/linux.yml/badge.svg)

This is a port of the CH55x SDK, from Keil C51 to SDCC, with `CH552` / `CH554`
conditional compilation.

本仓库把 WCH 官方 CH55x SDK（原为 Keil C51 工程）移植到 SDCC 工具链,并通过
`CHIP` 宏对 **CH552 / CH554** 做条件编译。默认目标为 **CH552**,配合
**WeAct CH552 Core Board V10** 核心板使用。

CH55x 系列单片机的突出特点是:价格极低、内置 USB Device/Host 外设、并且出厂预置
USB bootloader——无需任何专用编程器,用一根 USB 线即可下载固件。

> The CH55x family is notable for its extremely low cost, built-in USB
> device/host peripherals, and a preloaded USB bootloader.

---

## 目录 / Table of Contents

- [目标硬件 / Target Hardware](#目标硬件--target-hardware)
- [工具链 / Toolchain](#工具链--toolchain)
- [构建示例 / Build the Examples](#构建示例--build-the-examples)
- [烧录 / Flashing](#烧录--flashing)
- [构建配置变量 / Build Configuration Variables](#构建配置变量--build-configuration-variables)
- [从 Keil C51 移植 / Port from Keil C51](#从-keil-c51-移植--port-from-keil-c51)
- [新建示例 / Create a New Example](#新建示例--create-a-new-example)
- [外设状态 / Peripheral Status](#外设状态--peripheral-status)
- [致谢 / Credits](#致谢--credits)
- [参考资料 / References](#参考资料--references)

---

## 目标硬件 / Target Hardware

本仓库按 **CH552** 配置默认值,实际使用目标板为 **WeAct CH552 Core Board V10**。

> Default build target is **CH552**, used with the **WeAct CH552 Core Board V10**.

### WeAct CH552 Core Board V10 板级硬件事实

| 项目 | 说明 |
| --- | --- |
| 板载 LED | **`P3.0`**,高电平点亮(电路:`P3.0 → R6(10K) → D1(蓝) → GND`) |
| BOOT/IAP 键 | **`P3.6`**(按住 BOOT + 单击 RST 进入 bootloader;P3.6 复用为 USB D+) |
| 时钟 | 内部振荡器,无外部晶振,最高 **24 MHz**(本仓库默认即 24 MHz) |
| 供电 / 下载 | USB 供电 + USB bootloader 下载 |

> ⚠️ 注意:仓库自带 `examples/blink` 闪烁的是 **P1.7**(`SBIT(LED, 0x90, 7)`),
> 并非板载 LED。要点亮 WeAct 板载灯,需改用 `SBIT(LED, 0xB0, 0)` 并把 P3.0 配为输出。

CH552 为 WCH（沁恒）出品的增强型 8051（E8051 内核）USB 单片机;CH554 与之引脚/寄存器
高度兼容,本仓库以 `ch55x.h` 统一头文件,通过 `CHIP_CH552` / `CHIP_CH554` 宏区分差异寄存器。

---

## 工具链 / Toolchain

### Linux (Debian 系)

```bash
sudo apt install build-essential sdcc
```

### 烧录工具 / Flash tools

下列开源工具均可把固件通过 USB bootloader 烧录进 CH55x:

| 工具 | 说明 |
| --- | --- |
| `wchisptool` | 默认 `make flash` 调用:`wchisptool -g -f` |
| [`chprog`](https://github.com/ole00/chprog) | Python 烧录器,可经 `WCHISP` 变量覆盖使用(本仓库 `docker/Dockerfile` 即用它) |
| [`LibreCH551`](https://github.com/rgwan/librech551) | 兼容 CH552/CH554,可替代官方 WCHISPTool |
| [`ch552tool`](https://github.com/MarsTechHAN/ch552tool) | 另一社区烧录器 |

### Windows

需要较新版本的 [SDCC](https://sourceforge.net/projects/sdcc/files/)、
[MinGW](https://downloads.sourceforge.net/project/mingw/Installer/mingw-get-setup.exe)
(提供 make)与 [git-bash](https://github.com/git-for-windows/git/releases);
烧录可使用官方 [WCHISPTool](http://wch.cn/download/WCHISPTool_Setup_exe.html),
亦可使用上表开源工具。

安装完成后,在 `.bashrc` 末尾追加(路径按实际安装位置调整):

```bash
# SDCC compiler tools
export PATH=$PATH:/c/Program\ Files/SDCC/bin
# Mingw tools (for Make)
export PATH=$PATH:/c/Qt/Qt5.10.0/Tools/mingw530_32/bin
alias make=mingw32-make.exe
```

### macOS

需要 Xcode(提供 make)与 SDCC;烧录工具与 Linux 相同。

---

## 构建示例 / Build the Examples

```bash
git clone https://github.com/changzhen976/ch55x_sdcc.git
cd ch55x_sdcc/examples
make
```

工具链配置正确的话,所有示例都会被构建,产物为 `*.bin` 与 `*.hex`。

默认编译参数(`examples/Makefile.include`):

| 变量 | 默认值 | 说明 |
| --- | --- | --- |
| `CHIP` | `CH552` | 芯片型号,传 `-DCHIP_CH552`;编 CH554 示例用 `make CHIP=CH554` |
| `FREQ_SYS` | `24000000` | 系统主频 24 MHz(CH552 内部振荡器上限),传 `-DFREQ_SYS=` |

> `ch55x.h` 会根据 `CHIP_CH552` / `CHIP_CH554` 宏做条件编译;库内延时/波特率计算
> (`debug.c` 等)读取 `FREQ_SYS`。

---

## 烧录 / Flashing

CH55x 出厂自带 USB bootloader,无需专用编程器:

1. 按住板载 **BOOT 键(P3.6)**,单击 **RST**,然后松开 BOOT —— CH552 进入 USB bootloader 模式。
2. 在工程目录执行:

   ```bash
   make flash
   # 或用 Python 烧录器 chprog:
   make flash WCHISP="python3 /path/to/chprog.py"
   ```

Windows 亦可直接用 WCHISPTool 把 `.hex` / `.bin` 烧入目标板。

---

## 构建配置变量 / Build Configuration Variables

构建参数在共享模板 `examples/Makefile.include` 中定义,可被各示例 Makefile 覆盖:

| Makefile 变量 | 说明 |
| --- | --- |
| `TARGET` | 示例名,用于命名输出 `.hex` / `.bin` |
| `C_FILES` | 参与编译的 `.c` 文件列表(建议显式列出,SDCC 链接器不做死代码裁剪) |
| `FREQ_SYS` | 系统时钟频率,默认 `24000000`(12 MHz 亦可,见 `include/debug.c`) |
| `CHIP` | 芯片型号,默认 `CH552` |
| `XRAM_SIZE` | 非保留 XRAM 大小,默认 `0x0400`(1 KB) |
| `XRAM_LOC` | 非保留 XRAM 起始地址,默认 `0x0000` |
| `CODE_SIZE` | 代码区上限,默认 `0x3800`(为 0x3800 处的 bootloader 预留空间) |
| `STDIO_UART` | `0` 用 UART0 做 STDIO,`1` 用 UART1(未完成) |

---

## 从 Keil C51 移植 / Port from Keil C51

两个编译器语法略有差异,SDK 移植中遇到的主要问题:

- `SFR` / `SBIT` 定义语法不同
- 绝对地址定位格式不同
- SDCC 不会自动跟踪绝对地址变量
- SDCC 可用标准类型如 `uint8_t`,而 C51 示例用了非标准定义如 `UINT8`
- SDCC 为小端序,C51 为大端序

仓库内置一个简易 Python 脚本,可自动完成部分语法转换:

```bash
python tools/c51_to_sdcc.py [source] [destination]
```

通常能自动完成约 90% 的移植工作。

---

## 新建示例 / Create a New Example

在 `examples/` 下新建以示例名命名的目录:

```bash
cd examples/
mkdir fastblink
cd fastblink
```

添加一个引用共享模板的 Makefile:

```makefile
TARGET = fastblink

C_FILES = \
	main.c \
	../../include/debug.c

include ../Makefile.include
```

再写一个最简 `main.c`(闪烁 P1.7):

```c
// Blink an LED connected to pin 1.7

#include <ch55x.h>
#include <debug.h>

#define LED_PIN 7
SBIT(LED, 0x90, LED_PIN);

void main() {
    // Configure pin 1.7 as GPIO push-pull output
    P1_DIR_PU &= 0x0C;
    P1_MOD_OC = P1_MOD_OC & ~(1 << LED_PIN);
    P1_DIR_PU = P1_DIR_PU | (1 << LED_PIN);

    while (1) {
        mDelaymS(10);
        LED = !LED;
    }
}
```

> ⚠️ 若忘记把 `debug.c` 加入 `C_FILES`,链接器会报
> `Undefined Global '_CfgFsys'` / `'_mDelaymS'`——务必显式列出所需库源文件。

然后编译:

```bash
make
```

---

## 外设状态 / Peripheral Status

下表按**当前仓库实际内容**(`include/` 驱动 + `examples/` 示例)核对,并经 `make`
全量构建验证(16 个示例通过;`usb_host_KM` 见注),而非上游历史快照。

> 图例:✅ complete ｜ ⚠️ partial(仅寄存器/部分实现)｜ ❌ not started ｜ ➖ n/a

| Peripheral | Driver / Example | Status |
| --- | --- | --- |
| ADC | `adc.c` + `examples/adc` | ✅ complete |
| GPIO | `examples/blink`、`blink_timer0` | ✅ complete |
| PWM | `pwm.h` + `examples/pwm`、`pwm_interrupt` | ✅ complete |
| UART0 / stdio | `debug.c`(UART0) | ✅ complete |
| UART1 / stdio | `debug.c`(UART1) | ✅ complete |
| Watchdog (WDT) | `debug.h`(`CH554WDTModeSelect/Feed`) | ✅ complete *(无独立示例)* |
| Timer 0/1 | `examples/blink_timer0` | ✅ complete |
| Timer 2 | `ch55x.h` 寄存器/位定义 | ⚠️ partial *(仅寄存器)* |
| SPI | `spi.c`(主/从)+ `examples/usb_spi_flash` | ✅ complete |
| I²C | `i2c.c` + `examples/oled_ssd1306` | ✅ complete |
| TouchKey | `touchkey.c` + `examples/touchkey` | ✅ complete |
| Chip ID | `ch55x.h`(`ROM_CHIP_ID_*`) | ⚠️ partial *(仅寄存器)* |
| IAP / Bootloader | `bootloader.h` + `examples/iap` | ✅ complete |
| USB Device CDC | `examples/usb_device_cdc`、`cdc_i2c`、`cdcx2` | ✅ complete |
| USB Device MIDI | `examples/usb_device_midi` | ✅ complete |
| USB Device HID | — | ❌ not started |
| USB Device 大容量存储 (U_DISK) | — | ❌ not started |
| USB Compound Device | — | ❌ not started |
| USB Host (HID / Hub) | `examples/usb_host_KM` | ⚠️ excluded *(见下注)* |
| USB Host 大容量存储 | — | ❌ not started *(CH554 only)* |
| Type-C (PD) | — | ➖ n/a on CH552 *(CH554 only)* |
| DataFlash / EEPROM | — | ❌ not started |

> ℹ️ CH552 仅含 USB **Device**;USB **Host** 与 Type-C PD 是 **CH554** 专属外设。
> `examples/usb_host_KM` 是 CH554-only 示例,且在 `--model-small` 下内部 RAM 放不下
> (链接期 DSEG 溢出),故已从顶层 `make` 构建中排除,仅留作参考(可单独
> `make -C usb_host_KM` 尝试)。另有应用类示例:`ws2812`、`lpd8806Strip`、`sleep` 等。

---

## 致谢 / Credits

本仓库并非从零开始,源自社区与官方的长期积累,在此一并致谢:

- **[Blinkinlabs/ch554_sdcc](https://github.com/Blinkinlabs/ch554_sdcc)** —— 本仓库的
  直接上游(原 fork 来源)。最初的 CH554 SDK 从 Keil C51 到 SDCC 的移植、示例工程、
  CI 构建以及英文手册翻译均出自该项目。本仓库在其基础上增加了 CH552/CH554 条件编译,
  并将默认目标调整为 CH552 / WeAct CH552 Core Board。
- **WCH（沁恒）** —— [官方 CH554 EVT / SDK](https://github.com/HonghongLu/CH554),
  所有外设驱动的最初来源。
- 感谢所有向上游贡献代码、翻译注释与提交 issue 的社区作者。

溯源链 / Provenance:

```
WCH 官方 CH554 EVT (Keil C51)
        ↓ 移植到 SDCC
Blinkinlabs/ch554_sdcc
        ↓ fork + CH552/CH554 条件编译
changzhen976/ch55x_sdcc  (本仓库)
```

---

## 参考资料 / References

- [CH551/CH552 数据手册 (WCH 官方)](http://www.wch-ic.com/downloads/file/309.html)
- [CH554 数据手册 (WCH 官方)](http://www.wch-ic.com/downloads/file/229.html)
- [CH554 manual (Chinese)](documentation/CH554%20manual%201E%20%28chinese%29.pdf)
- [CH554 manual (English translation)](documentation/CH554%20manual%20english.pdf)
- [SDCC Compiler User's Guide](http://sdcc.sourceforge.net/doc/sdccman.pdf)
- [CH554 instruction set timings](https://github.com/HonghongLu/CH554/blob/master/official%20ch554%20evt%20pcb/PUB/CH55X%C2%AA%E2%80%9E%C2%B1%E2%80%A1%C3%B7%E2%88%8F%C2%A1%C3%93.PDF)

> 想要更平易近人的 CH55x 入门体验,可以看看这个 Arduino 移植:
> [DeqingSun/ch55xduino](https://github.com/DeqingSun/ch55xduino)

---

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md).
