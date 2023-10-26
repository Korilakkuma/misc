'use strict';

// the number of function samples
const N = 44100;

const PADDING      = 48;
const SIZE_OF_RECT = 4;
const LINE_WIDTH   = 8;

const PIXEL_RATIO = 4;

const plotSeries = (canvas, context, series) => {
  const width  = canvas.width;
  const height = canvas.height;

  context.fillStyle = '#666';
  context.fillRect(PADDING, ((height / 2) - (SIZE_OF_RECT / 2)), (width - (2 * PADDING)), SIZE_OF_RECT);
  context.fillRect(((width / 2) - (SIZE_OF_RECT / 2)), 0, SIZE_OF_RECT, height);

  context.font = '48px Roboto';

  const textX = ((width / 2) - (SIZE_OF_RECT / 2)) - 88;

  [[' 1.0', 1], [' π/4', (Math.PI / 4)], [' 0.0', 0], ['-π/4', -(Math.PI / 4)], ['-1.0', -1]].forEach(([text, value]) => {
    context.fillText(text, textX, ((1 - value) * ((height - (2 * PADDING)) / 2)) + PADDING);
  });

  context.lineWidth = LINE_WIDTH;
  context.strokeStyle = 'rgba(0 0 255 / 30%)';

  context.beginPath();

  for (let n = 0; n < N; n++) {
    const plotX = (n * ((width - (2 * PADDING)) / N)) + PADDING;
    const y     = series[n];
    const plotY = ((1 - y) * ((height - (2 * PADDING)) / 2)) + PADDING;

    if (n === 0) {
      context.moveTo(plotX, plotY);
    } else {
      context.lineTo(plotX, plotY);
    }
  }

  context.stroke();
};

WebAssembly
  .instantiateStreaming(fetch('./fourier-series.wasm'))
  .then(({ instance }) => {
    const canvas = document.getElementById('canvas');

    canvas.style.width  = `${Math.trunc(canvas.width / PIXEL_RATIO)}px`;
    canvas.style.height = `${Math.trunc(canvas.height / PIXEL_RATIO)}px`;

    const context = canvas.getContext('2d');

    const plotButtonElement      = document.getElementById('button-plot');
    const clearButtonElement     = document.getElementById('button-clear');
    const animationButtonElement = document.getElementById('button-animation');
    const intervalSelectElement  = document.getElementById('select-interval');
    const functionSelectElement  = document.getElementById('select-function');
    const rangeElement           = document.getElementById('range-sum');
    const outputTextElement      = document.getElementById('output-sum');

    const linearMemory = instance.exports.memory.buffer;

    const createTypedArrayFromLinearMemory = (linearMemory, k) => {
      switch (functionSelectElement.value) {
        case 'square': {
          const offset = instance.exports.square(N, k);

          return new Float32Array(linearMemory, offset, N);
        }

        case 'sawtooth': {
          const offset = instance.exports.sawtooth(N, k);

          return new Float32Array(linearMemory, offset, N);
        }

        case 'triangle': {
          const offset = instance.exports.triangle(N, k);

          return new Float32Array(linearMemory, offset, N);
        }

        default: {
          const sinc = new Float32Array(N);

          const RESOLUTION = 3000;

          const offset = Math.trunc(N / 2);

          for (let n = -offset; n < offset; n++) {
            const x = n / RESOLUTION;

            if (n === 0) {
              sinc[offset] = 1;
              continue;
            }

            sinc[offset + n] = Math.sin(Math.PI * x) / (Math.PI * x);
          }

          return sinc;
        }
      }
    };

    let K = rangeElement.valueAsNumber;

    let intervalId = null;
    let animationId = null;

    plotSeries(canvas, context, createTypedArrayFromLinearMemory(linearMemory, K));

    plotButtonElement.addEventListener('click', () => {
      plotSeries(canvas, context, createTypedArrayFromLinearMemory(linearMemory, K));
    }, false);

    clearButtonElement.addEventListener('click', () => {
      context.clearRect(0, 0, canvas.width, canvas.height);

      if (intervalId || animationId) {
        if (intervalId) {
          window.clearInterval(intervalId);
          intervalId = null;
        }

        if (animationId) {
          window.cancelAnimationFrame(animationId);
          animationId = null
        }

        plotButtonElement.removeAttribute('disabled');
        animationButtonElement.removeAttribute('disabled');
        intervalSelectElement.removeAttribute('disabled');
        functionSelectElement.removeAttribute('disabled');
        rangeElement.removeAttribute('disabled');
      }
    }, false);

    animationButtonElement.addEventListener('click', () => {
      plotButtonElement.setAttribute('disabled', 'disabled');
      animationButtonElement.setAttribute('disabled', 'disabled');
      intervalSelectElement.setAttribute('disabled', 'disabled');
      functionSelectElement.setAttribute('disabled', 'disabled');
      rangeElement.setAttribute('disabled', 'disabled');

      K = 1;

      const animate = (auto = false) => {
        context.clearRect(0, 0, canvas.width, canvas.height);

        plotSeries(canvas, context, createTypedArrayFromLinearMemory(linearMemory, K));

        rangeElement.valueAsNumber    = K;
        outputTextElement.textContent = K;

        ++K;

        if (K > 100) {
          if (intervalId) {
            window.clearInterval(intervalId);
            intervalId = null;
          }

          if (animationId) {
            window.cancelAnimationFrame(animationId);
            animationId = null;
          }

          plotButtonElement.removeAttribute('disabled');
          animationButtonElement.removeAttribute('disabled');
          intervalSelectElement.removeAttribute('disabled');
          functionSelectElement.removeAttribute('disabled');
          rangeElement.removeAttribute('disabled');
          return;
        }

        if (auto) {
          animationId = window.requestAnimationFrame(() => {
            animate(auto);
          });
        }
      };

      if (intervalSelectElement.value === '60 fps') {
        animate(true);
      } else {
        const interval = Number(intervalSelectElement.value);

        intervalId = window.setInterval(animate, interval);
      }
    }, false);

    rangeElement.addEventListener('input', (event) => {
      K = event.currentTarget.valueAsNumber;

      outputTextElement.textContent = K;
    });

    rangeElement.addEventListener('change', (event) => {
      plotSeries(canvas, context, createTypedArrayFromLinearMemory(linearMemory, K));
    }, false);

    functionSelectElement.addEventListener('change', (event) => {
      if (event.currentTarget.value === 'sinc') {
        animationButtonElement.setAttribute('disabled', 'disabled');
        intervalSelectElement.setAttribute('disabled', 'disabled');
        rangeElement.setAttribute('disabled', 'disabled');
      } else {
        animationButtonElement.removeAttribute('disabled');
        intervalSelectElement.removeAttribute('disabled');
        rangeElement.removeAttribute('disabled');
      }
    }, false);
  })
  .catch(console.error);
