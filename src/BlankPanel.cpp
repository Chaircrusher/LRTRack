#include "BlankPanel.hpp"
#include "LindenbergResearch.hpp"

struct BlankPanel : Module {
    enum ParamIds {
        NUM_PARAMS
    };
    enum InputIds {
        NUM_INPUTS
    };
    enum OutputIds {
        NUM_OUTPUTS
    };
    enum LightIds {
        NUM_LIGHTS
    };

    BlankPanel() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS) {}

    void step() override;
};

void BlankPanel::step() {
}


BlankPanelWidget::BlankPanelWidget() {
    BlankPanel *module = new BlankPanel();

    setModule(module);
    box.size = Vec(MODULE_WIDTH * RACK_GRID_WIDTH, RACK_GRID_HEIGHT);

    {
        SVGPanel *panel = new SVGPanel();
        panel->box.size = box.size;
        panel->setBackground(SVG::load(assetPlugin(plugin, "res/BlankPanel.svg")));
        addChild(panel);
    }

    // ***** SCREWS **********
    addChild(createScrew<ScrewDarkA>(Vec(15, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 2)));
    addChild(createScrew<ScrewDarkA>(Vec(15, 365)));
    addChild(createScrew<ScrewDarkA>(Vec(box.size.x - 30, 365)));
    // ***** SCREWS **********
}
