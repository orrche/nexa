// SPDX-License-Identifier: GPL-2.0
/*
 * Simple application for controlling old nexa units
 *
 * Copyright (C) 2021 Kent Gustavsson <kent@minoris.se>
 */

#define SHORT 350
#define LONG 1000

/**
 * Connect PORTB5 to 443.92 a2abtae-02 sending module
 *
 *
 *  a2abtae-02
 *  +-------+
 *  | /---\ |
 *  ||     ||
 *  | \___/ |
 *  +-------+
 *   | | | |
 *   | | | +-- Ant
 *   | | +---- VCC
 *   | +------ Data
 *   +-------- GND
 */

char proto[10];
char protopos;

void setup()
{
	Serial.begin(9600);
	DDRB |= (1<<5);
	protopos = 0;
}

void sendLong()
{
	PORTB |= (1<<5);
	delayMicroseconds(LONG);
	PORTB &= ~(1<<5);
	delayMicroseconds(SHORT);
}

void sendShort()
{
	PORTB |= (1<<5);
	delayMicroseconds(SHORT);
	PORTB &= ~(1<<5);
	delayMicroseconds(LONG);
}

void sendByte(unsigned char startbit, unsigned char d)
{
	for(unsigned char i = startbit ; i<8; i++) {
		sendShort();
		if (d & (1<<(7-i))) {
			sendLong();
		} else {
			sendShort();
		}
	}
}
void send(unsigned char d1, unsigned char d2)
{
	for(int i = 0 ; i < 8 ; i++ ) {
		sendByte(4, d1);
		sendByte(0, d2);
		PORTB |= (1<<5);
		delayMicroseconds(SHORT);
		PORTB &= ~(1<<5);
		delayMicroseconds(LONG*10);
	}
}

unsigned char hexValue(char v) {
	if (v >= '0' && v <= '9') {
		return (v - '0');
	} else if (v >= 'A' && v <= 'F') {
		return (v - 'A' + 10);
	} else if (v >= 'a' && v <= 'f') {
		return (v - 'a' + 10);
	}

	return 0;
}

void handleProto() {
	if (protopos == 5) {
		if (proto[0] == 'a' ) {
			send(16 * hexValue(proto[1]) + hexValue(proto[2]), 16 * hexValue(proto[3]) + hexValue(proto[4]));
		}
	}
	protopos = 0;
}

void loop()
{
	if (Serial.available() > 0) {
		int incomingByte = Serial.read();
		if (incomingByte == '\n' || incomingByte == '\r') {
			handleProto();
			Serial.write('\r');
			Serial.write('\n');
		} else {
			Serial.write(incomingByte);
		if ( protopos < 10) {
			proto[protopos] = incomingByte;
			protopos++;
		}
		}
	}
}
