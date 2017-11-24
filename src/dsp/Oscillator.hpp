#pragma once
#include "helper.hpp"

namespace rack {

/**
 * @brief Oscillator base class
 */
    struct BLITOscillator {
        float freq = 440.f; // oscillator frequency
        float pw = 0.5f;    // pulse-width value
        float phase = 0.f;  // current phase
        float incr = 0.f;   // current phase increment for PLL

        float harmonics = 18000.f;
        int N = 0;

        /* currents of waveforms */
        float ramp = 0.f;
        float saw = 0.f;
        float pulse = 0.f;
        float sawtri = 0.f;
        float tri = 0.f;

        /* leaky integrators */
        Integrator int1;
        Integrator int2;
        Integrator int3;


        BLITOscillator();
        ~BLITOscillator();

        /**
         * @brief Proccess next sample for output
         */
        void proccess();


        /**
         * @brief ReCompute states on change
         */
        void invalidate();


        /**
         * @brief Reset oscillator
         */
        void reset();

        /* common getter and setter */
        float getFrequency() const;
        void setFrequency(float freq);
        float getPulseWidth() const;
        void setPulseWidth(float pw);

        float getRampWave() const;
        float getSawWave() const;
        float getPulseWave() const;
        float getSawTriWave() const;
        float getTriangleWave() const;

        float getHarmonics() const;
        void setHarmonics(float harmonics);
    };
}