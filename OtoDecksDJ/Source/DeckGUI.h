#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h" // to know about this class
#include "WaveformDisplay.h"

//==============================================================================
class DeckGUI  : public Component,
                 public Button::Listener,
                 public Slider::Listener,
                 public FileDragAndDropTarget,
                 public Timer

{
public:
    DeckGUI(DJAudioPlayer* player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
    // DeckGUI uses the assignment list to get the DJ audio device from the constructor.
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked(Button*) override; // function to implement Button::Listener; called when button clicked

    void sliderValueChanged(Slider* slider) override; // function to implement Slider::Listener

    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& files, int, int y) override;

    void timerCallback() override;
    void loadFile(URL audioURL);

private:

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };
    ToggleButton loopButton{ "LOOP" };

    Slider volumeSlider;
    Label volumeLabel;
    Slider speedSlider;
    Label speedSliderLabel;
    Slider positionSlider;
    Label positionSliderLabel;

    FileChooser fChooser{ "Select a file..." };

    DJAudioPlayer* player;
    // DeckGUI doesn't know how to transmit events to the DJAudioPlayer player when it receives events from buttons, so it needs a pointer for guidance.

    WaveformDisplay waveformDisplay;
    // introduce an instance of the waveform class; WaveformDisplay is contained within DeckGUI, which is contained within MainComponent.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};



