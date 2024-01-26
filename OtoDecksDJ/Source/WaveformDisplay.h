#pragma once

#include <JuceHeader.h>

//==============================================================================
class WaveformDisplay  : public Component, public ChangeListener // add ChangeBroadcaster listener to inheritance definition
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse); // constructor with AudioFormatManager and the thumbnail cache
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    void setPositionRelative(double pos); // set the relative position of the playhead

private:

    AudioThumbnail audioThumb;
    // WaveformDisplay must be added to the initialization list in the constructor file because the AudioThumbnail constructor is used to display the waveform portion of audio) only accepts three arguments. audioThumb(1000, formatManagerToUse, cacheToUse) Since the cache is shared by all audio thumbnails, you only need one instance of each AudioFormatManager and AudioThumbnailCache, which will both be a part of the Main Component where data is kept.

    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};


