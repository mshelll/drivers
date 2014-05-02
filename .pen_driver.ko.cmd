cmd_/driver/pen_driver.ko := ld -r -m elf_i386 -T /usr/src/linux-headers-3.5.0-17-generic/scripts/module-common.lds --build-id  -o /driver/pen_driver.ko /driver/pen_driver.o /driver/pen_driver.mod.o
