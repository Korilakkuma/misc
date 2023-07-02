(module
  (memory $memory 1 256)  ;; initial: 64 KiB, maximum: 256 * 64 KiB
  (global $map (mut i32) (i32.const 0))
  (global $tmpmap (mut i32) (i32.const 0))
  (global $width (mut i32) (i32.const 0))
  (global $height (mut i32) (i32.const 0))

  (func $init (param $w i32) (param $h i32)
    (local $size i32)
    (local $p i32)
    (local $fillp i32)

    local.get $w
    global.set $width
    local.get $h
    global.set $height

    local.get $w
    local.get $h
    i32.mul
    i32.const 4
    i32.mul
    local.set $size

    local.get $size
    i32.const 2
    i32.mul
    i32.const 0xffff
    i32.add
    i32.const 16
    i32.shr_u
    memory.size
    i32.sub
    memory.grow
    drop

    i32.const 0
    global.set $map

    local.get $size
    global.set $tmpmap

    global.get $map
    local.tee $p
    local.get $size
    i32.add
    local.set $fillp

    loop $fill_loop
      local.get $p
      i32.const 0xff1e0f0f
      i32.store

      local.get $p
      i32.const 4
      i32.add
      local.tee $p
      local.get $fillp
      i32.lt_u
      br_if $fill_loop
    end

    ;; S-expression
    ;; (global.set $width (local.get $w))
    ;; (global.set $height (local.get $h))

    ;; (local.set $size
    ;;   (i32.mul
    ;;     (i32.mul (local.get $w) (local.get $h))
    ;;     (i32.const 4)
    ;;   )
    ;; )

    ;; (i32.sub
    ;;   (i32.shr_u
    ;;     (i32.add
    ;;       (i32.mul (local.get $size) (i32.const 2))
    ;;       (i32.const 0xffff)
    ;;     )
    ;;     (i32.const 16)
    ;;   )
    ;;   (memory.size)
    ;; )

    ;; memory.grow
    ;; drop

    ;; (global.set $map (i32.const 0))
    ;; (global.set $tmpmap (local.get $size))

    ;; (local.set $fillp
    ;;   (i32.add
    ;;     (local.tee $p (global.get $map))
    ;;     (local.get $size)
    ;;   )
    ;; )

    ;; (loop $fill_loop
    ;;   (i32.store (local.get $p) (i32.const 0xff1e0f0f))

    ;;   (br_if $fill_loop
    ;;     (i32.lt_u
    ;;       (local.tee $p (i32.add (local.get $p) (i32.const 4)))
    ;;       (local.get $fillp)
    ;;     )
    ;;   )
    ;; )
  )

  (func $dot (param $x i32) (param $y i32) (param $b i32)
    local.get $y
    global.get $width
    i32.mul
    local.get $x
    i32.add
    i32.const 4
    i32.mul
    global.get $map
    i32.add
    i32.const 0xffc8ffc8
    i32.const 0xff1e0f0f
    local.get $b
    select
    i32.store

    ;; S-expression
    ;; (i32.store
    ;;   (i32.mul (local.get $y) (global.get $width))
    ;;   (i32.add (local.get $x))
    ;;   (i32.mul (i32.const 4))
    ;;   (i32.add (global.get $map))
    ;;   (select (i32.const 0xffc8ffc8) (i32.const 0xff1e0f0f) (local.get $b))
    ;; )
  )

  (func $getmap (result i32)
    global.get $map
  )

  (func $lifegame
    (local $dest i32)
    (local $sizeline i32)
    (local $src0 i32)
    (local $src1 i32)
    (local $src2 i32)
    (local $c0 i32)
    (local $c1 i32)
    (local $c2 i32)
    (local $offset_x i32)
    (local $y i32)
    (local $tmp i32)

    global.get $tmpmap
    local.set $dest

    global.get $width
    i32.const 4
    i32.mul
    local.set $sizeline

    global.get $map
    local.tee $src2
    global.get $height
    i32.const 1
    i32.sub
    local.get $sizeline
    i32.mul
    i32.add
    local.set $src1

    i32.const 0
    local.set $y

    loop $loop_y
      local.get $src1
      local.set $src0
      local.get $src2
      local.tee $src1
      local.get $sizeline
      i32.add
      local.set $src2

      local.get $y
      i32.const 1
      i32.add
      local.tee $y
      global.get $height
      i32.eq
      local.tee $tmp
      local.get $tmp

      if
        global.get $map
        local.set $src2
      end

      i32.const 0xffc8ffc8
      local.get $src0
      local.get $sizeline
      i32.const 4
      i32.sub
      local.tee $tmp
      i32.add
      i32.load
      i32.eq
      i32.const 0xffc8ffc8
      local.get $src1
      local.get $tmp
      i32.add
      i32.load
      i32.eq
      i32.const 0xffc8ffc8
      local.get $src2
      local.get $tmp
      i32.add
      i32.load
      i32.eq
      i32.add
      i32.add
      local.set $c0

      i32.const 0xffc8ffc8
      local.get $src0
      i32.load
      i32.eq
      i32.const 0xffc8ffc8
      local.get $src1
      i32.load
      i32.eq
      i32.const 0xffc8ffc8
      local.get $src2
      i32.load
      i32.eq
      i32.add
      i32.add
      local.set $c1

      i32.const 0
      local.set $offset_x

      loop $loop_x
        i32.const 0xffc8ffc8
        local.get $src0
        local.get $offset_x
        i32.const 4
        i32.add
        local.get $sizeline
        i32.rem_u
        local.tee $tmp
        i32.add
        i32.load
        i32.eq
        i32.const 0xffc8ffc8
        local.get $src1
        local.get $tmp
        i32.add
        i32.load
        i32.eq
        i32.const 0xffc8ffc8
        local.get $src2
        local.get $tmp
        i32.add
        i32.load
        i32.eq
        i32.add
        i32.add
        local.tee $c2
        local.get $c0
        local.get $c1
        i32.add
        i32.add
        local.tee $tmp
        i32.const 3
        i32.eq

        if
          local.get $dest
          i32.const 0xffc8ffc8
          i32.store
        else
          local.get $tmp
          i32.const 4
          i32.eq

          if
            local.get $dest
            local.get $src1
            local.get $offset_x
            i32.add
            i32.load
            i32.store
          else
            local.get $dest
            i32.const 0xff1e0f0f
            i32.store
          end
        end

        local.get $c1
        local.set $c0

        local.get $c2
        local.set $c1

        local.get $dest
        i32.const 4
        i32.add
        local.set $dest

        local.get $offset_x
        i32.const 4
        i32.add
        local.tee $offset_x
        local.get $sizeline
        i32.lt_u
        br_if $loop_x
      end

      i32.eqz
      br_if $loop_y
    end

    ;; S-expression
    ;; (local.set $dest (global.get $tmpmap))

    ;; (local.set $sizeline (i32.mul (global.get $width) (i32.const 4)))

    ;; (local.set $src1
    ;;   (i32.add
    ;;     (local.tee $src2 (global.get $map))
    ;;     (i32.mul
    ;;       (i32.sub (global.get $height) (i32.const 1))
    ;;       (local.get $sizeline)
    ;;     )
    ;;   )
    ;; )

    ;; (local.set $y (i32.const 0))

    ;; (loop $loop_y
    ;;   (local.set $src0 (local.get $src1))

    ;;   (local.set $src2
    ;;     (i32.add
    ;;       (local.tee $src1 (local.get $src2))
    ;;       (local.get $sizeline)
    ;;     )
    ;;   )

    ;;   (local.tee $tmp
    ;;     (i32.eq
    ;;       (local.tee $y (i32.add (local.get $y) (i32.const 1)))
    ;;       (global.get $height)
    ;;     )
    ;;   )

    ;;   (local.get $tmp)

    ;;   (if
    ;;     (then
    ;;       (local.set $src2 (global.get $map))
    ;;     )
    ;;   )

    ;;   (local.set $c0
    ;;     (i32.add
    ;;       (i32.eq
    ;;         (i32.const 0xffc8ffc8)
    ;;         (i32.load
    ;;           (i32.add
    ;;             (local.get $src0)
    ;;             (local.tee $tmp (i32.sub (local.get $sizeline) (i32.const 4)))
    ;;           )
    ;;         )
    ;;       )

    ;;       (i32.add
    ;;         (i32.eq
    ;;           (i32.const 0xffc8ffc8)
    ;;           (i32.load (i32.add (local.get $src1) (local.get $tmp)))
    ;;         )

    ;;         (i32.eq
    ;;           (i32.const 0xffc8ffc8)
    ;;           (i32.load (i32.add (local.get $src2) (local.get $tmp)))
    ;;         )
    ;;       )
    ;;     )
    ;;   )

    ;;   (local.set $c1
    ;;     (i32.add
    ;;       (i32.eq
    ;;         (i32.const 0xffc8ffc8)
    ;;         (i32.load (local.get $src0))
    ;;       )

    ;;       (i32.add
    ;;         (i32.eq
    ;;           (i32.const 0xffc8ffc8)
    ;;           (i32.load (local.get $src1))
    ;;         )

    ;;         (i32.eq
    ;;           (i32.const 0xffc8ffc8)
    ;;           (i32.load (local.get $src2))
    ;;         )
    ;;       )
    ;;     )
    ;;   )

    ;;   (local.set $offset_x (i32.const 0))

    ;;   (loop $loop_x
    ;;     (i32.eq
    ;;       (i32.const 0xffc8ffc8)
    ;;       (i32.load
    ;;         (i32.add
    ;;           (local.get $src0)
    ;;           (local.tee $tmp
    ;;             (i32.rem_u
    ;;               (i32.add (local.get $offset_x) (i32.const 4))
    ;;               (local.get $sizeline)
    ;;             )
    ;;           )
    ;;         )
    ;;       )
    ;;     )

    ;;     (i32.eq
    ;;       (local.tee $tmp
    ;;         (i32.add
    ;;           (local.tee $c2
    ;;             (i32.add
    ;;               (i32.eq
    ;;                 (i32.const 0xffc8ffc8)
    ;;                 (i32.load (i32.add (local.get $src1) (local.get $tmp)))
    ;;               )

    ;;               (i32.add
    ;;                 (i32.eq
    ;;                   (i32.const 0xffc8ffc8)
    ;;                   (i32.load (i32.add (local.get $src2) (local.get $tmp)))
    ;;                 )
    ;;               )
    ;;             )
    ;;           )
    ;;           (i32.add (local.get $c0) (local.get $c1))
    ;;         )
    ;;       )
    ;;       (i32.const 3)
    ;;     )

    ;;     (if
    ;;       (then
    ;;         (i32.store (local.get $dest) (i32.const 0xffc8ffc8))
    ;;       )
    ;;       (else
    ;;         (i32.eq (local.get $tmp) (i32.const 4))

    ;;         (if
    ;;           (then
    ;;             (i32.store (local.get $dest) (i32.load (i32.add (local.get $src1) (local.get $offset_x))))
    ;;           )
    ;;           (else
    ;;             (i32.store (local.get $dest) (i32.const 0xff1e0f0f))
    ;;           )
    ;;         )
    ;;       )
    ;;     )

    ;;     (local.set $c0 (local.get $c1))
    ;;     (local.set $c1 (local.get $c2))
    ;;     (local.set $dest (i32.add (local.get $dest) (i32.const 4)))

    ;;     (i32.lt_u
    ;;       (local.tee $offset_x (i32.add (local.get $offset_x) (i32.const 4)))
    ;;       (local.get $sizeline)
    ;;     )

    ;;     (br_if $loop_x)
    ;;   )

    ;;   (br_if $loop_y (i32.eqz))
    ;; )

    global.get $map
    global.get $tmpmap
    global.set $map
    global.set $tmpmap
  )

  (export "memory" (memory 0))
  (export "init" (func $init))
  (export "dot" (func $dot))
  (export "getmap" (func $getmap))
  (export "lifegame" (func $lifegame))
)
