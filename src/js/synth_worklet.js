//@format

// import '../synth.js';

class DualFmOscWorklet extends AudioWorkletProcessor {
    constructor() {
        super();
        this.kernel = Module();
        this.dualFmOsc = new this.kernel.DualFmOsc();

        const numSamples = 32;
        this.audioInBuffer = new Array(32);
        this.audioOutBuffer = new Array(32);
        for (let i = 0; i < numSamples; i++) {
            this.audioInBuffer[i] = new this.kernel.AudioFrame();
            this.audioInBuffer[i].chan_one = 5;
            console.log(this.audioInBuffer[i].chan_one);
            this.audioOutBuffer[i] = new this.kernel.AudioFrame();
        }

        console.log("Worklet launched successfully");
    }
  
    process(inputs, outputs, parameters) {
        // Copy worklet inputs to audio input buffer.
        if (inputs[0].length >= 1) {
            for (let i = 0; i < this.audioInBuffer.length; i++) {
                this.audioInBuffer[i].chan_one = inputs[0][0][i];
                this.audioInBuffer[i].chan_two = inputs[0][1][i];
            }
        }

        // Run audio process.
        this.dualFmOsc.process(this.audioInBuffer, this.audioOutBuffer);

        // Copy audio output buffer to worklet outputs.
        for (let i = 0; i < this.audioOutBuffer.length; i++) {
            outputs[0][0][i] = this.kernel.AudioFrame().scaleInput(this.audioOutBuffer[i].chan_one);
            outputs[0][1][i] = this.kernel.AudioFrame().scaleInput(this.audioOutBuffer[i].chan_two);
        }

        return true;
    }
  }
  
  registerProcessor("DualFmOscWorklet", DualFmOscWorklet);
  