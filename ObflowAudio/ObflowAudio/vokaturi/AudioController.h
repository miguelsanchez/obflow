/*
 * AudioController.h
 *
 * Copyright (C) 2016 Vokaturi
 * version 2016-07-06
 *
 * This code is part of the Vokamono demo app.
 * It uses modified parts of Apple's aurioTouch demo software.
 *
 * You can freely adapt this code for your own software.
 * It comes with no warranty of any kind.
 */

#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>


@interface AudioController : NSObject {
    
    AudioUnit               _rioUnit;
    AVAudioPlayer*          _audioPlayer;   // for button pressed sound
    BOOL                    _audioChainIsBeingReconstructed;
}

@property (nonatomic, assign, readonly) BOOL audioChainIsBeingReconstructed;

- (OSStatus)    startIOUnit;
- (OSStatus)    stopIOUnit;
- (double)      sessionSampleRate;

@end
