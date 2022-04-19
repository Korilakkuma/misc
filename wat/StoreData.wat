(module
  (import "env" "memory" (memory 1))
  (global $address (import "env" "address") i32)
  (global $count (import "env" "count") i32)

  (func $Store (param $index i32) (param $value i32)
    global.get $address
    local.get $index
    i32.const 4  ;; 32 bits /  8 bits = 4 bytes
    i32.mul
    i32.add
    local.get $value
    i32.store
  )

  (func $init
    (local $index i32)

    (loop $loop
      local.get $index
      local.get $index
      i32.const 5
      i32.mul
      call $Store  ;; $Store($index, ($index * 5))

      local.get $index
      i32.const 1
      i32.add
      local.tee $index
      global.get $count
      i32.lt_u
      br_if $loop
    )

    global.get $address
    i32.const 1
    i32.store
  )

  (start $init)
)
