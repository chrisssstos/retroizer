# Retroizer Audio Plugin

Retroizer is a digital audio effect plugin that creates lo-fi, retro sound effects by combining bit crushing and radio-style filtering. This plugin is built using the JUCE framework and provides a simple, intuitive interface for manipulating audio in real-time.

## Features

### Bit Crusher
- **Bit Depth**: Reduces the bit resolution of the audio, creating digital distortion and quantization noise reminiscent of early digital equipment.
- **Sample Rate Reduction**: Decreases the effective sample rate, emulating the sound of vintage samplers and early digital audio devices.

### Radio Effect
- **Radio Mix 1**: Applies a bandpass filter centered around 800 Hz to create a telephone/radio tone.
- **Radio Mix 2**: Applies a secondary bandpass filter centered around 1200 Hz for additional radio characteristics.

### Plugin Structure

- Built using the standard JUCE plugin architecture
- Parameter state is saved and recalled properly
- Supports stereo processing
- Minimal CPU usage

## Building the Plugin

### Prerequisites
- JUCE framework
- C++ compiler compatible with your platform (Visual Studio, Xcode, GCC, etc.)

### Build Steps

1. Clone or download the repository
2. Open the project in your IDE (Projucer project file or CMake)
3. Build the project for your target platforms (VST3, AU, AAX, etc.)

## Installation

Copy the built plugin files to your system's VST/AU plugin folders:

- **Windows**: Copy VST3 files to `C:\Program Files\Common Files\VST3`
- **macOS**: Copy VST3 files to `/Library/Audio/Plug-Ins/VST3` or AU files to `/Library/Audio/Plug-Ins/Components`
- **Linux**: Copy VST3 files to `~/.vst3`

## GUI
The official GUI is not available in this repo, message me for more details

![Retroizer2](/images/retroizercpp.png)

![Retroizer](/images/retroplug.png)

## Credits

Developed by Christos Constantinou

Built with the JUCE framework (https://juce.com)

