/*
Este archivo controla el circuito de la semana 3 en Wokwi y esta organizado
para que se entienda de un vistazo.

1) Por que te doy este codigo: hace funcionar el circuito completo: una barra
   de 5 LEDs que muestra cuanta luz hay, un LED y un buzzer que avisan cuando
   el sensor de movimiento detecta a alguien, y dos LEDs que avisan la
   temperatura y la humedad del sensor DHT22.

2) Que logica use: en cada vuelta del loop se lee la luz, se revisa el
   movimiento y, cada 2 segundos, se lee el DHT22. Con esos datos se prenden
   o parpadean los LEDs.

3) Por que esta forma: el codigo esta dividido en funciones cortas, una por
   cada grupo de LEDs, para que cambiar un umbral o un tiempo no obligue a
   tocar el resto. Los pines y el comportamiento son los mismos del circuito
   original.

4) Que hace paso a paso: setup() prepara los pines y el sensor. Luego, en
   cada vuelta del loop(): se dibuja la barra de luz, se atiende el
   movimiento y, cuando pasan 2 segundos, se lee temperatura y humedad y se
   actualizan sus LEDs.
*/

#include <DHT.h>

// Pines del circuito (los mismos del diagrama de Wokwi)
const int PIN_MOVIMIENTO = 3;
const int PIN_ZUMBADOR = 16;
const int PIN_DHT = 2;
const int PIN_LUZ = A0;

const int PIN_LED_CIAN = 5;
const int PIN_LED_MORADO = 18;
const int PIN_LED_AMARILLO = 19;

const int PIN_LEDS_ROJOS[] = {9, 10, 11, 12, 13};
const int TOTAL_LEDS_ROJOS = 5;

// Tiempos de espera, en milisegundos
const unsigned long ESPERA_MOVIMIENTO = 2000;
const unsigned long INTERVALO_SENSOR = 2000;

#define TIPO_DHT DHT22
DHT dht(PIN_DHT, TIPO_DHT);

unsigned long instanteMovimiento = 0;
unsigned long ultimaLectura = 0;
bool movimientoAvisado = false;

// Que hace: enciende la cantidad de LEDs rojos que corresponde al nivel de luz.
// Para que: mostrar de forma visual cuanta luz recibe el sensor LDR.
// Pasos: convierte la lectura (0 a 1023) en un numero de 1 a 5 LEDs y los prende en orden.
// Como cambiarla: cambia los valores 5 y 1 del map() para invertir o suavizar la barra.
void dibujarBarraDeLuz(int nivelLuz) {
  int ledsEncendidos = map(nivelLuz, 0, 1023, TOTAL_LEDS_ROJOS, 1);
  ledsEncendidos = constrain(ledsEncendidos, 1, TOTAL_LEDS_ROJOS);

  for (int i = 0; i < TOTAL_LEDS_ROJOS; i++) {
    digitalWrite(PIN_LEDS_ROJOS[i], i < ledsEncendidos ? HIGH : LOW);
  }
}

// Que hace: prende un LED indicador de tres maneras: fijo, parpadeo lento o parpadeo rapido.
// Para que: avisar un estado sin repetir el mismo bloque de delay en cada condicion.
// Pasos: prende el LED; si debe quedar fijo termina, si no espera, lo apaga y espera de nuevo.
// Como cambiarla: usa pausa en milisegundos para elegir la velocidad del parpadeo.
void avisarIndicador(int pin, bool quedaFijo, int pausa) {
  digitalWrite(pin, HIGH);

  if (quedaFijo) {
    return;
  }

  delay(pausa);
  digitalWrite(pin, LOW);
  delay(pausa);
}

// Que hace: decide como debe avisar el LED morado segun la temperatura.
// Para que: separar la regla de temperatura del resto del loop.
// Pasos: si hace frio parpadea lento, si esta en rango queda fijo y si hace calor parpadea rapido.
// Como cambiarla: ajusta los limites 0 y 35 o las pausas 250 y 50.
void avisarTemperatura(float temperatura) {
  if (temperatura < 0) {
    avisarIndicador(PIN_LED_MORADO, false, 250);
  } else if (temperatura <= 35) {
    avisarIndicador(PIN_LED_MORADO, true, 0);
  } else {
    avisarIndicador(PIN_LED_MORADO, false, 50);
  }
}

// Que hace: decide como debe avisar el LED amarillo segun la humedad.
// Para que: separar la regla de humedad del resto del loop.
// Pasos: si esta seco parpadea lento, si esta en rango queda fijo y si esta muy humedo parpadea rapido.
// Como cambiarla: ajusta los limites 30 y 70 o las pausas 250 y 50.
void avisarHumedad(float humedad) {
  if (humedad < 30) {
    avisarIndicador(PIN_LED_AMARILLO, false, 250);
  } else if (humedad <= 70) {
    avisarIndicador(PIN_LED_AMARILLO, true, 0);
  } else {
    avisarIndicador(PIN_LED_AMARILLO, false, 50);
  }
}

// Que hace: atiende el sensor de movimiento, el LED cian y el buzzer.
// Para que: mantener el aviso encendido 2 segundos aunque la persona ya se haya ido.
// Pasos: si hay movimiento prende y guarda la hora; si no hay, apaga cuando pasaron 2 segundos.
// Como cambiarla: cambia ESPERA_MOVIMIENTO o la frecuencia 1000 del tone().
void atenderMovimiento() {
  if (digitalRead(PIN_MOVIMIENTO) == HIGH) {
    digitalWrite(PIN_LED_CIAN, HIGH);
    tone(PIN_ZUMBADOR, 1000);
    instanteMovimiento = millis();
    movimientoAvisado = true;
  } else if (movimientoAvisado && millis() - instanteMovimiento >= ESPERA_MOVIMIENTO) {
    digitalWrite(PIN_LED_CIAN, LOW);
    noTone(PIN_ZUMBADOR);
    movimientoAvisado = false;
  }
}

// Que hace: lee el DHT22 cada 2 segundos y actualiza los dos LEDs indicadores.
// Para que: el sensor es lento y no hace falta leerlo en cada vuelta del loop.
// Pasos: espera a que pasen 2 segundos, lee ambos valores, revisa que sean validos y avisa.
// Como cambiarla: cambia INTERVALO_SENSOR para leer mas o menos seguido.
void leerAmbiente() {
  if (millis() - ultimaLectura < INTERVALO_SENSOR) {
    return;
  }

  ultimaLectura = millis();

  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();

  if (isnan(temperatura) || isnan(humedad)) {
    Serial.println("Lectura del DHT22 invalida, se mantiene el aviso anterior");
    return;
  }

  Serial.print("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" C   Humedad: ");
  Serial.print(humedad);
  Serial.println(" %");

  avisarTemperatura(temperatura);
  avisarHumedad(humedad);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Circuito listo: luz, movimiento, temperatura y humedad");

  pinMode(PIN_MOVIMIENTO, INPUT);
  pinMode(PIN_ZUMBADOR, OUTPUT);
  pinMode(PIN_LED_CIAN, OUTPUT);
  pinMode(PIN_LED_MORADO, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);

  for (int i = 0; i < TOTAL_LEDS_ROJOS; i++) {
    pinMode(PIN_LEDS_ROJOS[i], OUTPUT);
  }

  dht.begin();
}

void loop() {
  dibujarBarraDeLuz(analogRead(PIN_LUZ));
  atenderMovimiento();
  leerAmbiente();

  delay(10);
}
