(module
  (import "env" "buffer" (memory 1))
  (import "env" "printString" (func $PrintString (param i32 i32)))

  (data (i32.const 128) "0123456789ABCDEF")
  (data (i32.const 256) "               0")
  (data (i32.const 384) "             0x0")
  (data (i32.const 512) " 0000 0000 0000 0000 0000 0000 0000 0000")

  (global $decStringLength i32 (i32.const 16))
  (global $hexStringLength i32 (i32.const 16))
  (global $binStringLength i32 (i32.const 40))

  (func (export "toString10") (param $num i32)
    (call $ToString10 (local.get $num) (global.get $decStringLength))
    (call $PrintString (i32.const 256) (global.get $decStringLength))
  )

  (func (export "toString16") (param $num i32)
    (call $ToString16 (local.get $num) (global.get $hexStringLength))
    (call $PrintString (i32.const 384) (global.get $hexStringLength))
  )

  (func (export "toString2") (param $num i32)
    (call $ToString2 (local.get $num) (global.get $binStringLength))
    (call $PrintString (i32.const 512) (global.get $binStringLength))
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

  (func $ToString16 (param $num i32) (param $digit i32)
    (local $index i32)
    (local $digitChar i32)
    (local $digitValue i32)
    (local $xPos i32)

    global.get $hexStringLength
    local.set $index

    i32.const 0
    local.set $xPos

    (loop $digitLoop
      (block $break
        local.get $index
        i32.eqz
        br_if $break  ;; End loop

        local.get $num
        i32.const 0x0000000f  ;; 0b0000000_000000_00000000_00001111
        i32.and  ;; Get nibble
        local.set $digitValue

        local.get $num
        i32.eqz
        if
          local.get $xPos
          i32.eqz
          if
            local.get $index
            local.set $xPos
          else
            i32.const 32
            local.set $digitChar
          end
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
        local.tee $index
        i32.const 384
        i32.add  ;; address = 384 + $index
        local.get $digitChar
        i32.store8

        ;; 4 bits (nibble) right shift
        local.get $num
        i32.const 4
        i32.shr_u
        local.set $num

        br $digitLoop
      )
    )

    local.get $xPos
    i32.const 1
    i32.sub
    i32.const 384
    i32.add  ;; address = 384 + ($xPos - 1)
    i32.const 120  ;; ASCII 'x'
    i32.store8

    local.get $xPos
    i32.const 2  ;; '0', 'x'
    i32.sub
    i32.const 384
    i32.add  ;; address = 384 + ($xPos - 2)
    i32.const 48  ;; ASCII '0'
    i32.store8
  )

  (func $ToString2 (param $num i32) (param $digit i32)
    (local $index i32)
    (local $nibbleBits i32)
    (local $numberOfNibbleLoop i32)

    global.get $binStringLength
    local.set $index

    i32.const 8
    local.set $numberOfNibbleLoop

    (loop $digitLoop
      (block $digitBreak
        local.get $index
        i32.eqz
        br_if $digitBreak  ;; End loop

        i32.const 4
        local.set $nibbleBits

        (loop $nibbleLoop
          (block $nibbleBreak
            local.get $index
            i32.const 1
            i32.sub
            local.set $index

            local.get $num
            i32.const 1
            i32.and
            if
              i32.const 512
              local.get $index
              i32.add  ;; address = 512 + $index
              i32.const 49  ;; ASCII '1'
              i32.store8
            else
              i32.const 512
              local.get $index
              i32.add  ;; address = 512 + $index
              i32.const 48  ;; ASCII '0'
              i32.store8
            end

            ;; Right shift
            local.get $num
            i32.const 1
            i32.shr_u
            local.set $num

            ;; End nibble loop ?
            local.get $nibbleBits
            i32.const 1
            i32.sub
            local.tee $nibbleBits
            i32.eqz
            br_if $nibbleBreak

            br $nibbleLoop
          )
        )

        local.get $index
        i32.const 1
        i32.sub
        local.tee $index
        i32.const 512
        i32.add  ;; address = 512 + $index
        i32.const 32  ;; ASCII ' '
        i32.store8

        local.get $numberOfNibbleLoop
        i32.const 1
        i32.sub
        i32.eqz
        br_if $digitBreak

        br $digitLoop
      )
    )
  )
)
