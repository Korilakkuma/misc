'use strict';

class VocalCanceler extends AudioWorkletProcessor {
  static get parameterDescriptors() {
    return [{
      name          : 'depth',
      defaultValue  : 0,
      minValue      : 0,
      maxValue      : 1,
      automationRate: 'a-rate'
    }];
  }

  constructor() {
    super();
  }

  process(inputs, outputs, parameters) {
    console.log('process');
    const input  = inputs[0];
    const output = outputs[0];

    const numberOfChannels = output.length;

    console.log(numberOfChannels);

    if (numberOfChannels === 2) {
      const inputLs  = input[0];
      const inputRs  = input[1];
      const outputLs = output[0];
      const outputRs = output[1];

      for (let i = 0, len = outputLs.length; i < len; i++) {
        const depth = parameters.depth.length > 1 ? parameters.depth[i] : parameters.depth[0];

        outputLs[i] = inputLs[i] - (depth * inputRs[i]);
        outputRs[i] = inputRs[i] - (depth * inputLs[i]);
      }
    } else {
      output[0].set(input[0]);
    }

    return true;
  }
}

registerProcessor('vocal-canceler', VocalCanceler);
