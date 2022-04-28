(module
  (import "env" "memory" (memory 1))

  (global $digitPointer i32 (i32.const 128))
  (global $decStringPointer i32 (i32.const 256))
  (global $decStringLength i32 (i32.const 16))
  (global $hexStringPointer i32 (i32.const 384))
  (global $hexStringLength i32 (i32.const 16))
  (global $binStringPointer i32 (i32.const 512))
  (global $binStringLength i32 (i32.const 40))

  (data (i32.const 128) "0123456789ABCDEF")
  (data (i32.const 256) "               0")
  (data (i32.const 384) "             0x0")
  (data (i32.const 512) " 0000 0000 0000 0000 0000 0000 0000 0000")

  (global $listOpenPointer i32 (i32.const 640))
  (global $listOpenLength i32 (i32.const 4))

  (global $listClosePointer i32 (i32.const 656))
  (global $listCloseLength i32 (i32.const 5))

  (data (i32.const 640) "<li>")
  (data (i32.const 656) "</li>")

  (global $outStringPointer i32 (i32.const 1024))
  (global $outStringLength (mut i32) (i32.const 0))

  (func (export "toStringAsList") (param $num i32) (result i32)
    local.get $num
    global.get $decStringLength
    call $ToString10

    local.get $num
    global.get $hexStringLength
    call $ToString16

    local.get $num
    global.get $binStringLength
    call $ToString2

    i32.const 0
    global.set $outStringLength

    global.get $listOpenPointer
    global.get $listOpenLength
    call $Append

    global.get $decStringPointer
    global.get $decStringLength
    call $Append

    global.get $listClosePointer
    global.get $listCloseLength
    call $Append

    global.get $listOpenPointer
    global.get $listOpenLength
    call $Append

    global.get $hexStringPointer
    global.get $hexStringLength
    call $Append

    global.get $listClosePointer
    global.get $listCloseLength
    call $Append

    global.get $listOpenPointer
    global.get $listOpenLength
    call $Append

    global.get $binStringPointer
    global.get $binStringLength
    call $Append

    global.get $listClosePointer
    global.get $listCloseLength
    call $Append

    global.get $outStringLength
  )

  (func $Append (param $source i32) (param $length i32)
    local.get $source
    global.get $outStringPointer
    global.get $outStringLength
    i32.add
    local.get $length
    call $CopyString

    global.get $outStringLength
    local.get $length
    i32.add
    global.set $outStringLength
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

  (func $CopyByte (param $source i32) (param $dest i32) (param $length i32)
    (local $lastSourceByte i32)

    local.get $source
    local.get $length
    i32.add

    local.set $lastSourceByte

    (loop $copyLoop
      (block $break
        local.get $dest
        local.get $source
        i32.load8_u
        i32.store8

        local.get $dest
        i32.const 1  ;; 1 byte
        i32.add
        local.set $dest

        local.get $source
        i32.const 1  ;; 1 byte
        i32.add
        local.tee $source

        local.get $lastSourceByte
        i32.eq
        br_if $break

        br $copyLoop
      )
    )
  )

  (func $CopyByteEach64bits (param $source i32) (param $dest i32) (param $length i32)
    (local $lastSourceByte i32)

    local.get $source
    local.get $length
    i32.add

    local.set $lastSourceByte

    (loop $copyLoop
      (block $break
        local.get $dest
        local.get $source
        i64.load
        i64.store

        local.get $dest
        i32.const 8  ;; 8 bytes = 64 bits
        i32.add
        local.set $dest

        local.get $source
        i32.const 8 ;; 8 bytes = 64 bits
        i32.add
        local.tee $source

        local.get $lastSourceByte
        i32.ge_u
        br_if $break

        br $copyLoop
      )
    )
  )

  (func $CopyString (param $source i32) (param $dest i32) (param $length i32)
    (local $startSourceByte i32)
    (local $startDestByte i32)
    (local $singels i32)
    (local $lengthLessSingles i32)

    local.get $length
    local.set $lengthLessSingles

    local.get $length
    i32.const 7  ;; 0b00000000_00000000_00000000_00000111
    i32.and
    local.tee $singels  ;; mod 8

    if
      local.get $length
      local.get $singels
      i32.sub
      local.tee $lengthLessSingles

      local.get $source
      i32.add
      local.set $startSourceByte

      local.get $lengthLessSingles
      local.get $dest
      i32.add
      local.set $startDestByte

      (call $CopyByte (local.get $startSourceByte) (local.get $startDestByte) (local.get $singels))
    end

    local.get $length
    i32.const 0xfffffff8  ;; 0b11111111_11111111_11111111_11111000
    i32.and  ;; mod 8 === 0
    local.set $length

    (call $CopyByteEach64bits (local.get $source) (local.get $dest) (local.get $length))
  )
)
