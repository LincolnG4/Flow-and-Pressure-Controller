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

## 1.1 Componentes

| Componente | Unidade | Preço/unidade (R$) | Total | Link | 
|--|--|--|--|--|
|Arduino Mega|1| 200,00 | 200,00  | [Compre](https://www.baudaeletronica.com.br/arduino-mega-2560-compativel-cabo-usb.html) |
| Capacitor 1uF |9| 0,23 | 2,07 | [Compre](https://www.eletrogate.com/capacitor-eletrolitico-1uf-x-50v ) |
| Conector DB9|4|3,00  | 12,00 |  [Compre](https://www.soldafria.com.br/conector-db9-macho-90-graus-solda-placa-p-1709.html) |
| Conector RJ45 |1|4,00 | 4,00  |  |
| Conector Alimentação|1| 2,50 | 2,50  | [Compre](https://www.robocore.net/conector/conector-dc-p4-femea)  |
| Fonte de alimentação 24V |1| 30,00 | 30,00 |  |
| Modulo Leitor de microSD | 1 | 12,00 | 12,00 | [Compre](https://www.eletrogate.com/modulo-micro-sd-card)  |
| Modulo LM2596 StepDown | 1 |15,00  | 15,00  | [Compre](https://www.filipeflop.com/produto/regulador-de-tensao-lm2596-conversor-dc-dc-step-down/) |
| Modulo WiFi ESP8266 |1| 20,00 | 20,00  | [Compre](https://www.eletrogate.com/modulo-wifi-serial-esp8266-esp-01) |
| Resistor 4.7k Ohms |6|0,05| 0,30 | [Compre](https://www.eletrogate.com/resistor-4k7-1-4w-10-unidades ) |
| Resistor 470 Ohms |8| 0,05 |0,40 |[Compre](https://www.eletrogate.com/resistor-4k7-1-4w-10-unidades )  |
| Resistor 220 Ohms |1| 0,05 | 0,05  |[Compre](https://www.eletrogate.com/resistor-4k7-1-4w-10-unidades ) |
| RTC 1302 | 1 | 10,00 |10,00 | [Compre](https://www.eletrogate.com/modulo-real-time-clock-ds1302) |
| Tela Nextion 320x240 |1| 120,00 |120,00  | [Compre](https://pt.aliexpress.com/item/32685352949.html?spm=a2g0o.productlist.0.0.10a12308w7t6Pf&algo_pvid=ee404ca7-f5a8-4c20-a123-40767543fb84&algo_exp_id=ee404ca7-f5a8-4c20-a123-40767543fb84-0&pdp_ext_f=%7B%22sku_id%22%3A%2260390805771%22%7D&pdp_npi=2%40dis%21BRL%21%21102.36%21%21%2133.26%21%21%402101d4a716547459631704164eff88%2160390805771%21sea)|
| Transistor BC337 |4| 0,27 | 1,08 | [Compre](https://www.eletrogate.com/ecfjb2rzm-transistor-npn-bc337)  |
| __TOTAL__ ||  |  __544,40__|  |



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



### 3.2.2 Como instalar uma biblioteca 

 1. Abra o link para o donwload da biblioteca (ex: `https://github.com/itead/ITEADLIB_Arduino_Nextion`) e faça o download do `.ZIP`  
![download](https://sites.northwestern.edu/researchcomputing/files/2021/05/github.png) 
2. Abra o `Arduino IDE` e vá em `Sketch`>`Incluir biblioteca`> `Adicionar biblioteca .ZIP`  
![include](https://www.robocore.net/upload/tutoriais/32_img_2_H.png)
3. Selecione a biblioteca que você fez o download no Passo 1 


### 3.2.3 Como instalar os gerenciador de placa 

#### Arduino 

1. Abra o `Arduino IDE` e vá em `Ferramentas`>`Placa`>`Gerenciador de placas`  

![Como programar o NodeMCU com Arduino IDE ](https://www.robocore.net/upload/tutoriais/162_img_3_H.png)
2. Procure pela placa `Arduino AVR Boards` e selecione a versão `1.8.3`  
_______
#### ESP01
1.Abra o `Arduino IDE` e vá em `Arquivos`>`Preferências`  
2. No campo `URLs adicionais de Gerenciadores de Placas`, insira o link  `http://arduino.esp8266.com/stable/package_esp8266com_index.json`  

![Como programar o NodeMCU com Arduino IDE](https://www.robocore.net/upload/tutoriais/162_img_2_H.png)
3. Vá em `Ferramentas`>`Placa`>`Gerenciador de placas`  
4. Procure pela placa `esp8266 by ESP8266 community` e selecione a versão `2.4.3`  
 
## 3.3  Implementando firmware
Com todas as bibliotecas configuradas, é possível desenvolver e carregar o código para o  Arduino, Esp e Nextion.

`Faça o download desse respositório e descompacte em alguma pasta do seu computado`:
![Download repositorio](
https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/img/repoDownlod.PNG?raw=true)

### Arduino

- Abra o [`Arduino IDE`](#software-utilizados) em seu computador e vá em `Arquivo` > `Abrir`
- Vá até a pasta onde voce descompactou o repositório repositório, navegue por `Flow-and-Pressure-Controller-main` > `project` > `main.ino` 

- Conecte o `Arduino Mega` via porta `USB`
- No 
