(module
  (import "env" "memory" (memory 1))
  (global $base (import "env" "base") i32)
  (global $count (import "env" "count") i32)
  (global $stride (import "env" "stride") i32)
  (global $offsetX (import "env" "offsetX") i32)
  (global $offsetY (import "env" "offsetY") i32)
  (global $offsetRadius (import "env" "offsetRadius") i32)
  (global $offsetCollision (import "env" "offsetCollision") i32)

  (func $DetectCollision (param $x1 i32) (param $y1 i32) (param $r1 i32) (param $x2 i32) (param $y2 i32) (param $r2 i32) (result i32)
    (local $xDiffSq i32)
    (local $yDiffSq i32)
    (local $rSumSq i32)

    local.get $x1
    local.get $x2
    i32.sub
    local.tee $xDiffSq
    local.get $xDiffSq
    i32.mul
    local.set $xDiffSq  ;; distance = (x1 - x2)^2

    local.get $y1
    local.get $y2
    i32.sub
    local.tee $yDiffSq
    local.get $yDiffSq
    i32.mul
    local.set $yDiffSq  ;; distance = (y1 - y2)^2

    local.get $r1
    local.get $r2
    i32.add
    local.tee $rSumSq
    local.get $rSumSq
    i32.mul
    local.tee $rSumSq  ;; (r1 + r2)^2

    local.get $xDiffSq
    local.get $yDiffSq
    i32.add  ;; Pythagoras theorem

    i32.gt_u  ;; Inner circle ?
  )

  (func $GetAttribute (param $base i32) (param $offset i32) (result i32)
    local.get $base
    local.get $offset
    i32.add
    i32.load  ;; Return address
  )

  (func $SetCollision (param $base1 i32) (param $base2 i32)
    local.get $base1
    global.get $offsetCollision
    i32.add
    i32.const 1
    i32.store  ;; collision is true

    local.get $base2
    global.get $offsetCollision
    i32.add
    i32.const 1
    i32.store  ;; collision is true
  )

  (func $init
    (local $i i32)
    (local $ai i32)
    (local $xi i32)
    (local $yi i32)
    (local $ri i32)

    (local $j i32)
    (local $aj i32)
    (local $xj i32)
    (local $yj i32)
    (local $rj i32)

    (loop $outerLoop
      i32.const 0
      local.set $j

      (loop $innerLoop
        (block $continue
          local.get $i
          local.get $j
          i32.eq
          br_if $continue

          global.get $stride
          local.get $i
          i32.mul
          global.get $base
          i32.add
          local.set $ai

          local.get $ai
          global.get $offsetX
          call $GetAttribute
          local.set $xi

          local.get $ai
          global.get $offsetY
          call $GetAttribute
          local.set $yi

          local.get $ai
          global.get $offsetRadius
          call $GetAttribute
          local.set $ri

          global.get $stride
          local.get $j
          i32.mul
          global.get $base
          i32.add
          local.set $aj

          local.get $aj
          global.get $offsetX
          call $GetAttribute
          local.set $xj

          local.get $aj
          global.get $offsetY
          call $GetAttribute
          local.set $yj

          local.get $aj
          global.get $offsetRadius
          call $GetAttribute
          local.set $rj

          local.get $xi
          local.get $yi
          local.get $ri
          local.get $xj
          local.get $yj
          local.get $rj
          call $DetectCollision

          if
            local.get $aj
            local.get $ai
            call $SetCollision
          end
        )

        local.get $j
        i32.const 1
        i32.add
        local.tee $j
        global.get $count
        i32.lt_u
        br_if $innerLoop
      )

      local.get $i
      i32.const 1
      i32.add
      local.tee $i
      global.get $count
      i32.lt_u
      br_if $outerLoop
    )
  )

  (start $init)
)
