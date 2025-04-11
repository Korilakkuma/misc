.LBB0_1:
    ldaxr w8, [x0]     ; w8 = [x0]
    add   w8, w8, #1   ; w8 = w8 + 1
    stlxr w9, w8, [x0] ; [x0] = w8
    cbnz  w9, .LBB0_1  ; if w9 != 0 then goto .LBB0_1
