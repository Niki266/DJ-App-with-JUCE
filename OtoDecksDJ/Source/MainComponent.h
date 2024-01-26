#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================

// The built-in component is descended from the component class (GUI) and audio class, the main component is descended from audioAppComponent.

class MainComponent : public AudioAppComponent

{
public:
    MainComponent();
    ~MainComponent() override;

   

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override; // pure virtual functions

    void paint(Graphics& g) override; // not pure virtual function, do not need to provide implementation
    void resized() override; // not pure virtual function, do not need to provide implementation


private:
    //since the main component can see, access, and send data to both decks while preventing the deck GUIs from seeing one another
    // until it reaches the audio thumbnail, take data from the main component and send it through each layer.
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{ 100 }; // cache 100 files

    FileChooser fChooser{ "Select a file..." };

    DJAudioPlayer player1{formatManager}; // set player1 object of DJAudioPlayer class
    DJAudioPlayer player2{formatManager}; // set player2 object of DJAudioPlayer class
    DJAudioPlayer playerForParsingMetaData{ formatManager };
    DeckGUI deckGUI1{&player1, formatManager, thumbCache};
    // DeckGUI with pointers to AudioFormatManager and AudioThumbnailCache from below, as well as DJAudioPlayer player1
    DeckGUI deckGUI2{&player2, formatManager, thumbCache};

    //obtained the first layer, acquired the data in the primary component, made the connection between the two, and DeckGUI was prepared to accept the data.

    MixerAudioSource mixerSource;

    PlaylistComponent playlistComponent { &deckGUI1, &deckGUI2, &playerForParsingMetaData };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};


