const fs             = require('fs');
const tableBytes     = fs.readFileSync(`${__dirname}/Table.wasm`);
const tableTestBytes = fs.readFileSync(`${__dirname}/TableTest.wasm`);

let i = 0;

const increment = () => {
  i++;
  return i;
};

const decrement = () => {
  i--;
  return i;
};

const exec = (func) => {
  i = 0;

  const start = Date.now();

  func();

  const time = Date.now() - start;

  console.log(`${func.name} time is ${time}`);
};

const importObject = {
  js: {
    tbl          : null,
    increment    : increment,
    decrement    : decrement,
    wasmIncrement: null,
    wasmDecrement: null
  }
};

WebAssembly
  .instantiate(new Uint8Array(tableBytes), importObject)
  .then(({ instance: tableInstance }) => {
    importObject.js.tbl           = tableInstance.exports.tbl;
    importObject.js.wasmIncrement = tableInstance.exports.increment;
    importObject.js.wasmDecrement = tableInstance.exports.decrement;

    WebAssembly
     .instantiate(new Uint8Array(tableTestBytes), importObject)
     .then(({ instance: tableTestInstance }) => {
       console.log(tableTestInstance.exports);
       const { jsTableTest, jsImportTest, wasmTableTest, wasmImportTest } = tableTestInstance.exports;

       exec(jsTableTest);
       exec(jsImportTest);
       exec(wasmTableTest);
       exec(wasmImportTest);
     })
  });
