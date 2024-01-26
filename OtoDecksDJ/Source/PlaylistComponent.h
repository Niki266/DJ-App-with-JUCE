#pragma once

#include <JuceHeader.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include "DeckGUI.h"
#include "DJAudioPlayer.h"
#include "Track.h"

//==============================================================================
class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener // inherit TableListBoxModel, to allow PlayListComponent to behave like a table
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _playerForParsingMetaData);
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    void buttonClicked(Button* button); // function to implement Button::Listener is called when button clicked

private:
    
    TableListBox tableComponent;
    std::vector<Track> tracks;

    TextButton importButton{ "IMPORT AUDIO LIBRARY" };
    TextEditor searchField;
    TableListBox library;
    TextButton addToPlayer1Button{ "ADD TO LEFT DECK" };
    TextButton addToPlayer2Button{ "ADD TO RIGHT DECK" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* playerForParsingMetaData;

    String getLength(URL audioURL);
    String secondsToMinutes(double seconds);

    void importToLibrary();
    void searchLibrary(String searchText);
    void saveLibrary();
    void loadLibrary();
    void deleteFromTracks(int id);
    bool isInTracks(String fileNameWithoutExtension);
    int whereInTracks(String searchText);
    void loadInPlayer(DeckGUI* deckGUI);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};


