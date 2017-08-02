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

unsigned int measure()
{
    digitalWrite(TRIGGER_PIN,LOW); 
    delayMicroseconds(5);
    digitalWrite(TRIGGER_PIN, HIGH); 
    delayMicroseconds(10);
    unsigned int distance= int(0.017*pulseIn(ECHO_PIN, HIGH));
    Serial.println(String("measure:") + distance);
    return constrain(distance, 1, 300);
}

unsigned int measureMedia()
{
    static int measurements[DISTANCE_MAX_READS];
    long mean = 0;
    for (int i = 0; i < DISTANCE_MAX_READS - 1; i++)
    {
        measurements[i] = measurements[i + 1];
        mean += measurements[i];
    }
    measurements[DISTANCE_MAX_READS - 1] = measure();
    mean += measurements[DISTANCE_MAX_READS - 1];
    Serial.println(String("measureMedia:") + mean);
    return mean /= DISTANCE_MAX_READS;
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
            tone(SOUND_PIN, 500, 10);
       }
    } else {
       if (currentMillis - previousMillisSound >= 1000) {
            tone(SOUND_PIN, 1000, 10);
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