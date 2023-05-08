#include <Arduino.h>
#include "music.h"
#include "config.h"

#include <avr/wdt.h> // Incluir la librería que contiene el watchdog (wdt.h)

// funciones no toocar
void esperar(unsigned long milisegundos);
void enterTempSet();
void setInitTemp();
void setMinTemp();
void subirGrados(double grados);
void bajarGrados(double grados);
void guardarDatos();
void playFinishMusic();
void confirmSound();
void initMusic();

// the setup function runs once when you press reset or power the board
void setup()
{
	// initialize digital pin LED_BUILTIN as an output.
	wdt_disable();
	pinMode(LED_BUILTIN, OUTPUT);
	// pinMode(buzzer, OUTPUT);
	pinMode(POWER, OUTPUT);
	pinMode(SET, OUTPUT);
	pinMode(UP, OUTPUT);
	pinMode(DOWN, OUTPUT);
	pinMode(LED, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP);

	digitalWrite(LED, OUTPUT);
	esperar(1000);
	Serial.begin(9600);
	for (int i = 0; i < 3; i++)
	{
		tone(buzzer, 1000);
		delay(100);
		noTone(buzzer);
		delay(100);
	}
}

void loop()
{

	while (digitalRead(BUTTON))
	{
		// comprobar que esta pulsado por 1 segundo
		int ahora = millis();
		while (!digitalRead(BUTTON) && millis() - ahora < 1000)
		{
			digitalWrite(LED, 1);
		}
		digitalWrite(LED, 0);
	}
	// si esta  pulsado, esperar continuea sino vuelve al loop
	if (digitalRead(BUTTON))
	{
		return;
	}

	// empieza el programa

	initMusic();

	enterTempSet();
	setInitTemp();

	setMinTemp();

	guardarDatos();
	esperar(minutesIntervalUp * 60 * 1000);

	Serial.println("Iniciando ciclo de subida de temperatura...");
	// subir grados
	for (int i = 0; i < 24; i++)
	{
		enterTempSet();
		subirGrados(degreesIntervalUp);
		guardarDatos();
		esperar((unsigned long)(minutesIntervalUp * 60 * 1000));
	}

	esperar((unsigned long)(minutesMaxTemp * 60 * 1000));

	for (int i = 0; i < 24; i++)
	{
		enterTempSet();
		bajarGrados(degreesIntervalDown);
		guardarDatos();
		esperar((unsigned long)(minutesIntervalDown * 60 * 1000));
	}
	playFinishMusic();
}

void esperar(unsigned long milisegundos)
{

	act = millis();
	while (millis() - act < milisegundos)
	{
		digitalWrite(LED, 1);
		delay(100);
		digitalWrite(LED, 0);
		delay(100);
		// cada 30 segundos imprime cuanto falta
		if (millis() - act % 30000 < 100)
		{
			Serial.print("Faltan ");
			Serial.print((milisegundos - (millis() - act)) / 1000);
			Serial.println(" segundos");
		}
	}
	return;
}

void enterTempSet()
{
	// sonido de confirmacion
	confirmSound();

	Serial.println("Entrando seteo temperatura...");

	digitalWrite(SET, 1);
	esperar(3600);
	digitalWrite(SET, 0);
	esperar(minAwaitTime);
	digitalWrite(SET, 1);
	esperar(minAwaitTime);
	digitalWrite(SET, 0);
	esperar(minAwaitTime);

	Serial.println("OK");
	return;
}

void setInitTemp()
{

	Serial.println("Seteando temperatura inicial (-50 grados)...");

	tone(buzzer, 420);
	esperar(250);
	noTone(buzzer);
	esperar(250);
	tone(buzzer, 420);
	esperar(250);
	noTone(buzzer);
	esperar(250);

	digitalWrite(DOWN, 1);
	esperar(150000);

	digitalWrite(DOWN, 0);
	esperar(minAwaitTime);

	Serial.println("OK");

	return;
}

void setMinTemp()
{

	Serial.println("Seteando temperatura minima (20 grados)...");

	tone(buzzer, 520);
	esperar(250);
	noTone(buzzer);
	esperar(250);
	tone(buzzer, 200);
	esperar(250);
	noTone(buzzer);
	esperar(250);

	digitalWrite(UP, 1);
	digitalWrite(LED, 1);

	// esperrar 70.9 segundos
	esperar(70600);
	digitalWrite(UP, 0);

	esperar(minAwaitTime);

	Serial.println("OK");

	return;
}

void subirGrados(double grados)
{

	Serial.println("Subiendo grados...");

	tone(buzzer, 440);
	esperar(1000);
	noTone(buzzer);
	for (int i = 0; i < grados * 10; i++)
	{
		digitalWrite(UP, 1);
		digitalWrite(LED, 1);

		esperar(minAwaitTime);
		digitalWrite(UP, 0);
		digitalWrite(LED, 0);

		esperar(minAwaitTime);
	}

	Serial.println("OK");
	return;
}

void bajarGrados(double grados)
{

	Serial.println("Bajando grados...");

	tone(buzzer, 440);
	esperar(1000);
	noTone(buzzer);

	for (int i = 0; i < grados * 10; i++)
	{
		digitalWrite(DOWN, 1);
		digitalWrite(LED, 1);

		esperar(minAwaitTime);
		digitalWrite(DOWN, 0);
		digitalWrite(LED, 0);

		esperar(minAwaitTime);
	}

	Serial.println("OK");
	return;
}

void guardarDatos()
{

	Serial.println("Guardando datos...");

	digitalWrite(POWER, 1);
	esperar(minAwaitTime);
	digitalWrite(POWER, 0);
	esperar(minAwaitTime);

	Serial.println("OK");
	return;
}

void initMusic()
{
	tone(buzzer, 720);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	tone(buzzer, 520);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	tone(buzzer, 620);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	tone(buzzer, 320);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	tone(buzzer, 420);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	return;
}

void confirmSound()
{
	tone(buzzer, 420);
	esperar(100);
	noTone(buzzer);
	esperar(100);
	tone(buzzer, 440);
	esperar(100);
	noTone(buzzer);
	esperar(1000);
	return;
}

void playFinishMusic()
{

	// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
	// there are two values per note (pitch and duration), so for each note there are four bytes
	int notes = sizeof(melody) / sizeof(melody[0]) / 2;

	// this calculates the duration of a whole note in ms
	int wholenote = (60000 * 4) / tempo;

	int divider = 0, noteDuration = 0;

	for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
	{

		// calculates the duration of each note
		divider = melody[thisNote + 1];
		if (divider > 0)
		{
			// regular note, just proceed
			noteDuration = (wholenote) / divider;
		}
		else if (divider < 0)
		{
			// dotted notes are represented with negative durations!!
			noteDuration = (wholenote) / abs(divider);
			noteDuration *= 1.5; // increases the duration in half for dotted notes
		}

		// we only play the note for 90% of the duration, leaving 10% as a pause
		tone(buzzer, melody[thisNote], noteDuration * 0.9);

		// Wait for the specief duration before playing the next note.
		esperar(noteDuration);

		// stop the waveform generation before the next note.
		noTone(buzzer);
	}
	return;
}