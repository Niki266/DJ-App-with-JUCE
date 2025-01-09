# DJ App with JUCE 🎵

Welcome to **OtoDecks**, a fully functional DJ application developed using the JUCE framework. This project uses object oriented programming language C++ for real-time audio processing and user-friendly graphical interfaces, making it perfect for both aspiring DJs and developers interested in audio programming.

![image](https://github.com/user-attachments/assets/ca2dcffa-6d7c-46bd-996b-c60826c22b69)

## How to Use

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/DJ-App-with-JUCE.git
   ```
2. Open the `.jucer` file in [Projucer](https://juce.com/get-juce).
3. Configure the project for your platform (Windows/Mac/Linux).
4. Build the project using your preferred IDE (e.g. Xcode, Visual Studio).
5. Run the application and start mixing your favorite tracks!

## 🚀 Features

- **Dual Decks**:
  - Load and play two tracks simultaneously.
  - Independent playback controls (play, pause, stop) for each deck.
- **Waveform Display**:
  - Visualize audio waveforms in real time.
  - Scrub through tracks with precision.
- **Playlist Management**:
  - Add, remove, and manage tracks.
  - Drag-and-drop support for easy track loading.
- **Audio Controls**:
  - Volume, speed, and position sliders for custom playback.
  - Crossfader for smooth transitions between decks.

## 📂 Project Structure

```
DJ-App-with-JUCE-main
├── OtoDecksDJ
│   ├── OtoDecksDJ.jucer          # JUCE project configuration file
│   └── Source                    # Source code for the application
│       ├── Main.cpp              # Entry point of the application
│       ├── MainComponent.cpp     # Core GUI and logic
│       ├── PlaylistComponent.cpp # Playlist functionality
│       ├── DeckGUI.cpp           # GUI for individual decks
│       ├── DJAudioPlayer.cpp     # Audio playback logic
│       └── WaveformDisplay.cpp   # Audio waveform visualization
├── OOP final project otodecks.pdf # Documentation for the project
├── OOP Otodecks.mp4              # Video demonstration of the app
└── README.md                     # Project documentation
```
## 🛠️ Technology Stack

- **JUCE Framework**: A C++ framework for developing cross-platform audio applications.
- **C++**: Core programming language for implementing the application.
- **Object-Oriented Programming (OOP)**: Modular and reusable code design.
