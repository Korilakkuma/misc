'use strict';

const cancelEcho = (stream) => {
  const context = new AudioContext();

  const source = new MediaStreamAudioSourceNode(context, stream);

  const processor = context.createScriptProcessor(8192, 1, 1);

  source.connect(processor);
  processor.connect(context.destination);

  const bufferSize = processor.bufferSize;

  const filterLength = 1024;

  const delay     = 500;
  const step      = 0.025;
  const threshold = 0.000001
  const delta     = 0.00001;

  processor.onaudioprocess = (event) => {
    const inputs  = event.inputBuffer.getChannelData(0);
    const outputs = event.outputBuffer.getChannelData(0);

    const referenceSignal = new Float32Array(bufferSize);

    const adaptiveFilter = new Float32Array(filterLength);

    for (let t = 0; t < bufferSize; t++) {
      let norm = 0;
      let err  = 0;

      let output0 = 0;

      for (let n = 0; n < filterLength; n++) {
        const index = t - delay - n;

        if (index < 0 || index >= length) {
          continue;
        }

        output0 += adaptiveFilter[n] * referenceSignal[index];
        norm += referenceSignal[index] ** 2;
      }

      err = inputs[t] - output0;

      if (norm > threshold) {
        // Update Adaptive Filter (NLMS: Normalized Least Mean Square algorithm)
        for (let n = 0; n < filterLength; n++) {
          const index = t - delay - n;

          if (index < 0 || index >= length) {
            continue;
          }

          adaptiveFilter[n] += (step * (referenceSignal[index] * err)) / (norm + delta);
        }
      }

     outputs[t] = err;
   }
  };
};
