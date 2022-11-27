var audioContext = null;

const startFmSynth = async (context, analyzer) => {
  await context.audioWorklet.addModule('./js/synth_worklet.js');

  const worklet = new AudioWorkletNode(context,
    "DualFmOscWorklet",
    { outputChannelCount: [2] });

  worklet.connect(analyzer);
};

const startAnalyzer = function (context, destination) {
  // Set up the different audio nodes we will use for the app
  const analyzer = context.createAnalyser();
  analyzer.minDecibels = -80;
  analyzer.maxDecibels = -10;
  analyzer.smoothingTimeConstant = 0.85;
  analyzer.connect(destination);
  return analyzer;
};

const visualize = function (canvas, canvasCtx, analyzer) {
  const WIDTH = canvas.width;
  const HEIGHT = canvas.height;
  analyzer.fftSize = 2048;
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

// We can only create an audio context after we've clicked something on the page.
window.addEventListener('load', async () => {
  const startButton = document.getElementById('start-worklet-button');
  startButton.disabled = false;
  startButton.addEventListener('click', async () => {
    // Worklet --> Analyzer --> Gain Node --> output.
    audioContext = new AudioContext();
    const gainNode = audioContext.createGain();
    gainNode.connect(audioContext.destination);
    const analyzer = startAnalyzer(audioContext, gainNode);
    await startFmSynth(audioContext, analyzer);

    // Visualizer.
    const canvas = document.querySelector(".visualizer");
    const canvasCtx = canvas.getContext("2d");

    // Set up visualization animation frames.
    visualize(canvas, canvasCtx, analyzer);

    audioContext.resume();
    startButton.disabled = true;
    startButton.textContent = 'Playing...';

    // Setup mute button functionality.
    const muteButton = document.getElementById('mute');
    muteButton.disabled = false;
    mute.onclick = () => {
      if (mute.className === "unmuted") {
        gainNode.gain.value = 0.0;
        mute.className = "muted";
        mute.innerHTML = "Unmute";
      } else if (mute.className === "muted") {
        gainNode.gain.value = 1.0;
        mute.className = "unmuted";
        mute.innerHTML = "Mute";
      }
    }

  }, false);
});
