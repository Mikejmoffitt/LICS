.section .rodata

    .align  2
    .global eef_flat
eef_flat:
    dc.w    0xFE00, 0xFF00

    .align  2
    .global eef_hat
eef_hat:
    dc.w    0xFE07, 0x0A0C, 0x0D0E, 0x0FFF

