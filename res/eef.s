.section .rodata

    .align  2
    .global eef_flat
eef_flat:
    dc.w    0xFE00, 0xFF00

    .align  2
    .global eef_hat
eef_hat:
    dc.w    0x0004, 0x0709, 0x0A0B, 0xFFFF

    .align  2
    .global eef_qdecay
eef_qdecay:
    dc.w    0x0002, 0x0407, 0x0A0C, 0x0DFF

