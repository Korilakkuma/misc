const fs    = require('fs');
const bytes = fs.readFileSync(`${__dirname}/CollisionDetection.wasm`);

const memory      = new WebAssembly.Memory({ initial: 1 });
const i32memories = new Uint32Array(memory.buffer);

const base   = 0;
const count  = 32;
const stride = 16;  // bytes

const offsetX         = 0;
const offsetY         = 4;
const offsetRadius    = 8;
const offsetCollision = 12;

const offsetXIndex         = offsetX / 4;
const offsetYIndex         = offsetY / 4;
const offsetRadiusIndex    = offsetRadius / 4;
const offsetCollisionIndex = offsetCollision / 4;

const importObject = {
  env: {
    memory,
    base,
    count,
    stride,
    offsetX,
    offsetY,
    offsetRadius,
    offsetCollision
  }
};

for (let i = 0; i < count; i++) {
  const index = (i * (stride / 4)) + (base / 4);

  i32memories[index + offsetXIndex]      = Math.trunc(Math.random() * 100);
  i32memories[index + offsetYIndex]      = Math.trunc(Math.random() * 100);
  i32memories[index + offsetRadiusIndex] = Math.trunc(Math.random() * 10);
}

WebAssembly
  .instantiate(new Uint8Array(bytes), importObject)
  .then(() => {
    for (let i = 0; i < count; i++) {
      const index = (i * (stride / 4)) + (base / 4);

      const x = i32memories[index + offsetXIndex].toString().padStart(2, ' ');
      const y = i32memories[index + offsetYIndex].toString().padStart(2, ' ');
      const r = i32memories[index + offsetRadiusIndex].toString().padStart(2, '');
      const c = i32memories[index + offsetCollisionIndex];

      if (c === 1) {
        console.log(`\x1b[31m[${i.toString().padStart(2, '0')}] = (x = ${x}, y = ${y}, r = ${r})`);
      } else {
        console.log(`\x1b[0m[${i.toString().padStart(2, '0')}] = (x = ${x}, y = ${y}, r = ${r})`);
      }
    }
  });
