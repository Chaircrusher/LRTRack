#include "LindenbergResearch.hpp"

using namespace rack;

Plugin *plugin;


/**
 * @brief
 * @param p
 */
void init(rack::Plugin *p) {
    plugin = p;
    p->slug = "Lindenberg Research";
    p->website = "https://github.com/lindenbergresearch/LRTRack";

    p->addModel(createModel<SimpleFilterWidget>("Lindenberg Research", "LPFilter24dB", "24dB Lowpass Filter", FILTER_TAG));
    p->addModel(createModel<BlankPanelWidget>("Lindenberg Research", "BlankPanel", "Blank Panel 20TE", UTILITY_TAG));
    p->addModel(createModel<BlankPanelWidgetM1>("Lindenberg Research", "BlankPanel Mark I", "Blank Panel 12TE", UTILITY_TAG));
    p->addModel(createModel<ReShaperWidget>("Lindenberg Research", "ReShaper", "ReShaper Wavefolder", FILTER_TAG));
    p->addModel(createModel<VCOWidget>("Lindenberg Research", "VCO", "Voltage Controlled Oscillator", OSCILLATOR_TAG));
}


/**
 * @brief Draw method of custom LCD widget
 * @param vg
 */
void LCDWidget::draw(NVGcontext *vg) {
    nvgFontSize(vg, LCD_FONTSIZE);
    nvgFontFaceId(vg, gLCDFont_DIG7->handle);
    nvgTextLetterSpacing(vg, LCD_LETTER_SPACING);

    nvgFillColor(vg, bg);

    std::string s1;
    std::string s2;

    for (int i = 0; i < LCDWidget::length; ++i) {
        s1.append("8");
        s2.append(":");
    }

    nvgTextBox(vg, 0, 0, width, s1.c_str(), nullptr);
    nvgTextBox(vg, 0, 0, width, s2.c_str(), nullptr);

    nvgFillColor(vg, fg);
    nvgTextBox(vg, 0, 0, width, text.c_str(), nullptr);
}


/**
 * @brief Custom step implementation for LRT Modules
 */
void LRTModule::step() {
    Module::step();

    // increment counter
    cnt++;
}


/**
 * @brief
 * @param vg
 */
void LRLightWidget::draw(NVGcontext *vg) {
    float radius = box.size.x / 2.0;
    float oradius = radius + 15.0;

    color.r = clampf(color.r, 0.0, 1.0);
    color.g = clampf(color.g, 0.0, 1.0);
    color.b = clampf(color.b, 0.0, 1.0);
    color.a = clampf(color.a, 0.0, 1.0);

    // Solid
    nvgBeginPath(vg);
    nvgCircle(vg, radius, radius, radius);
    nvgFillColor(vg, bgColor);
    nvgFill(vg);

    // Border
    nvgStrokeWidth(vg, 1.0);
    NVGcolor borderColor = bgColor;
    borderColor.a *= 0.5;
    nvgStrokeColor(vg, borderColor);
    nvgStroke(vg);

    // Inner glow
    nvgGlobalCompositeOperation(vg, NVG_LIGHTER);
    nvgFillColor(vg, color);
    nvgFill(vg);

    // Outer glow
    nvgBeginPath(vg);
    nvgRect(vg, radius - oradius, radius - oradius, 2 * oradius, 2 * oradius);
    NVGpaint paint;
    NVGcolor icol = color;
    icol.a *= 0.10;
    NVGcolor ocol = color;
    ocol.a = 0.0;
    paint = nvgRadialGradient(vg, radius, radius, radius, oradius, icol, ocol);
    nvgFillPaint(vg, paint);
    nvgFill(vg);
}