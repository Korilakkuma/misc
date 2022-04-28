(module
  (func (export "Add") (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.add
  )

  (func (export "Sub") (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.sub
  )

  (func (export "Mul") (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.mul
  )

  (func (export "Div") (param $a i32) (param $b i32) (result i32)
    local.get $a
    local.get $b
    i32.div_s
  )
)
