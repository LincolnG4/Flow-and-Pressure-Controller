# Protótipo de controle de fluxo de massa e leitura de pressão

<img src="https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/img/visao_geral.png?raw=true" data-canonical-src="Projeto" width="500" height="600" />

O objetivo principal deste trabalho é desenvolver um protótipo de um sistema, composto de _hardware_, _firmware_ e _software_, para controle do fluxo e pressão de gases durante tratamento de materiais.

Os objetivos :

- Desenvolver um protótipo de um sistema de controle e alimentação de 4 _MFC_ MKS 500 _SCCM_ e leitura de pressão através de 1 Pirani APGX-M-NW16/ALI Edwards;

-  Desenvolver modos de funcionamento programados, tais como: liga/desliga de coleta sensores, pre-sets de gases comuns e alerta de erro;
- Desenvolver a placa de circuito impresso;
- Desenvolver a coleta de dados dos sensores.

## Wish list
Lista de desejos para o futuro do projeto:  
- [ ] Estudo mais detalhado dos resultados dos sinais aplicando metrologia; _`Em desenvolvimento`_
- [ ]  Sofware em Python para concatenar e gerar um grafico interativo; _`Em desenvolvimento`_
- [ ] Utilização de um hardware com Wi-Fi embutido e com mais de um núcleo, assim, permitindo multitarefas;
- [ ] Utilização de um processador de fácil integração na placa PCB;
- [ ] Otimização da programação; _`Em desenvolvimento`_
- [ ] Garantir a não utilização de um hardware genérico;
- [ ] Utilização de um roteador de internet dentro do laboratório;
- [ ] Remodelar o case para montagem por encaixe clicáveis ao invés de parafusos; _`Em desenvolvimento`_
- [ ] Função para alteração de data e hora do RTC; _`Em desenvolvimento`_

 
# 1. Visão Geral

 - Fonte de alimentação: 24 V ( **O módulo Step-Down aceita até 40 V de entrada, porém é necessário desmonta-lo e regular através do parafuso a saída para 7V** )
 - Sinal de saída: 0-5 V
 - Dimensões: 165,5 x 74,6 x 41 mm
 - Tela: Nextion Touch 320x240
 - Armazenamento: cartão microSD até 64 GB e 15GB em nuvem
 - Portas: 4 entradas DB9-macho, 1 entrada RJ45-femea e uma porta usb (debug)

# 2. Software Utilizados

| SOFTWARE        | DESCRIÇÃO|DOWNLOAD |
|----------------|-------------------------------|-------------------------------|
|`Cura3D`|Software para impressão 3D|https://ultimaker.com/software/ultimaker-cura|
|`FreeCAD 0.19`|CAD|https://www.freecadweb.org/downloads.php	|
|`Kicad 6.0`|Eletrônica e PCB |https://www.kicad.org/download/|
|`Nextion Editor`|Edição da interface LCD|https://nextion.tech/nextion-editor/|
|`Photopea`|Edição de fotos|https://photopea.com/|
|`Schematic editor`|Auto routing para trilha PCB|https://layouteditor.com/download|
|`Visual Studio Code e/ou Arduino IDE`|Edição de codigos|https://code.visualstudio.com / https://www.arduino.cc/en/software/download|

# 3. Passo a Passo de construção e instalação
## 3.1 Impressão do  case 3D

Faça o download dos arquivos para impressão 3D:
- [Gabinete](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/Hardware/CAD/case_bottom-Body.stl)
- [Tampa](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/Hardware/CAD/case_top-Body.stl)

Recomendações:

- Materiais: PLA ou ABS;
- Confira a [tabela de temperatura](https://3dlab.com.br/temperatura-na-impressao-3d/) para o bico para cada material;
- Configuração através do software [Cura 3D](#software-utilizados):  




![parameters](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/img/parametro3d.jpeg?raw=true)

## 3.2 Configurando seu ambiente de programação
 Faça o download das bibliotecas a seguir e siga o passo a passo para configuração do seu ambiente de desenvolvimento.
 
### 3.2.1 Bibliotecas Externas Utilizadas 

#### Arduino 

Versão da placa: Arduino AVR Boards 1.8.3
| SOFTWARE        | DESCRIÇÃO|DOWNLOAD |
|----------------|-------------------------------|-------------------------------|
|`ITEADLIB Arduino`|Auxiliar no controle da tela LCD|https://github.com/itead/ITEADLIB_Arduino_Nextion|
|`ArduinoRTClibrary`|Controle do modulo RTC|https://github.com/chrisfryer78/ArduinoRTClibrary|
|`Timelib`|Manipulação time string|https://github.com/PaulStoffregen/Time|
|`UnixTime`|Manipulação de timestamp|https://github.com/GyverLibs/UnixTime|
_______

#### ESP01 

Versão da placa: esp8266 2.4.3
| SOFTWARE        | DESCRIÇÃO|DOWNLOAD |
|----------------|-------------------------------|-------------------------------|
|`WiFi Manager`|Auxiliar no controle do WiFi|https://github.com/tzapu/WiFiManager|



### 3.2.1 Como instalar uma biblioteca 

 1. Abra o link para o donwload da biblioteca (ex: `https://github.com/itead/ITEADLIB_Arduino_Nextion`) e faça o download do `.ZIP`  
![download](https://sites.northwestern.edu/researchcomputing/files/2021/05/github.png) 
2. Abra o `Arduino IDE` e vá em `Sketch`>`Incluir biblioteca`> `Adicionar biblioteca .ZIP`  
![include](https://www.robocore.net/upload/tutoriais/32_img_2_H.png)
3. Selecione a biblioteca que você fez o download no Passo 1 


### 3.2.2 Como instalar os gerenciador de placa 

#### Arduino 

1. Abra o `Arduino IDE` e vá em `Ferramentas`>`Placa`>`Gerenciador de placas`  

![Como programar o NodeMCU com Arduino IDE - Tutoriais - RoboCore](https://www.robocore.net/upload/tutoriais/162_img_3_H.png)
2. Procure pela placa `Arduino AVR Boards` e selecione a versão `1.8.3`  
_______
#### ESP01
1.Abra o `Arduino IDE` e vá em `Arquivos`>`Preferências`  
2. No campo `URLs adicionais de Gerenciadores de Placas`, insira o link  `http://arduino.esp8266.com/stable/package_esp8266com_index.json`  

![Como programar o NodeMCU com Arduino IDE - Tutoriais - RoboCore](https://www.robocore.net/upload/tutoriais/162_img_2_H.png)
3. Vá em `Ferramentas`>`Placa`>`Gerenciador de placas`  
4. Procure pela placa `esp8266 by ESP8266 community` e selecione a versão `2.4.3`  
 

