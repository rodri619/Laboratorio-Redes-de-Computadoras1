#include <stdio.h>
#include <pcap.h>

// Esta funcion se ejecuta cada vez que llega una trama
void packet_callback(u_char *user,
                     const struct pcap_pkthdr *header,
                     const u_char *packet)
{
    (void) user;

    // Revisamos que tengamos por lo menos el encabezado Ethernet
    if (header->caplen < 14) {
        return;
    }

    // Sacamos el EtherType de la trama
    unsigned short ethertype = (packet[12] << 8) | packet[13];

    // Ignoramos las tramas que no sean nuestras
    if (ethertype != 0x88B5) {
        return;
    }

    printf("\n--- Trama recibida ---\n");

    // Mostramos la MAC destino
    printf("MAC destino: %02x:%02x:%02x:%02x:%02x:%02x\n",
           packet[0], packet[1], packet[2],
           packet[3], packet[4], packet[5]);

    // Mostramos la MAC origen
    printf("MAC origen: %02x:%02x:%02x:%02x:%02x:%02x\n",
           packet[6], packet[7], packet[8],
           packet[9], packet[10], packet[11]);

    // Mostramos el EtherType
    printf("EtherType: 0x%04x\n", ethertype);

    // El mensaje empieza despues de los 14 bytes del encabezado
    printf("Mensaje: %.*s\n",
           (int)(header->caplen - 14),
           (char *)(packet + 14));
}

int main()
{
    // Interfaz que vamos a escuchar
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
    printf("Escuchando en %s...\n", interfaz);

    // Nos quedamos escuchando las tramas que lleguen
    pcap_loop(handle, -1, packet_callback, NULL);

    // Cerramos la interfaz cuando terminemos
    pcap_close(handle);

    return 0;
}