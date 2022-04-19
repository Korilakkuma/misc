const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/StoreData.wasm`);

const memory  = new WebAssembly.Memory({ initial: 1 });
const address = 32;
const count   = 16;

const i32memories = new Uint32Array(memory.buffer);

const importObject = {
  env: {
    memory,
    address,
    count
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(() => {
    const maxIndex = (address / 4) + count + 4;

    for (let i = 0; i < maxIndex; i++) {
      const data = i32memories[i];

      if (data === 0) {
        console.log(`\x1b[0mdata[${i}] = ${data}`);
      } else {
        console.log(`\x1b[31mdata[${i}] = ${data}`);
      }
    }
  });
