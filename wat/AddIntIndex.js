const fs = require('fs');

const bytes = fs.readFileSync(`${__dirname}/AddIntIndex.wasm`);

const v1 = Number(process.argv[2]);
const v2 = Number(process.argv[3]);

WebAssembly
  .instantiate(new Uint8Array(bytes))
  .then(({ instance }) => {
    console.log(`${v1} + ${v2} = ${instance.exports.AddInt(v1, v2)}`);
  });
