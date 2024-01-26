#include "MainComponent.h"
#include <JuceHeader.h>

//==============================================================================
MainComponent::MainComponent()
{
    setSize(800, 600);

    // To establish input channels on some sites, submit a permission request here.
    if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio)
        && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request(RuntimePermissions::recordAudio,
            [&](bool granted) { setAudioChannels(granted ? 2 : 0, 2); });
    }
    else
    {
        //Indicate how many input and output channels you want to enable.
        setAudioChannels(2, 2);
    }

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);

    addAndMakeVisible(playlistComponent);

    formatManager.registerBasicFormats(); // for the AudioFormatManager that is now in the WaveformDisplay
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // When the audio device is turned on or when its parameters (such as sample rate, block size, etc.), are modified, this function is called.

    // This function will be called on the audio thread, not the GUI thread.
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    
    // Since we are not currently generating any data, we must clear the buffer to avoid the output of random noise.
    mixerSource.getNextAudioBlock(bufferToFill);
    
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being restarted due to a setting change.
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
    
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll(Colour{ 62, 95, 138 } );
    g.setColour(Colours::navajowhite);
    g.setFont(30.0f);
    g.drawText("", getLocalBounds(),
    Justification::centred, true);
    
}

void MainComponent::resized()
{
    DBG("MainComponent::resized");
    
    deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2); // getWidth() / 2 to set two decks
    deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2); // getWidth() / 2 to set two decks
    playlistComponent.setBounds(5, getHeight() / 2, getWidth() - 10, getHeight() / 2 - 10);

}

