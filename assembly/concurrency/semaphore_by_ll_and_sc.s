.LBB0_1:
  ldr   w8, [x0]
  cmp   w8, #3        ; `3` is semaphore variable
  b.hi  .LBB0_1       ; while (w8 > 3)
.Ltmp_1:
  ldaxr w2, [x0]      ; w2 = [x0]
  cmp   w2, #4
  b.lo  .Ltmp_2       ; if (w2 < 4) then goto .Ltmp_2
  clrex               ; clear exclusive
  b     .LBB0_1       ; goto .LBB0_1
.Ltmp_2:
  add   w2, w2, #1    ; w2 = w2 + 1
  stlxr w3, w2, [x0]  ; [x0] = w2
  cbnz  w3, .Ltmp_1   ; if (*w3 != 0) then goto .Ltmp_1
  ret
