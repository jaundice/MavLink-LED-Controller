#include <mavlink.h>
#include "SwapAnimator.h"
#include "PulseAnimator.h"
#include "Animator.h"
#include "PositionFlags.h"
#include <FastLED.h>

#define CHIP_TYPE WS2812B
#define NUM_LEDS 20
#define NUM_LEDS_SHORT 20
#define NUM_LEDS_LEGS 23
#define NUM_STRIPS 10
#define MAVLINK_SERIAL  Serial1

#define ReadChar uart1_get_char

static int packet_drops = 0;
static int mode = 0;

CRGB strip1[NUM_LEDS];
CRGB strip2[NUM_LEDS];
CRGB strip3[NUM_LEDS_SHORT];
CRGB strip4[NUM_LEDS];
CRGB strip5[NUM_LEDS];
CRGB strip6[NUM_LEDS];
CRGB strip7[NUM_LEDS_SHORT];
CRGB strip8[NUM_LEDS];
CRGB strip9[NUM_LEDS_LEGS];
CRGB strip10[NUM_LEDS_LEGS];


CLEDController *  controllers[NUM_STRIPS] = {
	&FastLED.addLeds<CHIP_TYPE, 45>(strip1, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 20>(strip2, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 7>(strip3, NUM_LEDS_SHORT, 0),
	&FastLED.addLeds<CHIP_TYPE, 9>(strip4, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 10>(strip5, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 12>(strip6, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 49>(strip7, NUM_LEDS_SHORT, 0),
	&FastLED.addLeds<CHIP_TYPE, 47>(strip8, NUM_LEDS, 0),
	&FastLED.addLeds<CHIP_TYPE, 2>(strip9, NUM_LEDS_LEGS, 0),
	&FastLED.addLeds<CHIP_TYPE, 4>(strip10, NUM_LEDS_LEGS, 0)
};

PositionFlags flags[NUM_STRIPS] = {
	//arms
	(PositionFlags)(Bottom | Forward | OutsideIn),
	(PositionFlags)(Bottom | Forward | Diagonal | Right | OutsideIn),
	(PositionFlags)(Bottom | Right | OutsideIn),
	(PositionFlags)(Bottom | Backward | Diagonal | Right | OutsideIn),
	(PositionFlags)(Bottom | Backward | OutsideIn),
	(PositionFlags)(Bottom | Backward | Diagonal | Left | OutsideIn),
	(PositionFlags)(Bottom | Left | OutsideIn),
	(PositionFlags)(Bottom | Forward | Diagonal | Left | OutsideIn),

	//legs
	(PositionFlags)(Bottom | Right | OutsideIn),
	(PositionFlags)(Bottom | Left | OutsideIn),

};

Animator * animators[] = {
	new PulseAnimator(CRGB::White,CRGB::Blue, CRGB::Green, CRGB::Red, 10000),
	new SwapAnimator(CRGB::White,CRGB::Blue, CRGB::Green, CRGB::Red, 20000)

};



int counter;
int oldMode = 0;



void setup()
{
	Serial.begin(115200);
	MAVLINK_SERIAL.begin(115200);
	FastLED.setBrightness(128);

}

void loop()
{
	unsigned long m = micros();

	bool updated = false;

	int idx = 0;
	bool newMode = oldMode != mode;

	if (newMode)
		oldMode = mode;

	//switch (mode) {
	//case MAV_MODE_FLAG_SAFETY_ARMED:
	//	idx = 0;
	//	break;
	//default:
	//	idx = 1;
	//	break;
	//}


	updated = updated | animators[idx]->Execute(m, newMode, controllers, flags, NUM_STRIPS);

	if (updated)
	{
		FastLED.show();
	}

	//if (Serial.available()) {
		//Serial.println(micros() - m);
	//Serial.println(mode);
	//Serial.println(otherMessage);
	//}

	comm_receive(MAVLINK_SERIAL);

	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}



mavlink_message_t msg;
mavlink_status_t status;


void comm_receive(HardwareSerial serial) {

	int packetDrops;
	//receive data over serial 
	int successfulParses = 0;
	while (serial.available() > 0) {

		byte b[1];

		serial.readBytes(b, 1);

		//try to get a new message 
		if (mavlink_parse_char(0, b[0], &msg, &status)) {
			// Handle message
			//Serial.println("Parsed");
			successfulParses++;
			switch (msg.msgid) {
			case  MAVLINK_MSG_ID_HIL_CONTROLS:
			{
				mavlink_hil_controls_t packet;
				mavlink_msg_hil_controls_decode(&msg, &packet);
				mode = packet.mode;
				Serial.println("here");
				break;

			}
			default: {

				//Do nothing
				//Serial.println(msg.msgid);
				break;
			}
			}

			// And get the next one

		}
		packetDrops += status.packet_rx_drop_count;
	}
	Serial.println(packetDrops);

}



