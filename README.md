# Práctica 2 - Capa 2
# Jiménez Cázares Ángel Rodrigo - 321173579

## Descripción

En esta práctica se trabaja con la capa 2 del modelo OSI utilizando la
librería libpcap en C. Se realizaron dos programas, un emisor y un receptor,
con los cuales se puede enviar y recibir una trama Ethernet utilizando
directamente una interfaz de red.

El emisor construye una trama Ethernet que contiene las direcciones MAC de
origen y destino, un EtherType y un mensaje como payload. El receptor se
queda escuchando las tramas que pasan por la interfaz y muestra las que
tienen el EtherType utilizado en esta práctica.

## Archivos

- `emisor.c`: construye y envía una trama Ethernet.
- `receptor.c`: escucha las tramas y muestra la información de las que
  pertenecen a esta práctica.
- `Práctica 2 - Capa 2.pdf`: reporte de la práctica, investigación de las
  funciones de libpcap y resultados obtenidos.

## Payload

El payload es la información útil que se transporta dentro de una trama
o paquete, sin considerar los encabezados utilizados para realizar la
comunicación. En este caso, al trabajar en la capa 2 del modelo OSI,
el payload será el mensaje que colocaremos dentro de una trama Ethernet
para enviarlo desde nuestro programa emisor hacia el receptor.

## Requisitos

Para compilar los programas es necesario tener instalado GCC y la librería
libpcap.

En Debian se puede instalar libpcap con:

```bash
sudo apt install libpcap-dev
```

## Compilación

Para compilar el emisor:

```bash
gcc -Wall -Wextra emisor.c -o emisor -lpcap
```

Para compilar el receptor:

```bash
gcc -Wall -Wextra receptor.c -o receptor -lpcap
```

## Ejecución

Para probar los programas se necesitan dos terminales.

Primero, en una terminal se ejecuta el receptor:

```bash
sudo ./receptor
```

El receptor quedará esperando tramas en la interfaz de red:

```text
Interfaz wlp1s0 abierta correctamente
Escuchando en wlp1s0...
```

Después, desde otra terminal se ejecuta el emisor indicando el mensaje que
se quiere mandar:

```bash
sudo ./emisor "Hola Redes2027-1"
```

El emisor mostrará algo parecido a:

```text
Interfaz wlp1s0 abierta correctamente
Mensaje guardado en la trama: Hola Redes2027-1
Trama enviada correctamente
```

Mientras que el receptor mostrará la información de la trama recibida:

```text
--- Trama recibida ---
MAC destino: ff:ff:ff:ff:ff:ff
MAC origen: 5c:61:99:2c:75:99
EtherType: 0x88b5
Mensaje: Hola Redes2027-1
```

## Funcionamiento

Para identificar las tramas de esta práctica se utiliza el EtherType
`0x88B5`. El emisor coloca el mensaje dentro del payload de una trama
Ethernet y la envía utilizando `pcap_sendpacket`.

El receptor utiliza `pcap_loop` para mantenerse escuchando y revisa el
EtherType de las tramas recibidas. Si encuentra una trama con el valor
`0x88B5`, muestra las direcciones MAC y el mensaje que contiene.

En las pruebas realizadas, el mensaje enviado por el emisor pudo ser
recibido correctamente por el receptor.
