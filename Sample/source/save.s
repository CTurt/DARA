.global _start

_start:
	.incbin "header.bin"
	.word	0x200228cf		@ Return address

code:
	.incbin "arm9.bin", 1

footer:
	.space (_start + 0x000002c4) - .
	.incbin "footer.bin"
