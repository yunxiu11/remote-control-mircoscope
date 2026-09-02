########################################################################
# LIBRARY SOURCES - MUST BE IN THE SAME FOLDER as main.c (DO NOT CHANGE)
########################################################################
FREERTOS_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS
# Set mylib folder path.
# Do not change the MYLIB_PATH variable name.
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
# e.g. MYLIBPATH=$(SOURCELIB_ROOT)/../mylib
MYLIB_PATH=$(SOURCELIB_ROOT)/../repo/mylib
NRF24L01PLUS_PATH=$(SOURCELIB_ROOT)/components/peripherals/nrf24l01plus/nucleo-f429
CLI_PATH = $(SOURCELIB_ROOT)/components/os/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI

# Set folder path with header files to include.
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
CFLAGS += -I$(MYLIB_PATH)
CFLAGS += -I$(FREERTOS_PATH)
CFLAGS += -I$(NRF24L01PLUS_PATH)
CFLAGS += -I$(CLI_PATH)

# List all c file locations that must be included (use space as separator
# e.g. LIBSRCS += path_to/file1.c path_to/file2.c)
# ONLY use relative file paths that start with $(SOURCELIB_ROOT)../
# DO NOT USE absolute file paths (e.g. /home/users/myuser/mydir)
LIBSRCS += $(FREERTOS_PATH)/portable/MemMang/heap_4.c
LIBSRCS += $(MYLIB_PATH)/s4831567_lta1000g.c
LIBSRCS += $(MYLIB_PATH)/s4831567_mfs_pb.c
LIBSRCS += $(MYLIB_PATH)/s4831567_rgb.c
LIBSRCS += $(MYLIB_PATH)/s4831567_mfs_trimpot.c
LIBSRCS += $(MYLIB_PATH)/s4831567_hamming.c
LIBSRCS += $(MYLIB_PATH)/s4831567_mfs_led.c
LIBSRCS += $(MYLIB_PATH)/s4831567_switchbank.c
LIBSRCS += $(MYLIB_PATH)/s4831567_rcmsys.c
LIBSRCS += $(MYLIB_PATH)/s4831567_txradio.c
LIBSRCS += $(MYLIB_PATH)/s4831567_rcmext.c
LIBSRCS += $(MYLIB_PATH)/s4831567_seven_segment.c
LIBSRCS += $(MYLIB_PATH)/s4831567_cli.c
LIBSRCS += $(wildcard $(NRF24L01PLUS_PATH)/*.c)
LIBSRCS += $(wildcard $(CLI_PATH)/*.c)



