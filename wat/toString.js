const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/toString.wasm`);

const num = Number(process.argv[2]);

const memory = new WebAssembly.Memory({ initial: 1 });

const importObject = {
  env: {
    buffer     : memory,
    printString: (pos, len) => {
      const b = new Uint8Array(memory.buffer, pos, len);
      const s = new TextDecoder('utf8').decode(b);

      console.log(`>${s}!`);
    }
  }
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    instance.exports.toString10(num);
    instance.exports.toString16(num);
    instance.exports.toString2(num);
  });
