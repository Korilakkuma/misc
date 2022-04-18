const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/Pointer.wasm`);

const memory = new WebAssembly.Memory({ initial: 1, maximum: 4 });

const importObject = { env: { memory } };

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    const pointer = instance.exports.Pointer();

    console.log(`pointer value = ${pointer}`);
  });
