// TODO #include "HardwareSerial.h"
// TODO #include <avr/wdt.h>
#include <iostream>
#include <ctype.h>

// Adruino port
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))


/// Defaults

// change this whenever the saved settings are not compatible with a change
// it force a load from defaults.
#define SETTINGS_VERSION 4

// LED
#define NUM_LEDS        288
#define MIN_LEDS				60
#define MAX_LEDS				1000

#define BRIGHTNESS           150
#define MIN_BRIGHTNESS			10
#define MAX_BRIGHTNESS 			255

// JOYSTICK
#define JOYSTICK_ORIENTATION 1     // 0, 1 or 2 to set the angle of the joystick
#define JOYSTICK_DIRECTION   1     // 0/1 to flip joystick direction

#define ATTACK_THRESHOLD     30000 // The threshold that triggers an attack
#define MIN_ATTACK_THRESHOLD     20000
#define MAX_ATTACK_THRESHOLD     30000

#define JOYSTICK_DEADZONE    7     // Angle to ignore
#define MIN_JOYSTICK_DEADZONE 3
#define MAX_JOYSTICK_DEADZONE 12

// PLAYER
#define MAX_PLAYER_SPEED    10     // Max move speed of the player

#define LIVES_PER_LEVEL		3
#define MIN_LIVES_PER_LEVEL 3
#define MAX_LIVES_PER_LEVEL 9

//AUDIO
#define MAX_VOLUME           10
#define MIN_VOLUME							0
#define MAX_VOLUME							10

enum stripTypes{
	strip_APA102 = 0,
	strip_WS2812 = 1
};

enum ErrorNums{
	ERR_SETTING_NUM,
	ERR_SETTING_RANGE
};

typedef struct {
	uint8_t settings_version; // stores the settings format version

	uint8_t led_type;  // dotstar, neopixel

	uint16_t led_count;
	uint8_t led_brightness;

	uint8_t joystick_deadzone;
	uint16_t attack_threshold;

	uint8_t audio_volume;

	uint8_t lives_per_level;

	// saved statistics
	uint16_t games_played;
	uint32_t total_points;
	uint16_t high_score;
	uint16_t boss_kills;

}settings_t;

settings_t user_settings;

#define READ_BUFFER_LEN 10
#define CARRIAGE_RETURN 13
char readBuffer[READ_BUFFER_LEN];
uint8_t readIndex = 0;

long lastInputTime = 0;

void reset_cpu();
void processSerial(char inChar);
void show_settings_menu();
void reset_settings();
void change_setting(char *line);
void settings_write();
void settings_write();
void printError(int reason);
void show_game_stats();


void reset_cpu()
{
	std::cout << "RESTART; NOT IMPLEMENTED" << '\n';
	//wdt_enable(WDTO_15MS);
  //while(1)
  //{
    // wait for it...boom
  //}
}

void processSerial(char inChar)
{
	readBuffer[readIndex] = inChar;

		switch(readBuffer[readIndex]) {
			case '?':// show settings
				readIndex = 0;
				std::cout << "TWANG VERSION: " << VERSION << '\n';
				show_settings_menu();
				return;
			break;

			case 'R': // reset to defaults
				readIndex = 0;
				reset_settings();
				return;
			break;

			case 'P': // reset play stats
				user_settings.games_played = 0;
				user_settings.total_points = 0;
				user_settings.high_score = 0;
				user_settings.boss_kills = 0;
				settings_write();
				return;
			break;

			case '!': // reboot
				reset_cpu();
			break;

			default:
			break;
		}

		if (readBuffer[readIndex] == CARRIAGE_RETURN) {
			if (readIndex < 3) {
				// not enough characters
				readIndex = 0;
			}
			else {
				readBuffer[readIndex] = 0; // mark it as the end of the string
				change_setting(readBuffer);
				readIndex = 0;
			}
		}
		else if (readIndex >= READ_BUFFER_LEN) {
			readIndex = 0; // too many characters. Reset and try again
		}
		else
			readIndex++;
}

void show_settings_menu() {


	std::cout << "\r\n====== TWANG Settings Menu ========" << '\n';
	std::cout << "=    Current values are shown     =" << '\n';
	std::cout << "=   Send new values like B=150    =" << '\n';
	std::cout << "=     with a carriage return      =" << '\n';
	std::cout << "===================================" << '\n';

	std::cout << "\r\nT=";
	std::cout << user_settings.led_type;
	std::cout << " (LED Type  0=APA102/Dotstar, 1=WS2812/Neopixel ... forces restart)" << '\n';

	std::cout << "C=";
	std::cout << user_settings.led_count;
	std::cout << " (LED Count 100-1000.. forces restart if increased above initial val.)" << '\n';

	std::cout << "B=";
	std::cout << user_settings.led_brightness;
	std::cout << " (LED Brightness 5-255)" << '\n';

	std::cout << "S=";
	std::cout << user_settings.audio_volume;
	std::cout << " (Sound Volume 0-10)" << '\n';

	std::cout << "D=";
	std::cout << user_settings.joystick_deadzone;
	std::cout << " (Joystick Deadzone 3-12)" << '\n';

	std::cout << "A=";
	std::cout << user_settings.attack_threshold;
	std::cout << " (Attack Sensitivity 20000-35000)" << '\n';

	std::cout << "L=";
	std::cout << user_settings.lives_per_level;
	std::cout << " (Lives per Level (3-9))" << '\n';

	std::cout << "\r\n(Send...)";
	std::cout << "  ? to show current settings";
	std::cout << "  R to reset everything to defaults";
	std::cout << "  P to reset play statistics" << '\n';

}

void show_game_stats()
{
	std::cout << "\r\n ===== Play statistics ======" << '\n';
	std::cout << "Games played: ";
	std::cout << user_settings.games_played << '\n';
	if (user_settings.games_played > 0)	{
		std::cout << "Average Score: ";
		std::cout << user_settings.total_points / user_settings.games_played << '\n';
	}
	std::cout << "High Score: ";
	std::cout << user_settings.high_score << '\n';
	std::cout << "Boss kills: ";
	std::cout << user_settings.boss_kills << '\n';
}

void reset_settings() {
	user_settings.settings_version = SETTINGS_VERSION;

	user_settings.led_type = strip_APA102;
	user_settings.led_count = NUM_LEDS;
	user_settings.led_brightness = BRIGHTNESS;

	user_settings.joystick_deadzone = JOYSTICK_DEADZONE;
	user_settings.attack_threshold = ATTACK_THRESHOLD;

	user_settings.audio_volume = MAX_VOLUME;

	user_settings.lives_per_level = LIVES_PER_LEVEL;

	user_settings.games_played = 0;
	user_settings.total_points = 0;
	user_settings.high_score = 0;
	user_settings.boss_kills = 0;

	settings_write();

}

void change_setting(char *line) {

	char setting_val[6];
  char param;
  uint16_t newValue;

	if (readBuffer[1] != '='){  // check if the equals sign is there
		std::cout << "Missing '=' in command" << '\n';
		readIndex = 0;
		return;
  }

	// move the value characters into a char array while verifying they are digits
  for(int i=0; i<5; i++) {
	if (i+2 < readIndex) {
		if (isdigit(readBuffer[i+2]))
			setting_val[i] = readBuffer[i+2];
		else {
			std::cout << "Invalid setting value" << '\n';
			return;

		}
	}
	else
		setting_val[i] = 0;
  }

	param = readBuffer[0];
  newValue = atoi(setting_val); // convert the val section to an integer

	switch (param) {

		CurrentTime current_time;
		lastInputTime = current_time.milliseconds(); // reset screensaver count

		case 'T': // LED Type
				user_settings.led_type = constrain(newValue, strip_APA102, strip_WS2812);
				settings_write();
				reset_cpu();
		break;

		case 'C': // LED Count
				user_settings.led_count = constrain(newValue, MIN_LEDS, MAX_LEDS);
				settings_write();
				if (NUM_LEDS < user_settings.led_count)
					reset_cpu(); // reset required to prevent overrun the
		break;

		case 'B': // brightness
			user_settings.led_brightness = constrain(newValue, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
			led.setBrightness(user_settings.led_brightness);
			settings_write();
		break;

		case 'S': // sound
			user_settings.audio_volume = constrain(newValue, MIN_VOLUME, MAX_VOLUME);
			settings_write();
		break;

		case 'D': // deadzone, joystick
			user_settings.joystick_deadzone = constrain(newValue, MIN_JOYSTICK_DEADZONE, MAX_JOYSTICK_DEADZONE);
			settings_write();
		break;

		case 'A': // attack threshold, joystick
			user_settings.attack_threshold = constrain(newValue, MIN_ATTACK_THRESHOLD, MAX_ATTACK_THRESHOLD);
			settings_write();
		break;

		case 'L': // lives per level
			user_settings.lives_per_level = constrain(newValue, MIN_LIVES_PER_LEVEL, MAX_LIVES_PER_LEVEL);
			settings_write();
		break;

		default:
			std::cout << "Command Error: ";
			std::cout << readBuffer[0] << '\n';
			return;
		break;

  }
	show_settings_menu();

}

void settings_read() {
	return; // TODO
	uint8_t ver = 4; // TODO EEPROM.read(0);
	uint8_t temp[sizeof(user_settings)];
	bool read_fail = false;

	if (ver != SETTINGS_VERSION) {
		std::cout << "Error: Reading  settings failed" << '\n';
		std::cout << "Loading defaults" << '\n';
		reset_settings();
		return;
	}

	for (int i=0; i<sizeof(user_settings); i++)
	{
		// TODO temp[i] = EEPROM.read(i);
	}

	//memcpy((char*)&user_settings, temp, sizeof(user_settings));

	// if any values are out of range, reset them all
	if (user_settings.led_type < strip_APA102 || user_settings.led_type > strip_WS2812)
		read_fail = true;

	if (user_settings.led_count < MIN_LEDS || user_settings.led_count > MAX_LEDS)
		read_fail = true;

	if (user_settings.led_brightness < MIN_BRIGHTNESS || user_settings.led_brightness > MAX_BRIGHTNESS)
		read_fail = true;

	if (user_settings.joystick_deadzone < MIN_JOYSTICK_DEADZONE || user_settings.joystick_deadzone > MAX_JOYSTICK_DEADZONE)
		read_fail = true;

	if (user_settings.attack_threshold < MIN_ATTACK_THRESHOLD || user_settings.attack_threshold > MAX_ATTACK_THRESHOLD )
		read_fail = true;

	if (user_settings.audio_volume < MIN_VOLUME || user_settings.audio_volume > MAX_VOLUME)
		read_fail = true;

	if (user_settings.lives_per_level < MIN_LIVES_PER_LEVEL || user_settings.lives_per_level > MAX_LIVES_PER_LEVEL)
		read_fail = true;


	if (read_fail) {
		reset_settings();

	}

}

void settings_write() {
	return; // TODO
	uint8_t temp[sizeof(user_settings)];
	//memcpy(temp, (char*)&user_settings, sizeof(user_settings));

	for (int i=0; i<sizeof(user_settings); i++)
	{
		// TODO EEPROM.write(i, temp[i]);
	}
}

void printError(int reason) {
	switch(reason) {
		case ERR_SETTING_NUM:
			std::cout << "Error: Invalid setting number" << '\n';
		break;
		case ERR_SETTING_RANGE:
			std::cout << "Error: Setting out of range" << '\n';
		break;
		default:
			std::cout << "Error:";
			std::cout << reason << '\n';
		break;
	}
}
