(module
  (import "js" "externalCall" (func $ExternalCall (result i32)))
  (global $i (mut i32) (i32.const 0))

  (func $InternalCall (result i32)
    global.get $i
    i32.const 1
    i32.add
    global.set $i

    global.get $i
  )

  (func (export "WasmCall")
    (loop $again
      call $InternalCall

      i32.const 4000000
      i32.le_u

      br_if $again
    )
  )

  (func (export "JsCall")
    (loop $again
      call $ExternalCall

      i32.const 4000000
      i32.le_u

      br_if $again
    )
  )
)
