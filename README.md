# [Arduino] Controlador de fluxo de pessoas em uma sala
Utilizando a placa Arduino e dois sonares detectamos a movimentação de pessoas de
uma sala, controlando quantas pessoas entraram e quantas saíram dela. Quando o número de
pessoas em uma sala chega a zero um relatório é criado no servidor com informações sobre a
sala.
A ordem em que os sonares detectam pessoas indicará se uma pessoa entrou ou saiu
da sala. O Arduino será o cliente que coletará os dados, e o Linux o servidor.
