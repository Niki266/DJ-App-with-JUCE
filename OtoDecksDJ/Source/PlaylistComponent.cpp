#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _playerForParsingMetaData) : deckGUI1(_deckGUI1), deckGUI2(_deckGUI2),playerForParsingMetaData(_playerForParsingMetaData)

{
    //The import tracks button serves as the foundation for the music library, which also includes three headers for track titles, music length, and a delete button for library management.The add to left deck and add to right deck buttons remain at the foot of the table list box, and the search field section is located directly beneath the file list.
    addAndMakeVisible(tableComponent);

    addAndMakeVisible(importButton);
    addAndMakeVisible(searchField);
    addAndMakeVisible(library);
    addAndMakeVisible(addToPlayer1Button);
    addAndMakeVisible(addToPlayer2Button);

    importButton.addListener(this);
    searchField.addListener(this);
    addToPlayer1Button.addListener(this);
    addToPlayer2Button.addListener(this);
    //setup search field
    searchField.setTextToShowWhenEmpty("Search tracks...", juce::Colours::orangered);
    searchField.onReturnKey = [this] { searchLibrary(searchField.getText()); };

    // setup table and load library from file
    library.getHeader().addColumn("Title", 1, 1);
    library.getHeader().addColumn("Length", 2, 1);
    library.getHeader().addColumn("Delete", 3, 1);
    library.setModel(this);
    loadLibrary();
}

PlaylistComponent::~PlaylistComponent()
{
    saveLibrary();
}

void PlaylistComponent::paint (Graphics& g)
{
    g.fillAll(juce::Colours::lightblue);

    g.setColour (juce::Colours::gold);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orangered);
    g.setFont (30.0f);
}

void PlaylistComponent::resized()
{
    importButton.setBounds(0.3 * getWidth(), 5, 0.4 * getWidth(), getHeight() / 10);
    library.setBounds(20, 4 * getHeight() / 30, getWidth() - 40, 20 * getHeight() / 30);
    searchField.setBounds(0, 16 * getHeight() / 20, getWidth(), getHeight() / 12);
    addToPlayer1Button.setBounds(0, 18 * getHeight() / 20, getWidth() / 2, getHeight() / 10);
    addToPlayer2Button.setBounds(getWidth() / 2, 18 * getHeight() / 20, getWidth() / 2, getHeight() / 10);

    //set columns
    library.getHeader().setColumnWidth(1, 4 * getWidth() / 20);
    library.getHeader().setColumnWidth(2, 4 * getWidth() / 20);
    library.getHeader().setColumnWidth(3, 4 * getWidth() / 20);
    library.getHeader().setColumnWidth(4, 4 * getWidth() / 20);
    library.getHeader().setColumnWidth(5, 2 * getWidth() / 20);
}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        //track when highlighted
        g.fillAll(juce::Colours::yellow);
    }
    else
    {
        //tracks color once imported
        g.fillAll(juce::Colours::plum);
    }
}

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    //customization of PlayistComponent
    if (rowNumber < getNumRows())
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].title, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length, 2, 0, width - 4, height, Justification::centredLeft, true);
        }
}

PlaylistComponent::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, PlaylistComponent::Component *existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Delete Track" };

            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colours::darkslategrey);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &importButton)
    {
        //load library
        DBG("Load button clicked");
        importToLibrary();
        library.updateContent();
    }
    else if (button == &addToPlayer1Button)
    {
        DBG("Add to Player 1 clicked");
        loadInPlayer(deckGUI1);
    }
    else if (button == &addToPlayer2Button)
    {
        DBG("Add to Player 2 clicked");
        loadInPlayer(deckGUI2);
    }
    else
    {
        //Delete track
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].title + " removed from Library");
        deleteFromTracks(id);
        library.updateContent();
    }
}

void PlaylistComponent::loadInPlayer(DeckGUI* deckGUI)
{
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].title << " to Player");
        deckGUI->loadFile(tracks[selectedRow].URL);
    }
    else
    {
        AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
            "Add to Deck Information:",
            "Please select a track to add to deck",
            "OK",
            nullptr
        );
    }
}

void PlaylistComponent::importToLibrary()
{
    //initialize file chooser
    FileChooser chooser{ "Select files" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const File& file : chooser.getResults())
        {
            String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track newTrack{ file };
                URL audioURL{ file };
                newTrack.length = getLength(audioURL);
                tracks.push_back(newTrack);
                DBG("loaded file: " << newTrack.title);
            }
            else // display info message
            {
                AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon,
                    "Load information:",
                    fileNameWithoutExtension + " already loaded",
                    "OK",
                    nullptr
                );
            }
        }
    }
}
//isInTracks iterates through the vector from start to finish to determine if the same name appears anywhere in the vector.
bool PlaylistComponent::isInTracks(String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}

void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}
//getLength allows us to reach the load URL and getLengthInSeconds functions in the DJAudioplayer by using the pointer to playerforparsingmetadata.
String PlaylistComponent::getLength(URL audioURL)
{
    playerForParsingMetaData->loadURL(audioURL);
    double seconds{ playerForParsingMetaData->getLengthInSeconds() };
    String minutes{ secondsToMinutes(seconds) };
    return minutes;
}
//we can measure the length of the music in seconds and use the seconds to minutes function to convert the outcome to minutes.
String PlaylistComponent::secondsToMinutes(double seconds)
{
    //find seconds and minutes and make into string
    int secondsRounded{ int(std::round(seconds)) };
    String min{ std::to_string(secondsRounded / 60) };
    String sec{ std::to_string(secondsRounded % 60) };

    if (sec.length() < 2) // if seconds is 1 digit or less
    {
        //add '0' to seconds until seconds is length 2
        sec = sec.paddedLeft('0', 2);
    }
    return String{ min + ":" + sec };
}
//Searching track titles in the audio library
void PlaylistComponent::searchLibrary(String searchText)
{
    DBG("Searching titles for: " << searchText);
    if (searchText != "")
    {
        int rowNumber = whereInTracks(searchText);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

int PlaylistComponent::whereInTracks(String searchText)
{
    // finds index where track title contains searchText
    auto it = find_if(tracks.begin(), tracks.end(),
        [&searchText](const Track& obj) {return obj.title.contains(searchText); });
    int i = -1;

    if (it != tracks.end())
    {
        i = std::distance(tracks.begin(), it);
    }

    return i;
}

void PlaylistComponent::saveLibrary()
{
    // create .csv to save library
    std::ofstream myLibrary("audioLibrary.csv");

    // save library to file
    for (Track& t : tracks)
    {
        myLibrary << t.file.getFullPathName() << "," << t.length << "\n";
    }
}

void PlaylistComponent::loadLibrary()
{
    // create input stream from saved library
    std::ifstream myLibrary("audioLibrary.csv");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (myLibrary.is_open())
    {
        while (getline(myLibrary, filePath, ',')) {
            File file{ filePath };
            Track newTrack{ file };

            getline(myLibrary, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    myLibrary.close();
}


