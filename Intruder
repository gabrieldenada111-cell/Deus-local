#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>

using namespace std;

const string RESET   = "\033[0m";
const string VERDE   = "\033[32m";
const string VERMELHO= "\033[31m";
const string CIANO   = "\033[36m";
const string AMARELO = "\033[33m";
const string NEGRITO = "\033[1m";

unsigned long long total_pacotes_capturados = 0;
int socket_raw;

void encerrar_sniffer(int sinal) {
    cout << "\n\n" << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << AMARELO << NEGRITO << "             [ ZODIAC INTRUDER - SESSÃO ENCERRADA ]" << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << CIANO << "[*] Total de pacotes interceptados no Palácio: " << total_pacotes_capturados << RESET << endl;
    cout << VERMELHO << "[+] Visão divina desligada com sucesso." << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    close(socket_raw);
    exit(sinal);
}

int main() {
    signal(SIGINT, encerrar_sniffer);
    system("clear");

    // Design Exclusivo do Palácio Imperial do ZODIAC
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << VERDE << "                 /\\                                /\\" << endl;
    cout << VERDE << "                /__\\      [ ZODIAC INTRUDER ]     /__\\" << endl;
    cout << VERDE << "               /\\  /\\                             /\\  /\\" << endl;
    cout << VERDE << "              /__\\/__\\          /\\   /\\          /__\\/__\\" << endl;
    cout << VERDE << "             /\\  /\\  /\\        /__\\_/__\\        /\\  /\\  /\\" << endl;
    cout << VERDE << "            /__\\/__\\/__\\      /\\  /|\\  /\\      /__\\/__\\/__\\" << endl;
    cout << VERDE << "            | _ _ _ _ _|     /__\\/_|_\\/__\\     | _ _ _ _ _|" << endl;
    cout << VERDE << "            |  _   _  |      |   _ _ _   |     |  _   _  |" << endl;
    cout << VERDE << "            | | | | | |      |  | | | |  |     | | | | | |" << endl;
    cout << VERDE << "            | |_| |_| |      |  |_|_|_|  |     | |_| |_| |" << endl;
    cout << VERDE << "            | _ _ _ _ |______|___________|_____| _ _ _ _ |" << endl;
    cout << VERDE << "            | |     | |      |     |     |     | |     | |" << endl;
    cout << VERDE << "            | |     | |      |     |     |     | |     | |" << endl;
    cout << VERDE << "            |_|_____|_|______|_____|_____|_____|_|_____|_|" << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;

    cout << CIANO << "[*] Conectando Raw Socket direto na placa de rede do Linux..." << RESET << endl;

    // Captura TODOS os pacotes de rede (IPPROTO_TCP escuta conexões abertas)
    socket_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    
    if (socket_raw < 0) {
        cout << VERMELHO << NEGRITO << "\n[!] ERRO: Visão Divina Negada! Acesse como ROOT administrativo!" << RESET << endl;
        cout << AMARELO << "[*] Execute usando: sudo ./Intruder\n" << RESET << endl;
        return 1;
    }

    cout << VERDE << "[+] OLHO DE DEUS ATIVADO! Interceptando tráfego total da rede local...\n" << RESET << endl;

    // Aloca o buffer na memória
    unsigned char buffer[65536];

    while (true) {
        struct sockaddr_in fonte;
        socklen_t tamanho_fonte = sizeof(fonte);
        
        // Puxa o frame "cru" direto do cabo/Wi-Fi
        int tamanho_pacote = recvfrom(socket_raw, buffer, sizeof(buffer), 0, (struct sockaddr*)&fonte, &tamanho_fonte);
        
        if (tamanho_pacote < 0) {
            continue;
        }

        total_pacotes_capturados++;

        // Estrutura o mapeamento do cabeçalho IP
        struct iphdr *ip_cabecalho = (struct iphdr*)buffer;

        struct sockaddr_in ip_origem, ip_destino;
        memset(&ip_origem, 0, sizeof(ip_origem));
        ip_origem.sin_addr.s_addr = ip_cabecalho->saddr;

        memset(&ip_destino, 0, sizeof(ip_destino));
        ip_destino.sin_addr.s_addr = ip_cabecalho->daddr;

        string protocolo_nome = "OUTRO";
        if (ip_cabecalho->protocol == 6)  protocolo_nome = "TCP (Jogos/Web)";
        if (ip_cabecalho->protocol == 17) protocolo_nome = "UDP (Stream/DNS)";
        if (ip_cabecalho->protocol == 1)  protocolo_nome = "ICMP (Ping)";

        // Saída hacker ultra-detalhada mostrando quem está conectado e o que está fazendo
        cout << VERDE << "[👁️ INTERCEPTADO] "
             << CIANO << "Quem enviou: " << AMARELO << inet_ntoa(ip_origem.sin_addr) 
             << CIANO << " ➔ Destino: " << AMARELO << inet_ntoa(ip_destino.sin_addr)
             << VERDE << " | Tipo: " << NEGRITO << protocolo_nome 
             << RESET << VERDE << " | Tamanho: " << tamanho_pacote << " bytes" << RESET << endl;
    }

    return 0;
}
