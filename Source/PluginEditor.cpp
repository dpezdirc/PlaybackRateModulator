#include "PluginProcessor.h"
#include "PluginEditor.h"

//------------------------------------------------------------------------------
PlaybackRateModulatorAudioProcessorEditor::PlaybackRateModulatorAudioProcessorEditor (PlaybackRateModulatorAudioProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(400, 300);
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

//------------------------------------------------------------------------------
void PlaybackRateModulatorAudioProcessorEditor::resized()
{
}
