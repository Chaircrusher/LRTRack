#include "DSPMath.hpp"
#include "Oscillator.hpp"

using namespace rack;


/**
 * @brief Set oscillator state back
 */
void BLITOscillator::reset() {
    freq = 0.f;
    pw = 1.f;
    phase = 0.f;
    incr = 0.f;
    saturate = 1.f;
    detune = rand.nextFloat(-0.281273f, 0.2912846f);
    drift = 0.f;
    warmup = 0.f;

    boost = false;

    saw = 0.f;
    ramp = 0.f;
    pulse = 0.f;
    sawtri = 0.f;
    tri = 0.f;

    saturate = 1.f;
    n = 0;

    _cv = 0.f;
    _oct = 0.f;

    _base = 1.f;
    _coeff = 1.f;
    _tune = 0.f;
    _biqufm = 0.f;

    /* force recalculation of variables */
    setFrequency(NOTE_C4);
}


/**
 * @brief Default constructor
 */
BLITOscillator::BLITOscillator() {
    reset();
}


/**
 * @brief Default destructor
 */
BLITOscillator::~BLITOscillator() {}


/**
 * @brief Get current frequency
 * @return
 */
float BLITOscillator::getFrequency() const {
    return freq;
}


/**
 * @brief Set frequency
 * @param freq
 */
void BLITOscillator::setFrequency(float freq) {
    /* just set if frequency differs from old value */
    if (BLITOscillator::freq != freq) {
        BLITOscillator::freq = freq;

        /* force recalculation of variables */
        invalidate();
    }
}


/**
 * @brief Get current pulse-width
 * @return
 */
float BLITOscillator::getPulseWidth() const {
    return pw;
}


/**
 * @brief Set current pulse-width
 * @param pw
 */
void BLITOscillator::setPulseWidth(float pw) {
    if (pw < 0.1f) {
        BLITOscillator::pw = 0.1f;
        return;
    }

    if (pw > 1.f) {
        BLITOscillator::pw = 1.f;
        return;
    }

    BLITOscillator::pw = pw;

    /* force recalculation of variables */
    invalidate();
}


/**
 * @brief Ramp waveform current
 * @return
 */
float BLITOscillator::getRampWave() const {
    return ramp;
}


/**
 * @brief Saw waveform current
 * @return
 */
float BLITOscillator::getSawWave() const {
    return saw;
}


/**
 * @brief Pulse waveform current
 * @return
 */
float BLITOscillator::getPulseWave() const {
    return pulse;
}


/**
 * @brief SawTri waveform current
 * @return
 */
float BLITOscillator::getSawTriWave() const {
    return sawtri;
}


/**
 * @brief Triangle waveform current
 * @return
 */
float BLITOscillator::getTriangleWave() const {
    return tri;
}


/**
 * @brief Process band-limited oscillator
 */
void BLITOscillator::proccess() {
    /* phase locked loop */
    phase = wrapTWOPI(incr + phase);

    /* pulse width */
    float w = pw * (float) M_PI;

    /* get impulse train */
    float blit1 = BLIT(n, phase);
    float blit2 = BLIT(n, wrapTWOPI(w + phase));

    /* feed integrator */
    int1.add(blit1, incr);
    int2.add(blit2, incr);

    /* integrator delta */
    float delta = int1.value - int2.value;

    /* 3rd integrator */
    float beta = int3.add(delta, incr) * 5.f;

    /* compute RAMP waveform */
    ramp = int1.value; //lp1.filter(int1.value);
    /* compute pulse waveform */
    pulse = delta * 1.6f;
    /* compute SAW waveform */
    saw = ramp * -1;

    /* compute triangle */
    tri = (float) M_PI / w * beta;
    /* compute sawtri */
    sawtri = saw + beta;

    //TODO: warmup oscillator with: y(x)=1-e^-(x/n) and slope

    /* adjust output levels */
    ramp *= 3;
    saw *= 3;

    dcb1.filter(saw);
    dcb2.filter(pulse);

    /* reshape waveforms */
    saw = shape1(OSC_SHAPING, saw);
    pulse = shape1(OSC_SHAPING, pulse);
    sawtri = shape1(OSC_SHAPING, sawtri);
    tri = shape1(OSC_SHAPING, tri);
}


/**
 * @brief ReCompute basic parameter
 */
void BLITOscillator::invalidate() {
    incr = getPhaseIncrement(freq);
    n = (int) floorf(BLIT_HARMONICS / freq);
}


/**
 * @brief Get saturation
 * @return
 */
float BLITOscillator::getSaturate() const {
    return saturate;
}


/**
 * @brief Set saturation
 * @param saturate
 */
void BLITOscillator::setSaturate(float saturate) {
    BLITOscillator::saturate = saturate;

    /* force recalculation of variables */
    invalidate();
}


/**
 * @brief Translate from control voltage to frequency
 * @param cv ControlVoltage from MIDI2CV
 * @param fm Frequency modulation
 * @param oct Octave
 */
void BLITOscillator::updatePitch(float cv, float fm, float tune, float oct) {
    // CV is at 1V/OCt, C0 = 16.3516Hz, C4 = 261.626Hz
    // 10.3V = 20614.33hz

    /* optimize the usage of expensive exp function and other computations */
    float coeff = (_oct != oct) ? powf(2.f, oct) : _coeff;
    float base = (_cv != cv) ? powf(2.f, cv) : _base;
    float biqufm = (_tune != tune) ? quadraticBipolar(tune) : _biqufm;

    setFrequency((NOTE_C4 + biqufm) * base * coeff + detune + fm);

    /* save states */
    _cv = cv;
    _oct = oct;
    _base = base;
    _coeff = coeff;
    _tune = tune;
    _biqufm = biqufm;
}