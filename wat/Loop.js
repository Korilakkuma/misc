const fs = require('fs');

const bytes = fs.readFileSync(`${__dirname}/Loop.wasm`);

const n = Number(process.argv[2]);

if ((n < 1) || (n > 12)) {
  console.error('n >= 1 and n <= 12 (32 bit integer)');
  process.exit(1);
}

WebAssembly
  .instantiate(new Uint8Array(bytes))
  .then(({ instance }) => {
    const r = instance.exports.Loop(n);

    console.log(`${n}! = ${r}`);
  });
