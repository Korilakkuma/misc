const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/HelloWorld.wasm`);

let memory = null;

const importObject = {
  HelloWorld: {
    console: (index) => {
      if (memory === null) {
        console.log('memory buffer is null');
        return;
      }

      const lengthIndex = index - 4;  // 2 * UTF-16 (2 * 2 bytes = 4 bytes)
      const lengthArray = new Uint32Array(memory.buffer, lengthIndex, (4 / Uint32Array.BYTES_PER_ELEMENT));
      const length      = lengthArray[0];
      const stringBytes = new Uint16Array(memory.buffer, index, (length / Uint16Array.BYTES_PER_ELEMENT));
      const message     = new TextDecoder('utf-16').decode(stringBytes);

      console.log(message);
    }
  },
  env: {
    abort: () => {}
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    memory = instance.exports.memory;
    instance.exports.HelloWorld();
  });
