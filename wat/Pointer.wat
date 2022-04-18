(module
  (memory 1)
  (global $pointer i32 (i32.const 128))

  (func $init
    global.get $pointer
    i32.const 99
    i32.store
  )

  (func (export "Pointer") (result i32)
    global.get $pointer
    i32.load
  )

  (start $init)
)
