//  node --print-bytecode --print-bytecode-filter=bytecodeTest bytecode.js

function bytecodeTest() {
  let x = 0;

  for (let i = 0; i < 10_000_000; i++) {
    x = i % 1000;
  }

  return 99;
}

bytecodeTest();
