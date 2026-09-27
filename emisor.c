#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pcap.h>

// MAC de mi computadora
unsigned char mac_origen[6] = {
    0x5C, 0x61, 0x99, 0x2C, 0x75, 0x99
};

// Por ahora mandaremos la trama por broadcast
unsigned char mac_destino[6] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

int main(int argc, char *argv[])
{
    // Revisamos que se haya escrito un mensaje
    if (argc != 2) {
        printf("Uso: %s <mensaje>\n", argv[0]);
        return 1;
    }

    // Interfaz que vamos a usar
    char interfaz[] = "wlp1s0";

    // Aqui se guarda el error si algo sale mal
    char errbuf[PCAP_ERRBUF_SIZE];

    // Abrimos la interfaz con libpcap
    pcap_t *handle = pcap_open_live(interfaz, BUFSIZ, 1, 1000, errbuf);

    // Revisamos que si se haya podido abrir
    if (handle == NULL) {
        printf("No se pudo abrir la interfaz %s\n", interfaz);
        printf("Error: %s\n", errbuf);
        return 1;
    }

    printf("Interfaz %s abierta correctamente\n", interfaz);

    // Revisamos que el mensaje quepa en la trama
    if (strlen(argv[1]) > 45) {
        printf("El mensaje es demasiado largo\n");
        pcap_close(handle);
        return 1;
    }

    // La trama tendra 60 bytes
    unsigned char trama[60];

    // Primero la llenamos de ceros
    memset(trama, 0, sizeof(trama));

    // Los primeros 6 bytes son la MAC destino
    memcpy(trama, mac_destino, 6);

    // Los siguientes 6 bytes son la MAC origen
    memcpy(trama + 6, mac_origen, 6);

    // Usaremos un EtherType experimental
    unsigned short ethertype = htons(0x88B5);
    memcpy(trama + 12, &ethertype, 2);

    // Ponemos el mensaje despues del encabezado Ethernet
    memcpy(trama + 14, argv[1], strlen(argv[1]));

    printf("Mensaje guardado en la trama: %s\n", trama + 14);

    // Mandamos los 60 bytes de la trama
    if (pcap_sendpacket(handle, trama, sizeof(trama)) != 0) {
        printf("Error al enviar la trama: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        return 1;
    }

    printf("Trama enviada correctamente\n");

    // Ya que terminamos, cerramos la interfaz
    pcap_close(handle);

    return 0;
}