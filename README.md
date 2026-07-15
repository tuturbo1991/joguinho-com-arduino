# joguinho-com-arduino
Um joguinho programado no Arduino com visualização em vídeo analógico.

# Ligação com conector AV

As imagens abaixo foram retiradas do tutorial do site [Arduino Inovação](https://arduinovacao.blogspot.com/2016/07/uso-da-biblioteca-tvout-com-arduino_15.html). Os créditos são do autor. Obrigado!

### Visão geral e resistores necessários

[Diagrama geral](tvout_resistores.png)

Dois resistores são necessários na conexão de vídeo:

- 1K Ohm: porta 9 do UNO, porta 11 do MEGA
- 470 Ohm: porta 7 do UNO, porta 29 do MEGA

A ligação do conector de áudio não necessita de resitor, basta ligar um fio diretado do Arduino (porta 11 do UNO, porta 10 do MEGA) ao conector central do plug. Todos os shields (parte externa do plug) devem compartilhar do mesmo ground.

### Ligação no Arduino UNO

[Ligação no Arduino UNO](tvout_uno.png)

### Ligação no Arduino MEGA

[Ligação no Arduino MEGA](tvout_mega.png)
