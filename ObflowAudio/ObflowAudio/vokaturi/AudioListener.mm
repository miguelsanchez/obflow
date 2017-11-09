/*
 * AudioListener.mm
 *
 * Copyright (C) 2016,2017 Vokaturi
 * version 2017-06-06
 *
 * This code is part of the Vokamono demo app.
 * It uses modified parts of Apple's aurioTouch demo software.
 *
 * You can freely adapt this code for your own software.
 * It comes with no warranty of any kind.
 */

#import "SharedBuffer.h"
#import "Vokaturi.h"
#import "AudioListener.h"
#import "AudioController.h"

/*
	The timer fires every 30 or 100 or 500 milliseconds.
*/
#define UPDATE_INTERVAL 0.500

@interface AudioListener ()

@property (nonatomic, copy)   AudioDataAvailable availableAudioDataBlock;
@property (nonatomic, strong) AudioController   *audioController;
@property (nonatomic)         NSTimer *fetchTimer;
@end

@implementation AudioListener

- (AudioController *) audioController
{
    if (_audioController == nil) {
        _audioController = [[AudioController alloc] init];
    }
    return _audioController;
}

-(NSTimer *)fetchTimer
{
    if (_fetchTimer == nil) {
        _fetchTimer = [NSTimer
			scheduledTimerWithTimeInterval: UPDATE_INTERVAL
			target: self
			selector: @selector (readData)
			userInfo:nil
			repeats: YES];
    }
    return _fetchTimer;
}

- (instancetype) initWithAudioDataAvailableBlock: (AudioDataAvailable) audioDataBlock
{
    self = [super init];
    if (self) {
        {
            self.availableAudioDataBlock = audioDataBlock;
        }
    }
    return self;
}

- (void)startTimer
{
    NSLog(@"Starting to read buffer %@", self.fetchTimer);
}

- (void)stopTimer
{
    [self.fetchTimer invalidate];
    self.fetchTimer = nil;
}

- (void)startListening
{
    [self startTimer];
    [self.audioController startIOUnit];
}


- (void)stopListening
{
    [self stopTimer];
    [self.audioController stopIOUnit];
}

- (void)readData
{
	static VokaturiVoice theVoice;
	if (! theVoice) {
		theVoice = VokaturiVoice_create (44100.0, 441000);
    
    /*
     double neutrality;
     double happiness;
     double sadness;
     double anger;
     double fear;
     */
    
		VokaturiEmotionProbabilities priorEmotionProbabilities = { 3.0, 3.0, 0.1, 0.3, 0.3 };
		VokaturiVoice_setRelativePriorProbabilities (theVoice, & priorEmotionProbabilities);
		if (! theVoice)
			return;
	}
	int64_t numberOfReceivedSamples = OSAtomicAdd64Barrier (0, & theSharedBuffer. numberOfReceivedSamples);
//	fprintf (stderr, "samples received %ld sent %ld\n", (long) numberOfReceivedSamples, (long) theSharedBuffer. numberOfSentSamples);
	if (numberOfReceivedSamples == 0)
		return;   // nothing recorded yet
	if (numberOfReceivedSamples > theSharedBuffer. numberOfSentSamples) {
		for (int64_t isamp = theSharedBuffer. numberOfSentSamples; isamp < numberOfReceivedSamples; isamp ++) {
			int32_t indexOfSampleToBeSent = (int32_t) (isamp % SHARED_BUFFER_SIZE);
			VokaturiVoice_fill (theVoice, 1, & theSharedBuffer.samples [indexOfSampleToBeSent]);
		}
		theSharedBuffer. numberOfSentSamples = numberOfReceivedSamples;
		static VokaturiQuality quality;
		static VokaturiEmotionProbabilities emotionProbabilities;
		VokaturiVoice_extract (theVoice, & quality, & emotionProbabilities);
		if (quality.valid) {
			self.availableAudioDataBlock (
				quality.num_frames_analyzed,
				emotionProbabilities.neutrality,
				emotionProbabilities.happiness,
				emotionProbabilities.sadness,
				emotionProbabilities.anger,
				emotionProbabilities.fear
			);
      
      printf("\n\nneutrality: %f", emotionProbabilities.neutrality);
      printf("\nhappiness: %f", emotionProbabilities.happiness);
      printf("\nsadness: %f", emotionProbabilities.sadness);
      printf("\nanger: %f", emotionProbabilities.anger);
      printf("\nfear: %f", emotionProbabilities.fear);
		}
	}
}


@end
