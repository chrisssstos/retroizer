# Retroizer Audio Plugin

Retroizer is a digital audio effect plugin that creates lo-fi, retro sound effects by combining bit crushing and radio-style filtering. This plugin is built using the JUCE framework and provides a simple, intuitive interface for manipulating audio in real-time.

## Features

### Bit Crusher
- **Bit Depth**: Reduces the bit resolution of the audio, creating digital distortion and quantization noise reminiscent of early digital equipment.
- **Sample Rate Reduction**: Decreases the effective sample rate, emulating the sound of vintage samplers and early digital audio devices.

### Radio Effect
- **Radio Mix 1**: Applies a bandpass filter centered around 800 Hz to create a telephone/radio tone.
- **Radio Mix 2**: Applies a secondary bandpass filter centered around 1200 Hz for additional radio characteristics.

## User Interface

The plugin features a clean, modern interface with clearly labeled controls:
- Top section displays the plugin name "RETROIZER"
- Left side contains the Bit Crusher controls (orange theme)
- Right side contains the Radio Effect controls (blue theme)
- All parameters use intuitive rotary knobs with value displays

## Technical Implementation

### DSP Components

1. **BitCrusher**
   - Reduces bit depth from 1-16 bits
   - Provides sample rate reduction using a sample-and-hold approach
   - Parameters scale appropriately for musical results

2. **RadioEffect**
   - Uses JUCE DSP IIR filters to create bandpass filtering
   - First filter centered at 800 Hz with Q of 0.5
   - Second filter centered at 1200 Hz with Q of 0.7
   - Provides mix controls for both filters

### Plugin Structure

- Built using the standard JUCE plugin architecture
- Parameter state is saved and recalled properly
- Supports stereo processing
- Minimal CPU usage

## Building the Plugin

### Prerequisites
- JUCE framework
- C++ compiler compatible with your platform (Visual Studio, Xcode, GCC, etc.)
- CMake (optional, depending on your build system)

### Build Steps

1. Clone or download the repository
2. Open the project in your IDE (Projucer project file or CMake)
3. Build the project for your target platforms (VST3, AU, AAX, etc.)

## Installation

Copy the built plugin files to your system's VST/AU plugin folders:

- **Windows**: Copy VST3 files to `C:\Program Files\Common Files\VST3`
- **macOS**: Copy VST3 files to `/Library/Audio/Plug-Ins/VST3` or AU files to `/Library/Audio/Plug-Ins/Components`
- **Linux**: Copy VST3 files to `~/.vst3`

## Usage Tips

- For classic 8-bit sounds, try setting the Bit Depth to around 0.5
- For AM radio simulation, combine moderate bit reduction with both radio filters
- For telephone effects, use Radio Mix 1 with a bit of sample rate reduction
- For old video game sounds, use heavy bit depth reduction and sample rate reduction with no radio effect

## License

This plugin is provided under [Your License Here]. See the LICENSE file for details.

## Credits

Developed by [Your Name/Team]

Built with the JUCE framework (https://juce.com)