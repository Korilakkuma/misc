'use strict';

class Bypass extends AudioWorkletProcessor {
  constructor() {
    super();

    console.log(this);
  }

  process(inputs, outputs) {
    const input  = inputs[0];
    const output = outputs[0];

    const numberOfChannels = output.length;

    for (let channel = 0; channel < numberOfChannels; channel++) {
      output[channel].set(input[channel]);
    }

    return true;
  }
}

registerProcessor('bypass', Bypass);
