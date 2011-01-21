CROSS_COMPILE ?= arm-elf-
# CFLAGS = -std=gnu99 -mcpu=arm926ej-s -mlittle-endian -msoft-float -ffreestanding -Os -Wall -pedantic -D_LF1000_BOOTLOADER
CFLAGS = -std=gnu99 -mcpu=arm926ej-s -mlittle-endian -msoft-float -Os -Wall -pedantic -D_LF1000_BOOTLOADER
LDFLAGS = -static --architecture armv5te -EL -M -Map lightning-boot.map -e StartUp -T lightning.lds 

#-std=gnu99

CFLAGS += -Iinclude/arch-lf1000

# release version
CFLAGS += -DLB_VERSION=\"1.6p1\"

# Self-bootstrap
CFLAGS += -DSELF_BOOTSTRAP

# Nand release (default: uart)
CFLAGS += -DNAND_RELEASE
CFLAGS += -DMMC_DEBUG
# debug printing support
CFLAGS += -DDEBUG

# debug stopwatch support
CFLAGS += -DDEBUG_STOPWATCH

# u-boot bootloader support
CFLAGS += -DUBOOT_SUPPORT

OBJS = startup.o bootstrap.o nand.o jffs2.o crc32.o string.o gpio.o debug.o \
       timer.o tfs.o adc.o xmodem.o fbuffer.o font.o SDcard.o diskio.o pff.o

# Boot Splash Screen support
CFLAGS += -DDISPLAY_SUPPORT -DDISPLAY_BACKLIGHT_PWM
OBJS += display.o dpc.o mlc.o pwm.o

# Configure CPU and bus clocks
CFLAGS += -DCONFIGURE_CLOCKS

# Perform CRC32 checks on kernel image
#CFLAGS += -DCHECK_KERNEL_CRC

#PLATFORM_LIBS = -L/opt/toolchain_didj/toolchain/arm-opendidj-linux-uclibcgnueabi/lib/gcc/arm-opendidj-linux-uclibcgnueabi/4.1.2 -lgcc -lgcc_eh
PLATFORM_LIBS = -L/media/Storage/toolchain_didj/toolchain/arm-opendidj-linux-uclibcgnueabi/lib/gcc/arm-opendidj-linux-uclibcgnueabi/4.1.2 -lgcc -lgcc_eh
# CFLAGS += -save-temps

LINUX_FILES=include/autoconf.h include/mach-types.h include/dpc_hal.h \
	    include/mlc_hal.h include/pwm_hal.h

all: $(OBJS)
	$(CROSS_COMPILE)gcc $(CFLAGS) -c main.c
	$(CROSS_COMPILE)ld $(LDFLAGS) $(OBJS) main.o $(PLATFORM_LIBS) -o lightning-boot
	$(CROSS_COMPILE)objcopy -S -I elf32-littlearm -O binary lightning-boot lightning-boot.bin
	# $(CROSS_COMPILE)objdump -D -b binary -EL -m armv5te lightning-boot.bin > lightning-boot.dis
	@echo
	@$(CROSS_COMPILE)size --target=binary lightning-boot.bin
	@echo
	@$(CROSS_COMPILE)objcopy --gap-fill 0xff --pad-to 0x4000 -S -I elf32-littlearm -O binary lightning-boot lightning-boot.bin

startup.o: startup.S
	$(CROSS_COMPILE)gcc $(CFLAGS) -Wa,--defsym,StartUp=0x0000 -c startup.S

%.o:
	$(CROSS_COMPILE)gcc $(CFLAGS) -c $*.c

clean:
	rm -f *.o *.bin *.dis *.map lightning-boot
	
program:
	@echo Switch the DIDJ now on , hold the button until the backlight starts...
	#@sleep 1
	@echo 3
	#@sleep 1
	@echo 2
	#@sleep 1
	@echo 1
	#@sleep 1
	@echo 0
	@mode com2 BAUD=19200 PARITY=N DATA=8 STOP=1
	@echo Sending lightning-boot to didj ...
	@copy /b lightning-boot.bin com2
	@echo ..finish!
 
