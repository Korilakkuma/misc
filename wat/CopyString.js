const fs = require('fs');

const bytes = fs.readFileSync(`${__dirname}/CopyString.wasm`);

const importObject = {
};

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(({ instance }) => {
    // TODO;
  });
