(module
  (import "js" "tbl" (table $tbl 4 anyfunc))
  (import "js" "increment" (func $increment (result i32)))
  (import "js" "decrement" (func $decrement (result i32)))
  (import "js" "wasmIncrement" (func $wasmIncrement (result i32)))
  (import "js" "wasmDecrement" (func $wasmDecrement (result i32)))

  (type $returnsi32 (func (result i32)))

  (global $incPtr i32 (i32.const 0))
  (global $decPtr i32 (i32.const 1))
  (global $wasmIncPtr i32 (i32.const 2))
  (global $wasmDecPtr i32 (i32.const 3))

  (func (export "jsTableTest")
    (loop $incCycle
      (call_indirect (type $returnsi32) (global.get $incPtr))
      i32.const 4000000
      i32.le_u
      br_if $incCycle
    )

    (loop $decCycle
      (call_indirect (type $returnsi32) (global.get $decPtr))
      i32.const 4000000
      i32.le_u
      br_if $decCycle
    )
  )

  (func (export "jsImportTest")
    (loop $incCycle
      call $increment
      i32.const 4000000
      i32.le_u
      br_if $incCycle
    )

    (loop $decCycle
      call $decrement
      i32.const 4000000
      i32.le_u
      br_if $decCycle
    )
  )

  (func (export "wasmTableTest")
    (loop $incCycle
      (call_indirect (type $returnsi32) (global.get $wasmIncPtr))
      i32.const 4000000
      i32.le_u
      br_if $incCycle
    )

    (loop $decCycle
      (call_indirect (type $returnsi32) (global.get $wasmDecPtr))
      i32.const 4000000
      i32.le_u
      br_if $decCycle
    )
  )

  (func (export "wasmImportTest")
    (loop $incCycle
      call $wasmIncrement
      i32.const 4000000
      i32.le_u
      br_if $incCycle
    )

    (loop $decCycle
      call $wasmDecrement
      i32.const 4000000
      i32.le_u
      br_if $decCycle
    )
  )
)
