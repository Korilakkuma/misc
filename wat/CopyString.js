const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/CopyString.wasm`);

const memory = new WebAssembly.Memory({ initial: 1 });

const importObject = {
  env: {
    buffer   : memory,
    strPosLen: (pos, len) => {
      const bytes  = new Uint8Array(memory.buffer, pos, len);
      const string = new TextDecoder('utf8').decode(bytes);

      console.log(string);
    }
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    instance.exports.main();
  });
