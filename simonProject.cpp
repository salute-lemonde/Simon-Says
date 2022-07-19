#include <Arduino.h>

#define MAX_SEQ_LEN 30
enum colours {green1=0, yellow=1, red=2, green2=3, ledB=4 };
long seqArray[MAX_SEQ_LEN];
int userArr[MAX_SEQ_LEN];
int seqLen;
int dTime;

#define BTN_1 11
#define GREEN1_LED_PIN 12

#define BTN_2 8
#define YELLOW_LED_PIN 9

#define BTN_3 5
#define RED_LED_PIN 6

#define BTN_4 2
#define GREEN2_LED_PIN 3

//initialize the array
void initArray() {
	for(int i=0; i<MAX_SEQ_LEN; i++)
	{
		seqArray[i] = ledB;
		userArr[i] = ledB;
	}
}

//make a random sequence
void setSequence(int seqLen) {
	//initArray();

	randomSeed(analogRead(A0)); 	//seed the sequence to start at an arbitrary number
	Serial.println("setting sequence");
	Serial.print("level: ");
	Serial.println(seqLen);
	for(int i=0; i<seqLen; i++) 	 //make a random seq of length seqLen in the array
		seqArray[i] = random(green1, ledB);
}

//lights the LED that corresponds to each item in the computer generated sequence
void lightLED(int dTime) {
	Serial.println("in the LED block");
	for(int i=0; i<seqLen; i++) {
		int num = seqArray[i];

		switch (num) {
		case green1:
			digitalWrite(GREEN1_LED_PIN, HIGH);
			delay(dTime);
			digitalWrite(GREEN1_LED_PIN, LOW);
			delay(dTime);
			break;
		case yellow:
			digitalWrite(YELLOW_LED_PIN, HIGH);
			delay(dTime);
			digitalWrite(YELLOW_LED_PIN, LOW);
			delay(dTime);
			break;
		case red:
			digitalWrite(RED_LED_PIN, HIGH);
			delay(dTime);
			digitalWrite(RED_LED_PIN, LOW);
			delay(dTime);
			break;
		case green2:
			digitalWrite(GREEN2_LED_PIN, HIGH);
			delay(dTime);
			digitalWrite(GREEN2_LED_PIN, LOW);
			delay(dTime);
			break;
		}
	}

}//void lightLED()

//return true if user sequence is correct, false if otherwise
bool checkUserSeq() {
	bool boolean;
	Serial.println("comparing sequences");

	Serial.print("computer seq : ");
	for(int i=0; i<seqLen; i++) {
		Serial.print(seqArray[i]);
		Serial.print(", ");
	}

	//Serial.println();

	Serial.print("user seq : ");
	for(int i=0; i<seqLen; i++) {
		Serial.print(userArr[i]);
		Serial.print(", ");
	}

	//Serial.println();

	for(int i=0; i<seqLen; i++) {
		if(userArr[i] != seqArray[i]) {   //if false; restart
			Serial.println("restart");
			return false;
		}
	}
	seqLen += 1;
	dTime += 15;
	Serial.println("proceed");
	return true;
} //checKUserSeq()

//light sequence to indicate the program has started
void start() {

	initArray();
	digitalWrite(GREEN1_LED_PIN, HIGH);
	digitalWrite(YELLOW_LED_PIN, HIGH);
	digitalWrite(RED_LED_PIN, HIGH);
	digitalWrite(GREEN2_LED_PIN, HIGH);

	delay(1000);

	digitalWrite(GREEN1_LED_PIN, HIGH);
	digitalWrite(YELLOW_LED_PIN, LOW);
	digitalWrite(RED_LED_PIN, HIGH);
	digitalWrite(GREEN2_LED_PIN, LOW);

	delay(1000);

	digitalWrite(GREEN1_LED_PIN, LOW);
	digitalWrite(YELLOW_LED_PIN, HIGH);
	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN2_LED_PIN, HIGH);
	delay(1000);
	//}
	digitalWrite(GREEN1_LED_PIN, LOW);
	digitalWrite(YELLOW_LED_PIN, LOW);
	digitalWrite(RED_LED_PIN, LOW);
	digitalWrite(GREEN2_LED_PIN, LOW);

	delay(1000);

	digitalWrite(LED_BUILTIN, HIGH);
} //start()

//replay start sequence - all lights come on first,
//then odd number lights come on while even number lights stay off, then vice versa
//blink the built in led until a new sequence has been created
void restart() { //only restart once user fails to replicate generated sequence

	bool flag = false;
	seqLen = 1;
	dTime = 700;
	start();
	while(!flag) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);
		setSequence(seqLen);
		flag = true;
	}
	lightLED(dTime);
}

//light sequence to indicate that the user chose the correct sequence
void proceed() {
	bool flag = false;

	while(!flag) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(1000);
		digitalWrite(LED_BUILTIN, LOW);
		delay(1000);
		setSequence(seqLen);
		flag = true;
	}
}

void setup() {
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);

	pinMode(BTN_1, INPUT_PULLUP);
	pinMode(GREEN1_LED_PIN, OUTPUT);

	pinMode(BTN_2, INPUT_PULLUP);
	pinMode(YELLOW_LED_PIN, OUTPUT);

	pinMode(BTN_3, INPUT_PULLUP);
	pinMode(RED_LED_PIN, OUTPUT);


	pinMode(BTN_4, INPUT_PULLUP);
	pinMode(GREEN2_LED_PIN, OUTPUT);

}// void setup()


int count = 0;

int g1ledState = LOW;   //state of the green LED 1
int p1State = LOW;		//old state of button1...
int c1State; 			//current state of button1

int yledState = LOW;	//state of the yellow LED
int p2State = LOW;		//old state of button2...
int c2State; 			//current state of button2...

int rledState = LOW;    //state of the green LED
int p3State = LOW;		//old state of button3...
int c3State; 			//current state of button3

int g2ledState = LOW;   //state of the green LED 2
int p4State = LOW;		//old state of button4...
int c4State; 			//current state of button4

unsigned long lastDb1Time = 0;   //debounce time for button1
unsigned long lastDb2Time = 0;	 //debounce time for button2
unsigned long lastDb3Time = 0;   //debounce time for button3
unsigned long lastDb4Time = 0;	 //debounce time for button4
long dbDelay = 50;

void loop() {

	if(seqLen > 0) {		//make sure there is a sequence

		//*******//
		int read1 = !digitalRead(BTN_1);
		if(read1 != p1State) { 		//check if button state has changed from prev state
			lastDb1Time = millis();
		}

		if((millis() - lastDb1Time) > dbDelay) {
			if(read1 != c1State) {
				c1State = read1;

				if(c1State == HIGH) {
					g1ledState = !g1ledState;  //now HIGH
					digitalWrite(GREEN1_LED_PIN, g1ledState);
					delay(200);
					userArr[count] = green1;
					count += 1;
					g1ledState = !g1ledState;
				}
				//else
			}
		}
		digitalWrite(GREEN1_LED_PIN, g1ledState);
		p1State = read1;
		//*********//

		int read2 = !digitalRead(BTN_2);
		if(read2 != p2State) { 		//check if button state has changed from prev state
			lastDb2Time = millis();
		}

		if ((millis() - lastDb2Time) > dbDelay) {

			if(read2 != c2State) {
				/*Serial.println("if-reading");
						Serial.print("read2: ");
						Serial.println(read2);*/
				c2State = read2;

				if(c2State == HIGH) {
					yledState = !yledState;
					digitalWrite(YELLOW_LED_PIN, yledState);
					delay(200);
					userArr[count] = yellow;
					count += 1;
					yledState = !yledState;
				}
				//else
			}
		}
		digitalWrite(YELLOW_LED_PIN, yledState);
		p2State = read2;
		//*********//

		int read3 = !digitalRead(BTN_3);
		if(read3 != p3State) { 		//check if button state has changed from prev state
			lastDb3Time = millis();
		}

		if ((millis() - lastDb3Time) > dbDelay) {

			if(read3 != c3State) {
				c3State = read3;
				if(c3State == HIGH) {
					rledState = !rledState;
					digitalWrite(RED_LED_PIN, rledState);
					delay(200);
					userArr[count] = red;
					count += 1;
					rledState = !rledState;
				}
				//else
			}
		}
		digitalWrite(RED_LED_PIN, rledState);
		p3State = read3;
		//*********//

		int read4 = !digitalRead(BTN_4);
		if(read4 != p4State) { 		//check if button state has changed from prev state
			lastDb4Time = millis();
		}

		if ((millis() - lastDb4Time) > dbDelay) {
			if(read4 != c4State) {
				c4State = read4;

				if(c4State == HIGH) {
					g2ledState = !g2ledState;
					digitalWrite(GREEN2_LED_PIN, g2ledState);
					delay(200);
					userArr[count] = green2;
					count += 1;
					g2ledState = !g2ledState;
				}
				//else
			}
		}
		digitalWrite(GREEN2_LED_PIN, g2ledState);
		p4State = read4;
		//*********//

		//compare user sequence and computer sequence after storing user sequence
		if(count == seqLen) {
			count = 0;			 //reset count
			if(!checkUserSeq())  //restart if user sequence is wrong or there is no sequence yet
				restart();

			else {						//proceed to the next level if user sequence is correct
				proceed();
				lightLED(dTime);
			}
		}
	}

	else {
		seqLen = 1;
		dTime = 700;
		start();
		setSequence(seqLen);
		lightLED(dTime);
	}
}//void loop ()
