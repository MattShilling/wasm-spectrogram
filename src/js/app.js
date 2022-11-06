const audioContext = new AudioContext();

const startAudio = async (context) => {
  await context.audioWorklet.addModule('./worklets/synth_worklet.js');
  const worklet = new AudioWorkletNode(context, "DualFmOscWorklet", { outputChannelCount: [2] });
  worklet.connect(context.destination);
};

// A simplem onLoad handler. It also handles user gesture to unlock the audio
// playback.
window.addEventListener('load', async () => {
  const startButton = document.getElementById('start-worklet-button');
  startButton.disabled = false;
  startButton.addEventListener('click', async () => {
    await startAudio(audioContext);
    audioContext.resume();
    startButton.disabled = true;
    startButton.textContent = 'Playing...';
  }, false);
});
