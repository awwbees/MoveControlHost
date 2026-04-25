#include "MainComponent.h"
#include "move/JuceToMoveDisplayBridge.h"
#include "move/../../Move-Display.h"

namespace
{
    ableton::MoveDisplayBridge ThePushBridge; // The bridge allowing to use juce::graphics for push
}

MainComponent::MainComponent(juce::DocumentWindow* window)
{
    mWindow = window;

    setSize (200, 100);

    mOutputText = "initializing...";

    Initialize();
}

bool MainComponent::Initialize()
{
    if (!ThePushBridge.IsInitialized())
    {
        if (auto result = ThePushBridge.Init(); result.Failed())
        {
            mOutputText = result.GetDescription();
            return false;
        }
        mOutputText = "ableton move connected";

        DrawToLCD();

        startTimerHz(6);
    }

    return true;
}

void MainComponent::DrawToLCD()
{
    constexpr int kMoveDisplayWidth = 128;
    constexpr int kMoveDisplayHeight = 64;
    constexpr int kCheckerboardSize = 8;

    unsigned char* pixels = ThePushBridge.GetDisplay()->GetRawBitmap();
    //memset(pixels, 0, sizeof(uint16_t) * kMoveDisplayWidth * kMoveDisplayHeight * 4);
    constexpr int kPixelBlockRows = 8;
    constexpr int kPixelBlockColumns = 128;
    constexpr int kPixelBlockCellHeight = 8;
    for (int row = 0; row < kPixelBlockRows; ++row)
    {
        for (int col = 0; col < kPixelBlockColumns; ++col)
        {
            int cellIndex = col + row * kPixelBlockColumns;
            pixels[cellIndex] = 0;
            for (int i = 0; i < kPixelBlockCellHeight; ++i)
            {
                int pixelX = col;
                int pixelY = row * kPixelBlockCellHeight + i;
                pixelY += mScroll;
                if ((pixelX / kCheckerboardSize) % 2 != (pixelY / kCheckerboardSize) % 2)
                    pixels[cellIndex] |= 1 << i;
            }
        }
    }

    mScroll = (mScroll + 1) % (kCheckerboardSize * 2);
}

void MainComponent::timerCallback()
{
    DrawToLCD();

    ThePushBridge.GetDisplay()->SendBitmapToDevice();
}


//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::FontOptions (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText (mOutputText, getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
}
