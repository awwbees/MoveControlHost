#pragma once

#include <juce_gui_extra/juce_gui_extra.h>

class MainComponent final : public juce::Component, private juce::Timer
{
public:
    MainComponent(juce::DocumentWindow* window);

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)

    bool Initialize();
    void DrawToLCD();

    std::string mOutputText{};
    juce::DocumentWindow* mWindow{ nullptr };
    int mScroll{ 0 };
};
