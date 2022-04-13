(module
  (import "env" "buffer" (memory 1))
  (import "env" "strPosLen" (func $StrPosLen (param i32 i32)))
  (import "env" "strNull" (func $StrNull (param i32)))
  (import "env" "strPrefix" (func $StrPrefix (param i32)))

  (data (i32.const 256) "Know the length of this string")  ;; 30 chars
  (data (i32.const 384) "Also know the length of this string")  ;; 35 chars
  (data (i32.const 0) "null-terminating string\00")
  (data (i32.const 128) "another null-terminating string\00")
  (data (i32.const 512) "\16length-prefired string")  ;; 22 chars (0x16)
  (data (i32.const 640) "\1eanother length-prefired string")  ;; 30 chars (0x1e)

  (func (export "main")
    (call $StrPosLen (i32.const 256) (i32.const 30))
    (call $StrPosLen (i32.const 384) (i32.const 35))
    (call $StrNull (i32.const 0))
    (call $StrNull (i32.const 128))
    (call $StrPrefix (i32.const 512))
    (call $StrPrefix (i32.const 640))
    ;; (call $StrPosLen (i32.const 256) (i32.const 30))
    ;; (call $StrPosLen (i32.const 384) (i32.const 35))
    ;; (call $CopyString (i32.const 256) (i32.const 384) (i32.const 30))
    ;; (call $StrPosLen (i32.const 384) (i32.const 35))
    ;; (call $StrPosLen (i32.const 384) (i32.const 30))
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
