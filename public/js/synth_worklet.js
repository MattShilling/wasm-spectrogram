//@format
import Module from '../synth.js';

class DualFmOscWorklet extends AudioWorkletProcessor {
    constructor() {
        super();
        Module().then(module => {
            this.kernel = module;
            this.dualFmOsc = new this.kernel.DualFmOsc();
        });

        const numSamples = 32;
        this.audioInBuffer = new Array(32);
        this.audioOutBuffer = new Array(32);
        for (let i = 0; i < numSamples; i++) {
            this.audioInBuffer[i] = {chan: [0,0]};
            this.audioOutBuffer[i] = {chan: [0,0]};
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
        const output = this.dualFmOsc.process(this.audioInBuffer, this.audioOutBuffer);

        // Copy audio output buffer to worklet outputs.
        for (let i = 0; i < this.audioOutBuffer.length; i++) {
            outputs[0][0][i] = this.kernel.scaleInput(output[i].chan[0] * 0.05);
            outputs[0][1][i] = this.kernel.scaleInput(output[i].chan[1] * 0.05);
        }

        return true;
    }
  }
  
  registerProcessor("DualFmOscWorklet", DualFmOscWorklet);
  