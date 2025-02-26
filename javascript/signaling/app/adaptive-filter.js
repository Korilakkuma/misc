'use strict';

const cancelEcho = (stream) => {
  const context = new AudioContext();

  const source = new MediaStreamAudioSourceNode(context, stream);

  const processor = context.createScriptProcessor(8192, 1, 1);

  source.connect(processor);
  processor.connect(context.destination);

  const bufferSize = processor.bufferSize;

  processor.onaudioprocess = (event) => {
    const inputs  = event.inputBuffer.getChannelData(0);
    const outputs = event.outputBuffer.getChannelData(0);

    for (let t = 0; t < bufferSize; t++) {
    }
  };
};
