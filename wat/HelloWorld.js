const fs = require('fs');
const bytes = fs.readFileSync(`${__dirname}/HelloWorld.wasm`);

const startStringIndex = 100;

const memory = new WebAssembly.Memory({ initial: 1 });

const importObject = {
  env: {
    buffer: memory,
    start_string: startStringIndex,
    print_string: (strLen) => {
      const bytes     = new Uint8Array(memory.buffer, startStringIndex, strLen);
      const logString = new TextDecoder('utf8').decode(bytes);
      console.log(logString);
    }
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    instance.exports.HelloWorld();
  });
