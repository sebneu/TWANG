// ---------------------------------
// -------------- SFX --------------
// ---------------------------------

#include "toneAC.h"

/*
   This is used sweep across (up or down) a frequency range for a specified duration.
   A sin based warble is added to the frequency. This function is meant to be called
   on each frame to adjust the frequency in sync with an animation

   duration 	= over what time period is this mapped
   elapsedTime 	= how far into the duration are we in
   freqStart 	= the beginning frequency
   freqEnd 		= the ending frequency
   warble 		= the amount of warble added (0 disables)


*/
void SFXFreqSweepWarble(int duration, int elapsedTime, int freqStart, int freqEnd, int warble)
{
	int freq = map_constrain(elapsedTime, 0, duration, freqStart, freqEnd);
	if (warble)
			warble = map(sin(current_time.milliseconds()/20.0)*1000.0, -1000, 1000, 0, warble);

	toneAC(freq + warble, user_settings.audio_volume);
}

/*

   This is used sweep across (up or down) a frequency range for a specified duration.
   Random noise is optionally added to the frequency. This function is meant to be called
   on each frame to adjust the frequency in sync with an animation

   duration 	= over what time period is this mapped
   elapsedTime 	= how far into the duration are we in
   freqStart 	= the beginning frequency
   freqEnd 		= the ending frequency
   noiseFactor 	= the amount of noise to added/subtracted (0 disables)


*/
void SFXFreqSweepNoise(int duration, int elapsedTime, int freqStart, int freqEnd, uint8_t noiseFactor)
{
	int freq;

	if (elapsedTime > duration)
		freq = freqEnd;
	else
		freq = map(elapsedTime, 0, duration, freqStart, freqEnd);

	if (noiseFactor)
			noiseFactor = noiseFactor - random8(noiseFactor / 2);

	toneAC(freq + noiseFactor, user_settings.audio_volume);
}


void SFXtilt(int amount){
    int f = map(abs(amount), 0, 90, 80, 900)+random8(100);
    if(playerPositionModifier < 0) f -= 500;
    if(playerPositionModifier > 0) f += 200;
    toneAC(f, min(min(abs(amount)/9, 5), user_settings.audio_volume));

}
void SFXattacking(){
    int freq = map(sin(current_time.milliseconds()/2.0)*1000.0, -1000, 1000, 500, 600);
    if(random8(5)== 0){
      freq *= 3;
    }
    toneAC(freq, user_settings.audio_volume);
}
void SFXdead(){
	SFXFreqSweepNoise(1000, current_time.milliseconds()-killTime, 1000, 10, 200);
}

void SFXgameover(){
	SFXFreqSweepWarble(GAMEOVER_SPREAD_DURATION, current_time.milliseconds()-killTime, 440, 20, 60);
}

void SFXkill(){
    toneAC(2000, user_settings.audio_volume, 1000, true);
}
void SFXwin(){
	SFXFreqSweepWarble(WIN_OFF_DURATION, current_time.milliseconds()-stageStartTime, 40, 400, 20);
}

void SFXbosskilled()
{
	SFXFreqSweepWarble(7000, current_time.milliseconds()-stageStartTime, 75, 1100, 60);
}

void SFXcomplete(){
    noToneAC();
}

/*
	This works just like the map function except x is constrained to the range of in_min and in_max
*/
long map_constrain(long x, long in_min, long in_max, long out_min, long out_max)
{
	// constrain the x value to be between in_min and in_max
	if (in_max > in_min){   // map allows min to be larger than max, but constrain does not
		x = constrain(x, in_min, in_max);
	}
	else {
		x = constrain(x, in_max, in_min);
	}

	return map(x, in_min, in_max, out_min, out_max);
}
