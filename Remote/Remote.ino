#include <IRLibSendBase.h>
#include <IRLib_HashRaw.h>
#include <IRLibRecvPCI.h>

#define RECEIVER_PIN 2
#define RECEIVER_LED_PIN 4
#define POSITIONS 16
#define BUTTONS 4

IRrecvPCI IRReceiver(RECEIVER_PIN);
IRsendRaw IRTransmitter;

uint16_t* RecvData = NULL;
int RecvLength = 0;
int CurrPos = 0;
int Button[BUTTONS];

struct Data {
	uint16_t* IR_Data = NULL;
	int Data_legth;
};

Data SavedButtons[POSITIONS];

void setup() {
	Serial.begin(9600);
	IRReceiver.enableIRIn();

	pinMode(RECEIVER_LED_PIN, OUTPUT);

	for (int i = 5; i <= 8; i++)
		pinMode(i, OUTPUT);

	for (int i = 10; i <= 13; i++)
		pinMode(i, INPUT);
}

void loop() {
	if (IRReceiver.getResults()) {

		digitalWrite(RECEIVER_LED_PIN, HIGH);

		RecvLength = recvGlobal.recvLength;
		RecvData = new uint16_t[RecvLength];

		for (bufIndex_t i = 1; i < recvGlobal.recvLength; i++) {
			RecvData[i - 1] = recvGlobal.recvBuffer[i];
		}

		RecvData[RecvLength - 1] = 1000;

		IRReceiver.enableIRIn();

		/*for (int i = 0; i < RecvLength; i++) {
			Serial.print(RecvData[i]);
			Serial.print(F(", "));
		}
		Serial.println(RecvLength, DEC);*/
	}

	for (int i = 0; i < 4; i++)
		Button[i] = digitalRead(i + 10);

	if (Button[3] == HIGH)
	{
		if (RecvData != NULL)
		{
			//Serial.println(RecvLength, DEC);
			Save();
			//Serial.println(F("saved"));
			//Serial.println(SavedButtons[CurrPos].Data_legth, DEC);
		}
	}

	if (Button[1] == HIGH)
	{
		IRReceiver.disableIRIn();

		/*Serial.println(F("sent"));
		for (int i = 0; i < SavedButtons[CurrPos].Data_legth; i++) {
			Serial.print(SavedButtons[CurrPos].IR_Data[i], DEC);
			Serial.print(F(", "));
		}*/

		IRTransmitter.send(SavedButtons[CurrPos].IR_Data, SavedButtons[CurrPos].Data_legth, 36);
		delay(200);
		IRReceiver.enableIRIn();

		digitalWrite(RECEIVER_LED_PIN, LOW);
	}

	if (Button[0] == HIGH)
	{
		IRReceiver.disableIRIn();

		if (CurrPos != 0)
			CurrPos--;
		else
			CurrPos = POSITIONS - 1;

		Indicate();
		delay(200);
		IRReceiver.enableIRIn();
	}

	if (Button[2] == HIGH)
	{
		IRReceiver.disableIRIn();

		if (CurrPos != POSITIONS - 1)
			CurrPos++;
		else
			CurrPos = 0;

		Indicate();
		delay(200);
		IRReceiver.enableIRIn();
	}
}

void Save()
{
	if (SavedButtons[CurrPos].IR_Data != NULL)
	{
		delete[] SavedButtons[CurrPos].IR_Data;
		SavedButtons[CurrPos].IR_Data = NULL;
	}

	SavedButtons[CurrPos].Data_legth = RecvLength;
	SavedButtons[CurrPos].IR_Data = new uint16_t[RecvLength];

	for (int i = 0; i < RecvLength; i++)
		SavedButtons[CurrPos].IR_Data[i] = RecvData[i];

	delete[] RecvData;
	RecvData = NULL;

	digitalWrite(RECEIVER_LED_PIN, LOW);
}

void Indicate()
{
	int Indicator = CurrPos;

	if (Indicator > 7)
	{
		digitalWrite(8, HIGH);
		Indicator -= 8;
	}
	else digitalWrite(8, LOW);

	switch (Indicator)
	{
	case 0:
		digitalWrite(5, LOW);
		digitalWrite(6, LOW);
		digitalWrite(7, LOW);
		break;

	case 1:
		digitalWrite(5, HIGH);
		digitalWrite(6, LOW);
		digitalWrite(7, LOW);
		break;

	case 2:
		digitalWrite(5, LOW);
		digitalWrite(6, HIGH);
		digitalWrite(7, LOW);
		break;

	case 3:
		digitalWrite(5, HIGH);
		digitalWrite(6, HIGH);
		digitalWrite(7, LOW);
		break;

	case 4:
		digitalWrite(5, LOW);
		digitalWrite(6, LOW);
		digitalWrite(7, HIGH);
		break;

	case 5:
		digitalWrite(5, HIGH);
		digitalWrite(6, LOW);
		digitalWrite(7, HIGH);
		break;

	case 6:
		digitalWrite(5, LOW);
		digitalWrite(6, HIGH);
		digitalWrite(7, HIGH);
		break;

	case 7:
		digitalWrite(5, HIGH);
		digitalWrite(6, HIGH);
		digitalWrite(7, HIGH);
		break;
	}

}
