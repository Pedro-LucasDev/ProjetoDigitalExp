/*
 * CCP020 - Reprodutor de Musicas - ARDUINO MEGA 2560
 * Sem biblioteca externa (so usa Wire)
 *
 * Placa: Ferramentas > Placa > Arduino Mega or Mega 2560
 *
 * Ligacoes:
 *  LCD I2C : GND->GND | VCC->5V | SDA->pino 20 | SCL->pino 21
 *  Botoes  : UP=2, DOWN=3, PLAY/PAUSE=4, STOP=5 (outra perna no GND)
 *  LEDs    : verde=6, vermelho=7 (resistor 330 ohms, catodo no GND)
 *  Buzzer  : pino 8 (outro terminal no GND)
 */

#include <Wire.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdio.h>

// ---------------- PINOS ----------------
#define BTN_UP 2
#define BTN_DOWN 3
#define BTN_PLAY 4
#define BTN_STOP 5
#define LED_VERDE 6
#define LED_VERMELHO 7
#define BUZZER 8

// =====================================================
//  DRIVER MINIMO PARA LCD 16x2 COM MODULO I2C (PCF8574)
//  P0=RS P1=RW P2=EN P3=Backlight P4..P7=D4..D7
// =====================================================
uint8_t lcdAddr = 0x27;
bool lcdOk = false;
#define LCD_BL 0x08
#define LCD_EN 0x04
#define LCD_RS 0x01

void expWrite(uint8_t dado) {
  Wire.beginTransmission(lcdAddr);
  Wire.write(dado | LCD_BL);
  Wire.endTransmission();
}

void lcdWrite4(uint8_t nibble, uint8_t rs) {
  uint8_t d = (nibble << 4) | rs;
  expWrite(d);
  expWrite(d | LCD_EN);
  delayMicroseconds(2);
  expWrite(d & ~LCD_EN);
  delayMicroseconds(60);
}

void lcdSend(uint8_t valor, uint8_t rs) {
  lcdWrite4(valor >> 4, rs);
  lcdWrite4(valor & 0x0F, rs);
}

void lcdCmd(uint8_t c) {
  lcdSend(c, 0);
}

void lcdInit() {
  delay(60);
  expWrite(0);
  delay(10);
  lcdWrite4(0x03, 0);
  delay(5);
  lcdWrite4(0x03, 0);
  delayMicroseconds(200);
  lcdWrite4(0x03, 0);
  delayMicroseconds(200);
  lcdWrite4(0x02, 0);   // modo 4 bits
  lcdCmd(0x28);         // 2 linhas, 5x8
  lcdCmd(0x0C);         // display ligado, sem cursor
  lcdCmd(0x06);         // cursor avanca para a direita
  lcdCmd(0x01);         // limpa
  delay(3);
}

void lcdLinha(uint8_t linha, const char* texto) {
  if (!lcdOk) return;   // sem LCD, ignora
  lcdCmd(0x80 | (linha == 0 ? 0x00 : 0x40));
  uint8_t i = 0;
  while (texto[i] != '\0' && i < 16) {
    lcdSend(texto[i], LCD_RS);
    i++;
  }
  while (i < 16) {
    lcdSend(' ', LCD_RS);
    i++;
  }
}

// ---------------- NOTAS ----------------
#define N_C4 262
#define N_D4 294
#define N_E4 330
#define N_F4 349
#define N_G4 392
#define N_A4 440

const char nomesMusicas[5][17] = {
  "Ode a Alegria",
  "Estrelinha",
  "Frere Jacques",
  "Jingle Bells",
  "Mary Lamb"
};

struct Nota {
  unsigned int frequencia;
  unsigned int duracao;
};

const Nota musica1[] PROGMEM = {
  {N_E4,300},{N_E4,300},{N_F4,300},{N_G4,300},
  {N_G4,300},{N_F4,300},{N_E4,300},{N_D4,300},
  {N_C4,300},{N_C4,300},{N_D4,300},{N_E4,300},
  {N_E4,450},{N_D4,150},{N_D4,600}
};

const Nota musica2[] PROGMEM = {
  {N_C4,400},{N_C4,400},{N_G4,400},{N_G4,400},
  {N_A4,400},{N_A4,400},{N_G4,700},
  {N_F4,400},{N_F4,400},{N_E4,400},{N_E4,400},
  {N_D4,400},{N_D4,400},{N_C4,700}
};

const Nota musica3[] PROGMEM = {
  {N_C4,300},{N_D4,300},{N_E4,300},{N_C4,300},
  {N_C4,300},{N_D4,300},{N_E4,300},{N_C4,300},
  {N_E4,300},{N_F4,300},{N_G4,600},
  {N_E4,300},{N_F4,300},{N_G4,600}
};

const Nota musica4[] PROGMEM = {
  {N_E4,250},{N_E4,250},{N_E4,500},
  {N_E4,250},{N_E4,250},{N_E4,500},
  {N_E4,250},{N_G4,250},{N_C4,350},
  {N_D4,150},{N_E4,700},
  {N_F4,250},{N_F4,250},{N_F4,350},
  {N_F4,150},{N_F4,250},{N_E4,250},
  {N_E4,250},{N_E4,150},{N_E4,150},
  {N_E4,250},{N_D4,250},{N_D4,250},
  {N_E4,250},{N_D4,500},{N_G4,500}
};

const Nota musica5[] PROGMEM = {
  {N_E4,300},{N_D4,300},{N_C4,300},{N_D4,300},
  {N_E4,300},{N_E4,300},{N_E4,600},
  {N_D4,300},{N_D4,300},{N_D4,600},
  {N_E4,300},{N_G4,300},{N_G4,600},
  {N_E4,300},{N_D4,300},{N_C4,300},{N_D4,300},
  {N_E4,300},{N_E4,300},{N_E4,300},{N_E4,300},
  {N_D4,300},{N_D4,300},{N_E4,300},{N_D4,300},
  {N_C4,600}
};

// ---------------- ESTADOS ----------------
#define MENU 0
#define TOCANDO 1
#define PAUSADO 2

int estado = MENU;
int musicaSelecionada = 0;
int notaAtual = 0;
unsigned long proximaNota = 0;
unsigned long tempoRestante = 0;
unsigned int frequenciaAtual = 0;

// ---------------- BOTOES ----------------
const int pinosBotoes[4] = {BTN_UP, BTN_DOWN, BTN_PLAY, BTN_STOP};
int ultimaLeitura[4] = {HIGH, HIGH, HIGH, HIGH};
int estadoEstavel[4] = {HIGH, HIGH, HIGH, HIGH};
unsigned long tempoDebounce[4] = {0, 0, 0, 0};
#define DEBOUNCE 40

int quantidadeNotas(int musica) {
  switch (musica) {
    case 0: return sizeof(musica1) / sizeof(Nota);
    case 1: return sizeof(musica2) / sizeof(Nota);
    case 2: return sizeof(musica3) / sizeof(Nota);
    case 3: return sizeof(musica4) / sizeof(Nota);
    case 4: return sizeof(musica5) / sizeof(Nota);
  }
  return 0;
}

void lerNota(int musica, int indice, unsigned int *freq, unsigned int *dur) {
  Nota n;
  n.frequencia = 0;
  n.duracao = 0;
  switch (musica) {
    case 0: memcpy_P(&n, &musica1[indice], sizeof(Nota)); break;
    case 1: memcpy_P(&n, &musica2[indice], sizeof(Nota)); break;
    case 2: memcpy_P(&n, &musica3[indice], sizeof(Nota)); break;
    case 3: memcpy_P(&n, &musica4[indice], sizeof(Nota)); break;
    case 4: memcpy_P(&n, &musica5[indice], sizeof(Nota)); break;
  }
  *freq = n.frequencia;
  *dur = n.duracao;
}

int botaoPressionado(int indice) {
  int leitura = digitalRead(pinosBotoes[indice]);
  if (leitura != ultimaLeitura[indice]) {
    tempoDebounce[indice] = millis();
    ultimaLeitura[indice] = leitura;
  }
  if (millis() - tempoDebounce[indice] > DEBOUNCE) {
    if (leitura != estadoEstavel[indice]) {
      estadoEstavel[indice] = leitura;
      if (estadoEstavel[indice] == LOW) return 1;
    }
  }
  return 0;
}

void atualizarLEDs() {
  digitalWrite(LED_VERDE, estado == TOCANDO ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, estado == PAUSADO ? HIGH : LOW);
}

void mostrarMenu() {
  char linha[20];
  snprintf(linha, sizeof(linha), ">%d %s", musicaSelecionada + 1, nomesMusicas[musicaSelecionada]);
  lcdLinha(0, "Selecione musica");
  lcdLinha(1, linha);
}

void atualizarDisplay() {
  lcdLinha(0, estado == TOCANDO ? "Reproduzindo:" : "Musica pausada:");
  lcdLinha(1, nomesMusicas[musicaSelecionada]);
}

void iniciarNota() {
  if (notaAtual >= quantidadeNotas(musicaSelecionada)) {
    noTone(BUZZER);
    estado = MENU;
    atualizarLEDs();
    mostrarMenu();
    return;
  }
  unsigned int freq, dur;
  lerNota(musicaSelecionada, notaAtual, &freq, &dur);
  frequenciaAtual = freq;
  if (freq > 0) tone(BUZZER, freq, dur);
  else noTone(BUZZER);
  proximaNota = millis() + dur + 70;
}

void iniciarMusica() {
  notaAtual = 0;
  estado = TOCANDO;
  atualizarLEDs();
  atualizarDisplay();
  iniciarNota();
}

void alternarPlayPause() {
  if (estado == MENU) {
    iniciarMusica();
  } else if (estado == TOCANDO) {
    tempoRestante = 0;
    if ((long)(proximaNota - millis()) > 0) tempoRestante = proximaNota - millis();
    noTone(BUZZER);
    estado = PAUSADO;
    atualizarLEDs();
    atualizarDisplay();
  } else if (estado == PAUSADO) {
    estado = TOCANDO;
    if (frequenciaAtual > 0 && tempoRestante > 0) tone(BUZZER, frequenciaAtual, tempoRestante);
    proximaNota = millis() + tempoRestante;
    atualizarLEDs();
    atualizarDisplay();
  }
}

void pararMusica() {
  noTone(BUZZER);
  estado = MENU;
  notaAtual = 0;
  atualizarLEDs();
  mostrarMenu();
}

// ---------------- DETECCAO DO ENDERECO I2C ----------------
bool existeI2C(uint8_t endereco) {
  Wire.beginTransmission(endereco);
  return Wire.endTransmission() == 0;
}

uint8_t acharLCD() {
  if (existeI2C(0x27)) return 0x27;
  if (existeI2C(0x3F)) return 0x3F;
  for (uint8_t a = 1; a < 127; a++) {
    if (existeI2C(a)) return a;
  }
  return 0;
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(9600);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_PLAY, INPUT_PULLUP);
  pinMode(BTN_STOP, INPUT_PULLUP);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Teste rapido: beep + LED verde + LED vermelho
  digitalWrite(LED_VERDE, HIGH);
  tone(BUZZER, 1000, 150);
  delay(300);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, HIGH);
  delay(300);
  digitalWrite(LED_VERMELHO, LOW);

  Wire.begin();
  uint8_t endereco = acharLCD();

  if (endereco == 0) {
    // Sem LCD: avisa piscando os dois LEDs 3 vezes e segue normalmente
    Serial.println("ERRO: LCD I2C nao encontrado. Seguindo sem display.");
    for (int i = 0; i < 3; i++) {
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, HIGH);
      delay(150);
      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_VERMELHO, LOW);
      delay(150);
    }
  } else {
    lcdAddr = endereco;
    lcdOk = true;
    Serial.print("LCD I2C encontrado em 0x");
    Serial.println(lcdAddr, HEX);
    lcdInit();
    lcdLinha(0, "REPRODUTOR");
    lcdLinha(1, "DE MUSICAS");
    delay(1500);
  }

  mostrarMenu();
}

// ---------------- LOOP ----------------
void loop() {
  int up = botaoPressionado(0);
  int down = botaoPressionado(1);
  int play = botaoPressionado(2);
  int stop = botaoPressionado(3);

  if (estado == MENU) {
    if (up) {
      musicaSelecionada--;
      if (musicaSelecionada < 0) musicaSelecionada = 4;
      mostrarMenu();
    }
    if (down) {
      musicaSelecionada++;
      if (musicaSelecionada > 4) musicaSelecionada = 0;
      mostrarMenu();
    }
  }

  if (play) alternarPlayPause();
  if (stop && estado != MENU) pararMusica();

  if (estado == TOCANDO && (long)(millis() - proximaNota) >= 0) {
    notaAtual++;
    iniciarNota();
  }
}
