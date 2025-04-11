    mov w8, #1           ; w8 = 1
.LBB0_1:
    ldaxrb w9, [x0]      ; w9 = [X0]
    stlxrb w10, w8, [x0] ; [x0] = x8
    cbnz   w10, .LBB0_1  ; if w10 != 0 then goto .LBB0_1
    and    w0, w9, #1    ; w0 = w9 & 1
    ret
