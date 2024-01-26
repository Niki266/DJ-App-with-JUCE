#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 AudioFormatManager& formatManagerToUse,
                 AudioThumbnailCache& cacheToUse) : player(_player),
                 waveformDisplay(formatManagerToUse,
                 cacheToUse)

// pass _player variable into the player object; so DeckGUI has access to the player in a different way to using the initialization list; instead of ONLY passing an instance of the same type of object and storing it (player), passing the constructor arguments to waveForm through the constructor passing the two arguments when instantiating waveformDisplay object.

{
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(loopButton);

    
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this); // 'this' is a pointer to itself and was derived from the button listener; register with button to receive click events.
    stopButton.addListener(this);
    loadButton.addListener(this);
    loopButton.addListener(this);
    volumeSlider.addListener(this);
    
    //volumeSlider customisation
    volumeSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volumeSlider.setRange(0.0, 1.0);
    volumeSlider.setValue(0.5); //default volume half of max vol
    volumeSlider.setNumDecimalPlacesToDisplay(2);
    volumeSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volumeSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 100,25);
    addAndMakeVisible(volumeLabel);
    volumeLabel.setText("Volume", dontSendNotification);
    volumeLabel.attachToComponent(&volumeSlider, false);
    volumeLabel.setJustificationType(Justification::centred);


    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.addListener(this); // The entity that wants to receive events must tell the object on the GUI that it wants to register for them.
    speedSlider.setRange(0.1, 2.0);
    speedSlider.setValue(1.0);
    speedSlider.setNumDecimalPlacesToDisplay(2);
    speedSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 100, 25);
    addAndMakeVisible(speedSliderLabel);
    speedSliderLabel.setText("Speed", dontSendNotification);
    speedSliderLabel.attachToComponent(&speedSlider, false);

    positionSlider.addListener(this); // The entity that wants to receive events must tell the object on the GUI that it wants to register for them.
    positionSlider.setRange(0.0, 1.0);
    addAndMakeVisible(positionSliderLabel);
    positionSliderLabel.setText("Position", dontSendNotification);
    positionSliderLabel.attachToComponent(&positionSlider, false);

    startTimer(500);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll(Colours::blueviolet);

    g.setColour (Colours::goldenrod );
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child components that your component contains..

    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth()/7, rowH);
    stopButton.setBounds(getWidth() / 7, 0, getWidth() / 7, rowH);
    loopButton.setBounds((getWidth() / 7)*5, 0, getWidth() / 7, rowH);
    loadButton.setBounds((getWidth() / 7)*6, 0, getWidth()/7, rowH);
    
    volumeSlider.setBounds(getWidth() / 3, rowH, getWidth()/5, rowH*3);
    speedSlider.setBounds(getWidth()/2, rowH, getWidth()/2, rowH*3.5);

    volumeLabel.setCentreRelative(0.43f, 0.4f);
    speedSliderLabel.setCentreRelative(0.94f, 0.4f);
    positionSlider.setBounds(0, rowH * 4.5, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5.5, getWidth(), rowH*3);
}

void DeckGUI::buttonClicked(Button* button) // pointer to button; memory address
{
    if (button == &playButton) // button to start audio file play
    {
        DBG("Play button has been clicked!");
        Logger::outputDebugString("Play button!");
        player->setPosition(0); // player calls setPosition(0) function from DJAudioPlayer to start playing from the beginning
        player->start(); // player calls start function from DJAudioPlayer

    }
    else if (button == &stopButton) // button to stop playing the audio file
    {
        DBG("Stop button has been clicked!");
        Logger::outputDebugString("Stop button!");
        player->stop(); // player calls stop function from DJAudioPlayer

    }
    else if (button == &loadButton) // button to load new audio file
    {
        DBG("Load button was clicked ");
        FileChooser chooser{ "Select a file" };
        if (chooser.browseForFileToOpen())
        {
            loadFile(URL{ chooser.getResult() });
        }
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volumeSlider)
    {
        DBG("Volume slider moved: " << slider->getValue());
        player->setGain(slider->getValue()); // player calls setGain function from DJAudioPlayer
    }
    else if (slider == &speedSlider)
    {
        DBG("Speed slider moved: " << slider->getValue());
        player->setSpeed(slider->getValue()); // player calls setSpeed function from DJAudioPlayer
    }
    else if (slider == &positionSlider)
    {
        DBG("Position slider: " << slider->getValue());
        player->setPositionRelative(slider->getValue()); // player calls setPositionRelative function from DJAudioPlayer
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int, int y)
{
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]} }); // player calls loadURL function from DJAudioPlayer
        waveformDisplay.loadURL(URL{ fChooser.getResult() });
    }
}

void DeckGUI::loadFile(URL audioURL)
{
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback() { //Every 100ms, the timer loops, checking to see if the "loop" button is turned on or off and updating the wave form display's current location.
    if (std::to_string(loopButton.getToggleState()) == "1") { //If loop button is checked
        if (player->getPositionRelative() >= 1) {// and the position more than 1, it means it is the end of the audio file
            player->setPositionRelative(0); // Sets position back to the start of the audio file
            player->start(); // Plays the audio file
        }
    }

    if (std::to_string(loopButton.getToggleState()) == "0") { //If loop button is not checked
        if (player->getPositionRelative() >= 1) { //and the position more than 1, it means it is the end of the audio file
            player->setPositionRelative(0); //Sets position back to the start of the audio file
            player->stop(); // Stops the audio file
        }
    }
    waveformDisplay.setPositionRelative(player->getPositionRelative()); //Updates the waveform representation every 100 ms to reflect the audio file's current timestamp.a
}


