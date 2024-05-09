int buzzer = 10;
unsigned long act = 0;
// HORNO BOTONES
#define POWER 5
#define SET 6
#define UP 8
#define DOWN 7

#define LED 9
#define BUTTON 12

// tiempo entre pulsacion minimo (NO TOCAR)
int minAwaitTime = 130;

// cada cuantos minutos debe subir los grados
double minutesIntervalUp = 5;

// grados a subir por intervalo
double degreesIntervalUp = 2.5;

// Minutos a temperatura maxima
double minutesMaxTemp = 720;

// cada cuantos minutos debe bajar los grados
double minutesIntervalDown = 1.25;

// grados a bajar por intervalo
double degreesIntervalDown = 2.5;
