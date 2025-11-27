#include "MainComponent.h"
#include "push2/JuceToPush2DisplayBridge.h"
#include "push2/Push2-Bitmap.h"
#include "push2/../../Push2-Display.h"

namespace
{
    ableton::Push2DisplayBridge ThePushBridge; // The bridge allowing to use juce::graphics for push
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
        if (auto result = ThePushBridge.Init(ableton::DeviceType::Move); result.Failed())
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

    uint16_t* pixels = ThePushBridge.GetDisplay()->GetRawBitmap();
    //memset(pixels, 0, sizeof(uint16_t) * kMoveDisplayWidth * kMoveDisplayHeight * 4);
    constexpr int kPixelBlockRows = 8;
    constexpr int kPixelBlockColumns = 64;
    constexpr int kPixelBlockCellWidth = 2;
    constexpr int kPixelBlockCellHeight = 8;
    for (int row = 0; row < kPixelBlockRows; ++row)
    {
        for (int col = 0; col < kPixelBlockColumns; ++col)
        {
            int cellIndex = col + row * kPixelBlockColumns;
            pixels[cellIndex] = 0;
            int pixelXStart = col * kPixelBlockCellWidth;
            int pixelYStart = row * kPixelBlockCellHeight;
            for (int i = 0; i < kPixelBlockCellWidth * kPixelBlockCellHeight; ++i)
            {
                int pixelX = pixelXStart + i / kPixelBlockCellHeight;
                int pixelY = pixelYStart + i % kPixelBlockCellHeight;
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
