void displayState() {
    Serial.print("estado:");
    Serial.println(state);
}

void counter(bool show) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillisDisplay >= 1000) {
        previousMillisDisplay = currentMillis;
        lastDigit--;
    } 
}

void redTrafficLight() {
    digitalWrite(PIN_RED_LED, HIGH);
    digitalWrite(PIN_GREEN_LED, LOW);
}

void greenTrafficLight(bool hurry) {
    if(hurry) {
        unsigned long currentMillis = millis();
        if (currentMillis - previousMillisGreenLight >= 500) {
            previousMillisGreenLight = currentMillis;
            lastGreenLight = !lastGreenLight;
            digitalWrite(PIN_GREEN_LED, lastGreenLight);
        }
    } else {
        digitalWrite(PIN_GREEN_LED, HIGH);
    }
    digitalWrite(PIN_RED_LED, LOW);
}

unsigned long previousMillisSound = 0;
void beep(bool hurry) {
    unsigned long currentMillis = millis();
    if(hurry) {
       if (currentMillis - previousMillisSound >= 200) {
           previousMillisSound = currentMillis;
            tone(SOUND_PIN, 500, 50);
       }
    } else {
       if (currentMillis - previousMillisSound >= 1000) {
           previousMillisSound = currentMillis;
            tone(SOUND_PIN, 1500, 100);
       }
    }
}

void closeBarrier(){
    Serial.println("closeBarrier");
	myservo.write(0); 
}

void openBarrier(){
    Serial.println("openBarrier");
	myservo.write(90);
}