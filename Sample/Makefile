include $(DEVKITARM)/ds_rules

OBJDUMP	:=	$(PREFIX)objdump

#arm9-disassembled.s: arm9.bin
#	$(OBJDUMP) -D -b binary -m arm arm9.bin > arm9-disassembled.s

source/save.sav: arm9.bin
	$(CC) -nostartfiles -nostdlib source/save.s -o save.elf -Ttext=0x200228cf
	$(OBJCOPY) -O binary save.elf save.sav
	../CRCFix/CRCFix save.sav -f
	@rm -f save.elf
	@rm -f arm9.bin

arm9.bin: source/arm9.s
	$(CC) -nostartfiles -nostdlib source/arm9.s -o arm9.elf -Ttext=0x200228cf
	$(OBJCOPY) -O binary arm9.elf arm9.bin
	@rm -f arm9.elf