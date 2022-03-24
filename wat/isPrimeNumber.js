const fs = require('fs');

const bytes = fs.readFileSync(`${__dirname}/isPrimeNumber.wasm`);

const n = Number(process.argv[2]);

WebAssembly
  .instantiate(new Uint8Array(bytes))
  .then(({ instance }) => {
    if (instance.exports.isPrimeNumber(n)) {
      console.log(`${n} is prime number`);
    } else {
      console.log(`${n} is not prime number`);
    }
  });
