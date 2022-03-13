const fs = require('fs');
const bytes = fs.readFileSync(`${__dirname}/Globals.wasm`);

const i32 = 5_000_000_000;

const importObject = {
  js: {
    log_i32: (value) => console.log(`i32: ${value} (0x${i32.toString(16)} - 0xffffffff = 0x${value.toString(16)})`),
    log_f32: (value) => console.log(`f32: ${value} (${value.toString(2)})`),
    log_f64: (value) => console.log(`f64: ${value} (${value.toString(2)})`),
  },
  env: {
    import_i32: i32,
    import_f32: 123.0123456789,
    import_f64: 123.0123456789,
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    instance.exports.Globals();
  });
