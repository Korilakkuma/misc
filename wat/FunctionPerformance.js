const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/FunctionPerformance.wasm`);

const exec = (func) => {
  const start = Date.now();

  func();

  const time = Date.now() - start;

  console.log(`${func.name} time = ${time}`);
};

let i = 0;

const importObject = {
  js: {
    externalCall: () => {
      i++;
      return i;
    }
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    const { WasmCall, JsCall } = instance.exports;

    exec(WasmCall);
    exec(JsCall);
  })
