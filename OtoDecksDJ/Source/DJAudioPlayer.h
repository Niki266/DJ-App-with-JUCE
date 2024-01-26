#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource // DJAudioPlayer is inheriting from AudioSource
{
public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override; // pure virtual functions

    void loadURL(URL audioURL); // load audio file
    void setGain(double gain); // set playback volume
    void setSpeed(double ratio); // set playback speed
    void setPosition(double posInSecs); // set playback position of the audio file
    void setPositionRelative(double pos); // set playback relative position of the audio file; i.e. 0 - 100% of any length audio file

    void start(); // start playback
    void stop(); // stop playback

    double getPositionRelative(); // get the relative position of the playhead; between 0 and 1

    double getLengthInSeconds(); // from transport source


private:

    // The AudioFormatManager interface is in charge of understanding about various audio formats (such as decompression, channels), and how to play the file.
    AudioFormatManager& formatManager;

    std::unique_ptr<AudioFormatReaderSource> readerSource; // to read a file after it has been indentified and to create a file on demand.

    AudioTransportSource transportSource;

    ResamplingAudioSource resampleSource{ &transportSource, false, 2 }; // wrapper to transport source to control the speed


};


