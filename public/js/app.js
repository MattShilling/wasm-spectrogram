var audioContext = null;

const startFmSynth = async (context, analyzer) => {
  await context.audioWorklet.addModule('./js/synth_worklet.js');

  const worklet = new AudioWorkletNode(context,
    "DualFmOscWorklet",
    { outputChannelCount: [2] });

  worklet.connect(analyzer);
};

const startAnalyzer = function(context) {
  // Set up the different audio nodes we will use for the app
  const analyzer = context.createAnalyser();
  analyzer.minDecibels = -80;
  analyzer.maxDecibels = -10;
  analyzer.smoothingTimeConstant = 0.85;
  analyzer.connect(context.destination);
  return analyzer;
};

// We can only create an audio context after we've clicked something on the page.
window.addEventListener('load', async () => {
  const startButton = document.getElementById('start-worklet-button');
  startButton.disabled = false;
  startButton.addEventListener('click', async () => {
    audioContext = new AudioContext();
    const analyzer = startAnalyzer(audioContext);
    await startFmSynth(audioContext, analyzer);

    // Visualizer.
    const canvas = document.querySelector(".visualizer");
    const canvasCtx = canvas.getContext("2d");

    function visualize() {
      const WIDTH = canvas.width;
      const HEIGHT = canvas.height;
      analyzer.fftSize = 256;
      const bufferLengthAlt = analyzer.frequencyBinCount;

      // See comment above for Float32Array()
      const dataArrayAlt = new Uint8Array(bufferLengthAlt);

      canvasCtx.clearRect(0, 0, WIDTH, HEIGHT);

      const drawAlt = function () {
        const drawVisual = requestAnimationFrame(drawAlt);

        analyzer.getByteFrequencyData(dataArrayAlt);

        canvasCtx.fillStyle = "rgb(0, 0, 0)";
        canvasCtx.fillRect(0, 0, WIDTH, HEIGHT);

        const barWidth = (WIDTH / bufferLengthAlt) * 2.5;
        let barHeight;
        let x = 0;

        for (let i = 0; i < bufferLengthAlt; i++) {
          barHeight = dataArrayAlt[i];

          canvasCtx.fillStyle = "rgb(" + (barHeight + 100) + ",50,50)";
          canvasCtx.fillRect(
            x,
            HEIGHT - barHeight / 2,
            barWidth,
            barHeight / 2
          );

          x += barWidth + 1;
        }
      };

      drawAlt();
    }

    audioContext.resume();
    startButton.disabled = true;
    startButton.textContent = 'Playing...';

    visualize();

  }, false);
});
