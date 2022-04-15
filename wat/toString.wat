(module
  (import "env" "buffer" (memory 1))
  (import "env" "printString" (func $PrintString (param i32 i32)))

  (data (i32.const 128) "0123456789ABCDEF")
  (data (i32.const 256) "               0")

  (global $decStringLength i32 (i32.const 16))

  (func (export "toString10") (param $num i32)
    (call $ToString10 (local.get $num) (global.get $decStringLength))
    (call $PrintString (i32.const 256) (global.get $decStringLength))
  )

  (func $ToString10 (param $num i32) (param $digit i32)
    (local $index i32)
    (local $digitChar i32)
    (local $digitValue i32)

    local.get $digit
    local.set $index

    local.get $num
    i32.eqz
    if
      local.get $index
      i32.const 1
      i32.sub
      local.set $index

      i32.const 256
      local.get $index
      i32.add  ;; address = 256 + $index
      i32.const 48  ;; ASCII '0'
      i32.store8
    end

    (loop $digitLoop
      (block $break
        local.get $index
        i32.eqz
        br_if $break  ;; End loop

        ;; Get the least significant digit
        local.get $num
        i32.const 10
        i32.rem_u
        local.set $digitValue

        local.get $num
        i32.eqz
        if
          i32.const 32  ;; ASCII ' '
          local.set $digitChar
        else
          i32.const 128
          local.get $digitValue
          i32.add  ;; address = 128 + $digitValue
          i32.load8_u
          local.set $digitChar
        end

        ;; Next digit
        local.get $index
        i32.const 1
        i32.sub
        local.set $index

        i32.const 256
        local.get $index
        i32.add  ;; address = 256 + $index
        local.get $digitChar
        i32.store8

        ;; Right shift
        local.get $num
        i32.const 10
        i32.div_u
        local.set $num

        br $digitLoop
      )
    )
  )
)
