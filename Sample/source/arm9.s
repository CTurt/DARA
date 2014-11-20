.global _start

_start:
    mov r0,#0x04000000      @ I/O space offset
    mov r1,#0x3             @ Both screens on
    mov r2,#0x00020000      @ Framebuffer mode
    mov r3,#0x80            @ VRAM bank A enabled, LCD
 
    str r1,[r0, #0x304]     @ Set POWERCNT
    str r2,[r0]             @     DISPCNT 
    str r3,[r0, #0x240]     @     VRAMCNT_A
 
    mov r0,#0x06800000      @ VRAM offset
    mov r1,#31              @ Writing red pixels
    mov r2,#0xC000          @ 96k of them
 
lp: strh r1,[r0],#2         @ Write a pixel
    subs r2,r2,#1           @ Move along one
    bne lp                  @ And loop back if not done

nf: b nf                    @ Sit in an infinite loop to finish
