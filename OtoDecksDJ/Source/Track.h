#pragma once
#include <JuceHeader.h>

class Track
{
public:
    Track(File _file);
    File file;
    URL URL;
    String title;
    String length;
    
    bool operator==(const juce::String& other) const; // files are compared by title
};


