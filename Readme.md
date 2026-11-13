# 🎵 Reprodutor de Músicas com Arduino UNO

Projeto desenvolvido para a disciplina **CCP020 – Digital Experience Ultimate**, do curso de Ciência da Computação.

O objetivo é desenvolver um reprodutor de músicas utilizando Arduino UNO, permitindo que o usuário navegue por uma lista de músicas, selecione uma melodia e controle sua reprodução por meio de botões físicos.

---

## 📌 1. Introdução

O projeto consiste no desenvolvimento de um reprodutor de músicas utilizando a plataforma Arduino UNO e componentes eletrônicos.

A proposta é criar um dispositivo capaz de armazenar melodias na memória do microcontrolador e reproduzi-las por meio de um buzzer, oferecendo ao usuário uma interface simples e intuitiva para controlar as músicas.

O sistema permite navegar entre as músicas disponíveis, iniciar a reprodução, pausar, continuar e interromper a execução.

Além disso, foram utilizados LEDs para indicar visualmente o estado do reprodutor, proporcionando uma melhor interação com o dispositivo.

### Objetivos

- Desenvolver um sistema de reprodução de músicas utilizando Arduino UNO.
- Armazenar pelo menos cinco melodias na memória do microcontrolador.
- Criar um menu de seleção de músicas utilizando display LCD 16x2.
- Implementar controles físicos para navegação e reprodução.
- Utilizar LEDs para indicar o estado atual do sistema.
- Aplicar conceitos de programação em C/C++, lógica de programação e eletrônica básica.

---

## 🛠️ 2. Metodologia

O desenvolvimento foi realizado utilizando o Arduino UNO como unidade principal de processamento, responsável por interpretar os comandos dos botões, controlar os LEDs, atualizar o display e reproduzir as melodias pelo buzzer.

O projeto foi dividido em etapas: montagem do circuito eletrônico, implementação dos controles, programação das músicas e integração dos componentes.

### 2.1. Materiais utilizados

Para a construção do projeto, foram utilizados os seguintes componentes:

| Componente | Quantidade |
|---|---:|
| Arduino UNO R3 | 1 |
| Protoboard | 1 |
| Display LCD 16x2 com módulo I2C | 1 |
| Botões Push Button | 4 |
| LED verde | 1 |
| LED vermelho | 1 |
| Buzzer passivo | 1 |
| Resistores de 220 Ω | 2 |
| Jumpers | Conforme necessário |
| Cabo USB | 1 |

### 2.2. Funcionamento do sistema

O sistema possui cinco músicas armazenadas na memória do Arduino. O usuário pode navegar entre elas utilizando os botões UP e DOWN, visualizando a seleção no display LCD.

Após selecionar uma música, o botão PLAY inicia sua reprodução. Durante a execução, o usuário pode pausar ou continuar a música utilizando o mesmo botão.

O botão STOP interrompe a reprodução e retorna o sistema ao menu principal.

Os LEDs indicam o estado atual do dispositivo:

- **LED verde:** indica que uma música está sendo reproduzida.
- **LED vermelho:** indica que a reprodução está pausada.
- **LEDs apagados:** indicam que o sistema está no menu principal.

### 2.3. Mapeamento dos componentes

As conexões entre os componentes e o Arduino UNO foram organizadas da seguinte maneira:

| Componente | Pino Arduino |
|---|---|
| Botão UP | D2 |
| Botão DOWN | D3 |
| Botão PLAY/PAUSE | D4 |
| Botão STOP | D5 |
| LED verde | D6 |
| LED vermelho | D7 |
| Buzzer | D8 |
| LCD SDA | A4 |
| LCD SCL | A5 |
| LCD VCC | 5V |
| LCD GND | GND |

Os botões utilizam a configuração `INPUT_PULLUP`, enquanto os LEDs possuem resistores de 220 Ω para limitar a corrente elétrica.

### 2.4. Desenvolvimento da programação

A programação foi desenvolvida no Arduino IDE, utilizando a linguagem C++ baseada na plataforma Arduino.

As melodias foram representadas por estruturas contendo a frequência e a duração de cada nota musical.

Para economizar memória RAM, os vetores das músicas foram armazenados na memória FLASH utilizando `PROGMEM`.

Também foram implementadas funções para:

- Inicializar os componentes eletrônicos.
- Exibir o menu de seleção no display.
- Identificar os comandos dos botões.
- Reproduzir as notas musicais.
- Controlar os estados de reprodução e pausa.
- Interromper a música e retornar ao menu.
- Atualizar os LEDs conforme o estado do sistema.

---

## 🧪 3. Experimentos e resultados

Nesta etapa, são apresentados os testes realizados com o dispositivo, demonstrando seu funcionamento e a interação entre os componentes.

Os testes devem contemplar:

1. Inicialização do Arduino e apresentação do menu no display LCD.
2. Navegação entre as cinco músicas utilizando os botões UP e DOWN.
3. Seleção e reprodução de uma música pelo buzzer.
4. Funcionamento do LED verde durante a reprodução.
5. Funcionamento do LED vermelho durante a pausa.
6. Continuação da música após pressionar PLAY/PAUSE novamente.
7. Interrupção da reprodução e retorno ao menu utilizando STOP.

### 3.1. Registro fotográfico

As imagens abaixo devem ser substituídas pelas fotografias reais do projeto durante o desenvolvimento e os testes.

**Montagem do circuito eletrônico**

![Montagem do projeto](imagens/montagem.jpg)

**Display e menu de seleção**

![Menu do reprodutor](imagens/menu.jpg)

**Reprodução de uma música**

![Projeto em funcionamento](imagens/reproducao.jpg)

**Demonstração em vídeo**

[Inserir link do vídeo de demonstração]

---

## 💻 4. Tecnologias utilizadas

As seguintes tecnologias e ferramentas foram utilizadas no desenvolvimento:

- **Arduino UNO R3:** microcontrolador responsável pelo funcionamento do projeto.
- **Arduino IDE:** ambiente de desenvolvimento utilizado para escrever, compilar e carregar o código.
- **C/C++:** linguagem utilizada para desenvolver a lógica do sistema.
- **LiquidCrystal I2C:** biblioteca utilizada para controlar o display LCD.
- **Git e GitHub:** ferramentas utilizadas para versionamento e armazenamento do código-fonte.

---

## 🎯 5. Conclusão

O desenvolvimento deste projeto proporcionou a aplicação prática de conceitos de programação e eletrônica, integrando software e hardware em um único sistema.

Durante o desenvolvimento, foram trabalhados conceitos como estruturas de dados, funções, vetores, condicionais, repetição, manipulação de memória e controle de componentes eletrônicos.

A utilização do Arduino UNO permitiu compreender melhor a comunicação entre o código desenvolvido e os componentes físicos, demonstrando como a programação pode ser aplicada na construção de dispositivos interativos.

O projeto também contribuiu para o desenvolvimento de habilidades relacionadas à resolução de problemas, organização do código e integração de diferentes tecnologias.

---

## 👨‍💻 6. Integrantes

- **Pedro Lucas Farias**
- **[Nome do segundo integrante, se houver]**

**Instituição:** FEI – Centro Universitário FEI  
**Curso:** Ciência da Computação  
**Disciplina:** CCP020 – Digital Experience Ultimate  
**Professor:** Isaac

---

## 📂 7. Estrutura do repositório

```text
reprodutor-musicas-arduino/
│
├── README.md
│
├── src/
│   └── reprodutor_musicas.ino
│
├── imagens/
│   ├── montagem.jpg
│   ├── menu.jpg
│   └── reproducao.jpg
│
└── videos/
    └── demonstracao.mp4
```

---

**Observação:** As fotografias e os vídeos devem corresponder aos testes realmente realizados durante o desenvolvimento do projeto.
