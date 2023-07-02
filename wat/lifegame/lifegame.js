'use strict';

WebAssembly.instantiateStreaming(fetch('./lifegame.wasm'))
  .then(({ instance }) => {
    const canvas  = document.getElementById('canvas');
    const context = canvas.getContext('2d');

    const checkboxAutorunning = document.getElementById('checkbox-autorunning');

    const buttonStep      = document.getElementById('button-step');
    const buttonBenchmark = document.getElementById('button-benchmark');

    const resultBenchmark = document.getElementById('result-benchmark');

    let animationId = null;
    let autorunning = false;

    let counterForBenchmark = 0;

    const setup = () => {
      autorunning = checkboxAutorunning.checked;

      instance.exports.init(canvas.width, canvas.height);

      for (let y = 0; y < canvas.height; y++) {
        for (let x = 0; x < canvas.width; x++) {
          const d = (Math.random() > 0.5) ? 1 : 0;

          instance.exports.dot(x, y, d);
        }
      }

      draw();
      next();
      run();
    };

    const draw = () => {
      const map   = instance.exports.getmap();
      const size  = 4 * (canvas.width * canvas.height);
      const bytes = new Uint8ClampedArray(instance.exports.memory.buffer, map, size);
      const image = new ImageData(bytes, canvas.width, canvas.height);

      context.putImageData(image, 0, 0);
    };

    const next = () => {
      if (!autorunning) {
        return;
      }

      if (animationId !== null) {
        return;
      }

      animationId = window.requestAnimationFrame(() => {
        cancelAnimationFrame(animationId);
        animationId = null;
        run();
      });
    };

    const life = () => {
      instance.exports.lifegame();
      draw();
      ++counterForBenchmark;
    };

    const run = () => {
      life();
      next();
    };

    checkboxAutorunning.addEventListener('click', () => {
      autorunning = checkboxAutorunning.checked;
      next();
    }, false);

    buttonStep.addEventListener('click', () => {
      if (autorunning) {
        return;
      }

      life();
    }, false);

    buttonBenchmark.addEventListener('click', () => {
      buttonBenchmark.setAttribute('disabled', 'disabled');
      buttonStep.setAttribute('disabled', 'disabled');
      checkboxAutorunning.setAttribute('disabled', 'disabled');
      resultBenchmark.textContent = 'Now Running Benchmark ...';

      counterForBenchmark = 0;
      autorunning         = true;

      run();

      setTimeout(() => {
        buttonBenchmark.removeAttribute('disabled');
        buttonStep.removeAttribute('disabled');
        checkboxAutorunning.removeAttribute('disabled');
        resultBenchmark.textContent = counterForBenchmark.toString(10);

        autorunning = false;
      }, 10 * 1000);
    }, false);

    setup();
  })
  .catch(console.error);
