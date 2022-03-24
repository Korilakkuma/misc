(module
  (func $eq2 (param $n i32)
    (result i32)

    local.get $n
    i32.const 2
    i32.eq
  )

  (func $isEven (param $n i32)
    (result i32)

    local.get $n
    i32.const 2

    i32.rem_u
    i32.const 0
    i32.eq
  )

  (func $isMultiple (param $n i32) (param $m i32)
    (result i32)
    local.get $n
    local.get $m

    i32.rem_u
    i32.const 0
    i32.eq
  )

  (func (export "isPrimeNumber") (param $n i32)
    (result i32)

    (local $i i32)

    (if (i32.eq (local.get $n) (i32.const 1))
      (then
        i32.const 0
        return
      )
    )

    (if (call $eq2 (local.get $n))
      (then
        i32.const 1
        return
      )
    )

    (block $notPrimeNumber
      (call $isEven (local.get $n))

      br_if $notPrimeNumber

      (local.set $i (i32.const 1))

      (loop $isPrimeNumberLoop
        (local.set $i (i32.add (local.get $i) (i32.const 2)))

        (if (i32.ge_u (local.get $n) (local.get $i))
          (then
            i32.const 1
            return
          )
        )

        (br_if $notPrimeNumber
          (call $isMultiple (local.get $n) (local.get $i))
        )

        br $isPrimeNumberLoop
      )
    )

    i32.const 0
  )
)
