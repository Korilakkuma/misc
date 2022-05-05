(module
  (import "env" "memory" (memory 80))

  (global $noHitColor (import "env" "noHitColor") i32)
  (global $hitColor (import "env" "hitColor") i32)
  (global $numberOfObjects (import "env" "numberOfObjects") i32)
  (global $numberOfPixels (import "env" "numberOfPixels") i32)
  (global $canvasSize (import "env" "canvasSize") i32)
  (global $width (import "env" "width") i32)
  (global $height (import "env" "height") i32)
  (global $size (import "env" "size") i32)
  (global $start (import "env" "start") i32)
  (global $startIndex (import "env" "startIndex") i32)
  (global $stride (import "env" "stride") i32)
  (global $offsetX (import "env" "offsetX") i32)
  (global $offsetY (import "env" "offsetY") i32)
  (global $offsetVx (import "env" "offsetVx") i32)
  (global $offsetVy (import "env" "offsetVy") i32)

  (func (export "Collide")
    (local $i i32)
    (local $j i32)

    (local $x1 i32)
    (local $x2 i32)
    (local $y1 i32)
    (local $y2 i32)

    (local $distX i32)
    (local $distY i32)

    (local $isHit i32)
    (local $vx i32)
    (local $vy i32)

    call $clear

    (loop $moveLoop
      local.get $i
      global.get $offsetX
      call $getAttribute
      local.set $x1

      local.get $i
      global.get $offsetY
      call $getAttribute
      local.set $y1

      local.get $i
      global.get $offsetVx
      call $getAttribute
      local.set $vx

      local.get $i
      global.get $offsetVy
      call $getAttribute
      local.set $vy

      local.get $vx
      local.get $x1
      i32.add
      i32.const 0x000001ff  ;; 511
      i32.and
      local.set $x1

      local.get $vy
      local.get $y1
      i32.add
      i32.const 0x000001ff  ;; 511
      i32.and
      local.set $y1

      local.get $i
      global.get $offsetX
      local.get $x1
      call $setAttribute

      local.get $i
      global.get $offsetY
      local.get $y1
      call $setAttribute

      local.get $i
      i32.const 1
      i32.add
      local.tee $i
      global.get $numberOfObjects
      i32.lt_u
      br_if $moveLoop
    )

    i32.const 0
    local.set $i

    (loop $outerLoop
      (block $outerBreak
        i32.const 0
        local.tee $j
        local.set $isHit

        local.get $i
        global.get $offsetX
        call $getAttribute
        local.set $x1

        local.get $i
        global.get $offsetY
        call $getAttribute
        local.set $y1

        (loop $innerLoop
          (block $innerBreak
            local.get $i
            local.get $j
            i32.eq

            if
              local.get $j
              i32.const 1
              i32.add
              local.set $j
            end

            local.get $j
            global.get $numberOfObjects
            i32.ge_u
            br_if $innerBreak

            local.get $j
            global.get $offsetX
            call $getAttribute
            local.set $x2

            local.get $x1
            local.get $x2
            i32.sub
            call $abs
            local.tee $distX
            global.get $numberOfPixels
            i32.ge_u
            if
              ;; no hit
              local.get $j
              i32.const 1
              i32.add
              local.set $j
              br $innerLoop
            end

            local.get $j
            global.get $offsetY
            call $getAttribute
            local.set $y2

            local.get $y1
            local.get $y2
            i32.sub
            call $abs
            local.tee $distY
            global.get $numberOfPixels
            i32.ge_u
            if
              ;; no hit
              local.get $j
              i32.const 1
              i32.add
              local.set $j
              br $innerLoop
            end

            ;; hit
            i32.const 1
            local.set $isHit
          )
        )

        local.get $isHit
        i32.const 0
        i32.eq
        if
          ;; no hit
          local.get $x1
          local.get $y1
          global.get $noHitColor
          call $draw
        else
          ;; hit
          local.get $x1
          local.get $y1
          global.get $hitColor
          call $draw
        end

        local.get $i
        i32.const 1
        i32.add
        local.tee $i
        global.get $numberOfObjects
        i32.lt_u
        br_if $outerLoop
      )
    )
  )

  (func $clear
    (local $i i32)
    (local $pixelBytes i32)

    global.get $width
    global.get $height
    i32.mul
    i32.const 4
    i32.mul
    local.set $pixelBytes

    (loop $loop
      local.get $i
      i32.const 0xff000000
      i32.store

      local.get $i
      i32.const 4
      i32.add
      local.set $i

      local.get $i
      local.get $pixelBytes
      i32.lt_u
      br_if $loop
    )
  )

  (func $abs (param $value i32) (result i32)
    local.get $value
    i32.const 0
    i32.ge_s

    if
      local.get $value
      return
    end

    i32.const 0
    local.get $value
    i32.sub
  )

  (func $setPixel (param $x i32) (param $y i32) (param $c i32)
    local.get $x
    global.get $width
    i32.ge_u
    if
      return
    end

    local.get $y
    global.get $height
    i32.ge_u
    if
      return
    end

    local.get $y
    global.get $canvasSize
    i32.mul
    local.get $x
    i32.add  ;; x + (y * canvas size) = position in linear memory
    i32.const 4  ;; 4 bytes each pixel
    i32.mul
    local.get $c
    i32.store
  )

  (func $draw (param $x i32) (param $y i32) (param $c i32)
    (local $maxX i32)
    (local $maxY i32)

    (local $xi i32)
    (local $yi i32)

    local.get $x
    local.tee $xi
    global.get $numberOfPixels
    i32.add
    local.set $maxX

    local.get $y
    local.tee $yi
    global.get $numberOfPixels
    i32.add
    local.set $maxY

    (loop $loop
      (block $break
        local.get $xi
        local.get $yi
        local.get $c
        call $setPixel

        local.get $xi
        i32.const 1
        i32.add
        local.get $xi
        local.get $maxX
        i32.ge_u

        if
          local.get $x
          local.set $xi

          local.get $yi
          i32.const 1
          i32.add
          local.tee $yi
          local.get $maxY
          i32.ge_u
          br_if $loop
        end

        br $break
      )
    )
  )

  (func $setAttribute (param $index i32) (param $offset i32) (param $value i32)
    local.get $index
    global.get $stride
    i32.mul
    global.get $start
    i32.add  ;; base address + (position * stride)
    local.get $offset
    i32.add  ;; (base address + (position * stride)) + offset
    local.get $value
    i32.store
  )

  (func $getAttribute (param $index i32) (param $offset i32) (result i32)
    local.get $index
    global.get $stride
    i32.mul
    global.get $start
    i32.add  ;; base address + (position * stride)
    local.get $offset
    i32.add  ;; (base address + (position * stride)) + offset
    i32.load
  )
)
