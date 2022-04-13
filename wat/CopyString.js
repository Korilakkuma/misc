const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/CopyString.wasm`);

const maxMemory = (2 ** 16) - 1;

const memory = new WebAssembly.Memory({ initial: 1 });

const importObject = {
  env: {
    buffer   : memory,
    strPosLen: (pos, len) => {
      const bytes  = new Uint8Array(memory.buffer, pos, len);
      const string = new TextDecoder('utf8').decode(bytes);

      console.log(string);
    },
    strNull  : (pos) => {
      const bytes  = new Uint8Array(memory.buffer, pos, (maxMemory - pos));
      const string = new TextDecoder('utf8').decode(bytes).split('\0')[0];

      console.log(string);
    },
    strPrefix: (pos) => {
      const length = new Uint8Array(memory.buffer, pos, 1)[0];
      const bytes  = new Uint8Array(memory.buffer, (pos + 1), length);
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
