'use strict';

let useAdaptiveFilter = false;

document.getElementById('enable-adaptive-filter').addEventListener('change', (event) => {
  if (event.currentTarget.checked) {
    useAdaptiveFilter = true;
  } else {
    useAdaptiveFilter = false;
  }
});

const remoteVideoElement = document.getElementById('remote');

document.getElementById('muted-remote').addEventListener('change', (event) => {
  if (event.currentTarget.checked) {
    remoteVideoElement.setAttribute('muted', 'muted');
  } else {
    remoteVideoElement.removeAttribute('muted');
  }
});

const svgLocal  = document.getElementById('analyser-local-time-domain');
const svgRemote = document.getElementById('analyser-remote-time-domain');

const svgWidth  = 600;
const svgHeight = 200;

const localObservedSignalPath  = document.createElementNS('http://www.w3.org/2000/svg', 'path');
const localTargetSignalPath    = document.createElementNS('http://www.w3.org/2000/svg', 'path');
const localReferenceSignalPath = document.createElementNS('http://www.w3.org/2000/svg', 'path');

const remoteReferenceSignalPath = document.createElementNS('http://www.w3.org/2000/svg', 'path');

svgLocal.appendChild(localObservedSignalPath);
svgLocal.appendChild(localTargetSignalPath);
svgLocal.appendChild(localReferenceSignalPath);

svgRemote.appendChild(remoteReferenceSignalPath);

const cancelEcho = (stream) => {
  const context = new AudioContext();

  const source = new MediaStreamAudioSourceNode(context, { mediaStream: stream });

  const processor = context.createScriptProcessor(2048, 2, 1);

  const remoteSource = new MediaElementAudioSourceNode(context, { mediaElement: remoteVideoElement });

  const splitter = new ChannelSplitterNode(context);

  source.connect(processor);
  processor.connect(context.destination);

  remoteSource.connect(splitter)
  splitter.connect(processor, 0);  // Observed Signal (local source + remote source)
  splitter.connect(processor, 1);  // Reference Signal
  processor.connect(context.destination);

  const bufferSize = processor.bufferSize;

  const filterLength = 1024;

  const delay     = 500;
  const step      = 0.025;
  const threshold = 0.000001
  const delta     = 0.00001;

  const render = (data, path, color) => {
    let d = '';

    for (let n = 0, len = data.length; n < len; n++) {
      const x = (n / len) * svgWidth;
      const y = (1 - data[n]) * (svgHeight / 2);

      if (n === 0) {
        d = `M${x} ${y}`;
      } else {
        d += `L${x} ${y}`;
      }
    }

    path.setAttribute('d', d);
    path.setAttribute('fill', 'none');
    path.setAttribute('stroke', color);
    path.setAttribute('stroke-width', '2');
  };

  processor.onaudioprocess = (event) => {
    const observedSignal = event.inputBuffer.getChannelData(0);
    const targetSignal   = event.outputBuffer.getChannelData(0);

    const referenceSignal = event.inputBuffer.getChannelData(1);

    render(observedSignal, localObservedSignalPath, 'rgba(0 255 255 / 100%)');
    render(referenceSignal, remoteReferenceSignalPath, 'rgba(255 0 0 / 100%)');
    render(targetSignal, localTargetSignalPath, 'rgba(0 255 0 / 100%)');

    if (!useAdaptiveFilter) {
      targetSignal.set(observedSignal);
      return;
    }

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

      err = observedSignal[t] - output0;

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

      targetSignal[t] = err;
    }

    render(targetSignal, localTargetSignalPath, 'rgba(0 255 0 / 100%)');
  };
};

const cancelEchoUseStepOnly = (stream) => {
  const context = new AudioContext();

  const source = new MediaStreamAudioSourceNode(context, { mediaStream: stream });

  const processor = context.createScriptProcessor(2048, 2, 1);

  const remoteSource = new MediaElementAudioSourceNode(context, { mediaElement: remoteVideoElement });

  const splitter = new ChannelSplitterNode(context);

  source.connect(processor);
  processor.connect(context.destination);

  remoteSource.connect(splitter)
  splitter.connect(processor, 0);  // Observed Signal (local source + remote source)
  splitter.connect(processor, 1);  // Reference Signal
  processor.connect(context.destination);

  const bufferSize = processor.bufferSize;

  const filterLength = 1024;

  const step = 0.01;

  const render = (data, path, color) => {
    let d = '';

    for (let n = 0, len = data.length; n < len; n++) {
      const x = (n / len) * svgWidth;
      const y = (1 - data[n]) * (svgHeight / 2);

      if (n === 0) {
        d = `M${x} ${y}`;
      } else {
        d += `L${x} ${y}`;
      }
    }

    path.setAttribute('d', d);
    path.setAttribute('fill', 'none');
    path.setAttribute('stroke', color);
    path.setAttribute('stroke-width', '2');
  };

  processor.onaudioprocess = (event) => {
    const observedSignal = event.inputBuffer.getChannelData(0);
    const targetSignal   = event.outputBuffer.getChannelData(0);

    const referenceSignal = event.inputBuffer.getChannelData(1);

    render(observedSignal, localObservedSignalPath, 'rgba(0 255 255 / 100%)');
    render(referenceSignal, remoteReferenceSignalPath, 'rgba(255 0 0 / 100%)');
    render(targetSignal, localTargetSignalPath, 'rgba(0 255 0 / 100%)');

    if (!useAdaptiveFilter) {
      targetSignal.set(observedSignal);
      return;
    }

    const adaptiveFilter = new Float32Array(filterLength);

    const referenceSignalBuffer = new Float32Array(filterLength);

    for (let t = 0; t < bufferSize; t++) {
      for (let n = filterLength - 1; n > 0; n--) {
        referenceSignalBuffer[n] = referenceSignalBuffer[n - 1];
      }

      referenceSignalBuffer[0] = referenceSignal[t]

      let err = 0;

      for (let n = 0; n < filterLength; n++) {
        err += adaptiveFilter[n] * referenceSignalBuffer[n];
      }

      targetSignal[t] = observedSignal[t] - err;

      for (let n = 0; n < filterLength; n++) {
        adaptiveFilter[n] += (step * targetSignal[t]) * referenceSignalBuffer[n];
      }
    }

    render(targetSignal, localTargetSignalPath, 'rgba(0 255 0 / 100%)');
  };
};
