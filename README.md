# Protótipo de controle de fluxo de massa e leitura de pressão

`![Projeto](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/a507edb70d68dfbb9c46d5f693ae8e5e239c7549/img/visao_geral.png = 500x450)`


O objetivo principal deste trabalho é desenvolver um protótipo de um sistema, composto de _hardware_, _firmware_ e _software_, para controle do fluxo e pressão de gases durante tratamento de materiais.

Os objetivos :

- Desenvolver um protótipo de um sistema de controle e alimentação de 4 _MFC_ MKS 500 _SCCM_ e leitura de pressão através de 1 Pirani APGX-M-NW16/ALI Edwards;

-  Desenvolver modos de funcionamento programados, tais como: liga/desliga de coleta sensores, pre-sets de gases comuns e alerta de erro;
- Desenvolver a placa de circuito impresso;
- Desenvolver a coleta de dados dos sensores.

# Software Utilizados

| SOFTWARE        | DESCRIÇÃO|DOWNLOAD |
|----------------|-------------------------------|-------------------------------|
|`Cura3D`|Software para impressão 3D|https://ultimaker.com/software/ultimaker-cura|
|`FreeCAD 0.19`|CAD|https://www.freecadweb.org/downloads.php	|
|`Kicad 6.0`|Eletrônica e PCB |https://www.kicad.org/download/|
|`Nextion Editor`|Edição da interface LCD|https://nextion.tech/nextion-editor/|
|`Photopea`|Edição de fotos|photopea.com/|
|`Schematic editor`|Auto routing para trilha PCB|https://layouteditor.com/download|
|`Visual Studio Code e/ou Arduino IDE`|Edição de codigos|https://code.visualstudio.com / https://www.arduino.cc/en/software/download|




# Visão Geral

 - Fonte de alimentação: 24 V ( **O módulo Step-Down aceita até 40 V de entrada, porém é necessário desmonta-lo e regular através do parafuso a saída para 7V** )
 - Sinal de saída: 0-5 V
 - Dimensões: 165,5 x 74,6 x 41 mm
 - Tela: Nextion Touch 320x240
 - Armazenamento: cartão microSD até 64 GB e 15GB em nuvem
 - Portas: 4 entradas DB9-macho, 1 entrada RJ45-femea e uma porta usb (debug)

# Passo a Passo de instalação
## Montagem do case

Faça o download dos arquivos para impressão 3D:
- [Gabinete](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/Hardware/CAD/case_bottom-Body.stl)
- [Tampa](https://github.com/LincolnG4/Flow-and-Pressure-Controller/blob/main/Hardware/CAD/case_top-Body.stl)

Recomendações de configuração através do software Cura 3D:



## Compilar
