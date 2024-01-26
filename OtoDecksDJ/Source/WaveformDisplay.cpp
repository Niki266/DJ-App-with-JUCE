#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager& formatManagerToUse,
                                 AudioThumbnailCache& cacheToUse) : audioThumb(1000,
                                 formatManagerToUse, cacheToUse),
                                 fileLoaded(false), position(0)

{
    audioThumb.addChangeListener(this); // register with audioThumbnail for changes
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll(juce::Colour{ 220, 220, 220 });

    g.setColour (juce::Colours::gold);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::red);
    if (fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.5f); // g from paint function to draw on canvas within bounds of waveform compononent (from DeckGUI::resized function)
        // from 0 to end of audio file
        g.setColour(juce::Colours::lightgreen);
        g.drawRect(position * getWidth(), 0, 5, getHeight());
        g.fillRect(position * getWidth(), 0, 5, getHeight());
    }
    else
    {
        // g.setFont (30.0f);
        g.setFont(juce::Font(20.0f, juce::Font::italic));
        g.drawText("Load a file...", getLocalBounds(), juce::Justification::centred, true);   // draw some placeholder text
    }
}

void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(URL audioURL) // When the file is complete, DeckGUI must notify WaveformDisplay via the loadButton.

{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL)); // obtaining URL; decompressing URL; creating a new input source by executing URLInputSource; and setting source to audioThumb
    // A boolean that indicates the file or stream that contains the audio file, AudioThumbnail::setSource returns True is a legitimate music source
    // need to call paint to draw the waveform after the file is loaded
    if (fileLoaded)
    {
        DBG("Waveform loaded!");
    }
    else
    {
        DBG("WFD: not loaded....");
    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (pos != position && pos > 0)
    {
        position = pos;
        repaint();
    }
}


