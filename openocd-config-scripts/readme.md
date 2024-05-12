# Scripts

Enclosed is OpenOCD scripts for Intel/Terasic DE10-Nano development board:

| File                              | Description                                         |
| --------------------------------- | --------------------------------------------------- |
| target/altera_fpgasoc_de.cfg      | Target setup TCL script                             |
| interface/altera-usb-blaster2.cfg | JTAG adapter interface setup TCL script             |

Copy the altera_fpgasoc_de.cfg to your OpenOCD target scripts folder, for example:
C:\devtools\xpack-openocd-0.12.0-2\share\openocd\scripts\target

Copy the altera-usb-blaster2.cfg to your OpenOCD interface scripts folder, for example:
C:\devtools\xpack-openocd-0.12.0-2\share\openocd\scripts\interface

# U-Boot SPL (preloader)

You will need to run a preloader first, using OpenOCD or GDB, to configure and initialise the DE10-Nano board, otherwise you cannot do much with the HPS.

Example files:

| File                             | Description                                                       |
| -------------------------------- | ----------------------------------------------------------------- |
| source/bsp/u-boot-spl-nocache    | Precompiled U-Boot SPL (elf) with embedded DTB and cache disabled |
| source/Debug/freertos_blinky.elf | Precompiled elf standalone application                            |

# Quick start notes

Below assumes GNU toolchain and OpenOCD paths are already setup.

## Start OpenOCD server

To start OpenOCD with USB-Blaster II JTAG adapter and run the SPL preloader, start a command prompt and enter:
```
openocd -f interface/altera-usb-blaster2.cfg -f target/altera_fpgasoc_de.cfg -c "init; halt; c5_reset; halt; load_image source/bsp/u-boot-spl-nocache; resume 0xffff0000; sleep 200; halt; arm core_state arm"
```

## Get entry point of the elf application

arm-none-eabi-readelf -h source/Debug/freertos_blinky.elf

Example output:
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              EXEC (Executable file)
  Machine:                           ARM
  Version:                           0x1
  Entry point address:               0x2250
  Start of program headers:          52 (bytes into file)
  Start of section headers:          1018204 (bytes into file)
  Flags:                             0x5000400, Version5 EABI, hard-float ABI
  Size of this header:               52 (bytes)
  Size of program headers:           32 (bytes)
  Number of program headers:         3
  Size of section headers:           40 (bytes)
  Number of section headers:         33
  Section header string table index: 32

## Connect with telnet

In another command prompt, enter:
```
telnet localhost 4444
```

Note: First, start a SSH terminal, e.g. PuTTY and connect to the DE10-Nano's UART USB port, to enable you view debug messages.
Load and run the elf application with the entry point address you found earlier:
```
load_image source/Debug/freertos_blinky.elf
resume 0x2250
```

### Some useful commands

Halt then print registers:
```
halt
reg
```

List targets and current state:
```
targets
```

## Connect with GDB

In another command prompt, enter:
```
arm-none-eabi-gdb -iex="target extended-remote localhost:3333" -ex="set pagination off" -ex="set confirm off"
```

Load and run the bare-metal elf, also sets a temporary breakpoint on _exit() so that we can get back to GDB:
```
file source/Debug/freertos_blinky.elf
load
thb _exit
continue
```
