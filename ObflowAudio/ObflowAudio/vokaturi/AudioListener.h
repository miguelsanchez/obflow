/*
 * AudioListener.h
 *
 * Copyright (C) 2016,2017 Vokaturi
 * version 2017-02-14
 *
 * This code is part of the Vokamono demo app.
 * It uses modified parts of Apple's aurioTouch demo software.
 *
 * You can freely adapt this code for your own software.
 * It comes with no warranty of any kind.
 */

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


typedef void (^AudioDataAvailable) (
	int numberOfFramesAnalyzed,
	double neutralProbability,
	double happyProbability,
	double sadProbability,
	double angryProbability,
	double fearProbability
);

@interface AudioListener : NSObject

- (instancetype) initWithAudioDataAvailableBlock: (AudioDataAvailable) audioDataBlock;

- (void) startListening;
- (void) stopListening;

@end
