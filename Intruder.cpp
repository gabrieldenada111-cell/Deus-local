#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <ctime>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <csignal>

using namespace std;

const string RESET   = "\033[0m";
const string VERDE   = "\033[32m";
const string VERMELHO= "\033[31m";
const string CIANO   = "\033[36m";
const string AMARELO = "\033[33m";
const string NEGRITO = "\033[1m";

int socket_raw;
unsigned long long total_analisado = 0;

// Estrutura para rastrear conexões ativas e evitar repetições na tela
struct ConexaoAtiva {
    string ip_origem;
    string ip_destino;
    string host_destino;
    time_t ultimo_tráfego;
    unsigned long long pacotes;
};

map<string, ConexaoAtiva> tabela_conexoes;

void encerrar_sistema(int sinal) {
    cout << "\n\n" << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << AMARELO << NEGRITO << "             [ ZODIAC CONTROLLER - MONITORAMENTO CONCLUÍDO ]" << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << CIANO << "[*] Total de pacotes gerenciados na sessão: " << total_analisado << RESET << endl;
    cout << CIANO << "[*] Conexões únicas mapeadas na tabela: " << tabela_conexoes.size() << RESET << endl;
    cout << VERMELHO << "[+] Sistema de monitoramento finalizado." << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    close(socket_raw);
    exit(sinal);
}

// Função para tentar descobrir o nome do site/servidor através do IP (DNS Reverso)
string descobrir_host(const string& ip_str) {
    struct sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip_str.c_str(), &sa.sin_addr) <= 0) return ip_str;

    char node[NI_MAXHOST];
    // NI_NAMEREQD força a retornar apenas se achar o nome real, se não falha rápido
    if (getnameinfo((struct sockaddr*)&sa, sizeof(sa), node, sizeof(node), NULL, 0, NI_NAMEREQD) == 0) {
        return string(node);
    }
    return "Servidor Externo (IP Direto)";
}

int main() {
    signal(SIGINT, encerrar_sistema);
    system("clear");

    // Design do Palácio de Controle
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;
    cout << VERDE << "                 /\\                                /\\" << endl;
    cout << VERDE << "                /__\\       [ ZODIAC CONTROLLER ]  /__\\" << endl;
    cout << VERDE << "               /\\  /\\                             /\\  /\\" << endl;
    cout << VERDE << "              /__\\/__\\          /\\   /\\          /__\\/__\\" << endl;
    cout << VERDE << "             /\\  /\\  /\\        /__\\_/__\\        /\\  /\\  /\\" << endl;
    cout << VERDE << "            /__\\/__\\/__\\      /\\  /|\\  /\\      /__\\/__\\/__\\" << endl;
    cout << VERDE << "            | _ _ _ _ _|     /__\\/_|_\\/__\\     | _ _ _ _ _|" << endl;
    cout << VERDE << "            |  _   _  |      |   _ _ _   |     |  _   _  |" << endl;
    cout << VERDE << "            | | | | | |      |  | | | |  |     | | | | | |" << endl;
    cout << VERDE << "            | |_| |_| |      |  |_|_|_|  |     | |_| |_| |" << endl;
    cout << VERDE << "            | _ _ _ _ |______|___________|_____| _ _ _ _ |" << endl;
    cout << VERDE << "            |_|_____|_|______|_____|_____|_____|_|_____|_|" << RESET << endl;
    cout << VERMELHO << NEGRITO << "========================================================================" << RESET << endl;

    // Escuta pacotes TCP estruturais na placa de rede
    socket_raw = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    
    if (socket_raw < 0) {
        cout << VERMELHO << NEGRITO << "\n[!] ERRO: Execução negada. Rode o controlador com privilégios administrativos!" << RESET << endl;
        cout << AMARELO << "[*] Comando: sudo ./ZodiacController\n" << RESET << endl;
        return 1;
    }

    cout << VERDE << "[+] CONTROLADOR ONLINE! Gerenciando tráfego de rede ativo...\n" << RESET << endl;

    unsigned char buffer[65536];
    time_t ultima_limpeza = time(0);

    while (true) {
        struct sockaddr_in fonte;
        socklen_t tamanho_fonte = sizeof(fonte);
        
        int tamanho_pacote = recvfrom(socket_raw, buffer, sizeof(buffer), 0, (struct sockaddr*)&fonte, &tamanho_fonte);
        if (tamanho_pacote < 0) continue;

        total_analisado++;
        time_t agora = time(0);

        struct iphdr *ip_cabecalho = (struct iphdr*)buffer;

        struct sockaddr_in src, dst;
        src.sin_addr.s_addr = ip_cabecalho->saddr;
        dst.sin_addr.s_addr = ip_cabecalho->daddr;

        string ip_origem = inet_ntoa(src.sin_addr);
        string ip_destino = inet_ntoa(dst.sin_addr);

        // Cria uma chave única para identificar esse fluxo (Origem -> Destino)
        string chave_fluxo = ip_origem + "->" + ip_destino;

        // Se a conexão não existe na tabela, cadastra e exibe a informação pela primeira vez
        if (tabela_conexoes.find(chave_fluxo) == tabela_conexoes.end()) {
            ConexaoAtiva nova_conexao;
            nova_conexao.ip_origem = ip_origem;
            nova_conexao.ip_destino = ip_destino;
            nova_conexao.ultimo_tráfego = agora;
            nova_conexao.pacotes = 1;
            
            // Tenta descobrir o nome do site resolvendo o IP
            nova_conexao.host_destino = descobrir_host(ip_destino);
            tabela_conexoes[chave_fluxo] = nova_conexao;

            // Imprime o registro de nova conexão ativa na rede
            cout << VERDE << NEGRITO << "[⚡ NOVA CONEXÃO ATIVA] " << RESET 
                 << CIANO << "Aparelho: " << AMARELO << ip_origem 
                 << CIANO << " ➔ Destino: " << AMARELO << nova_conexao.host_destino 
                 << RESET << VERDE << " (" << ip_destino << ")" << RESET << endl;
        } else {
            // Se já existe, apenas atualiza o contador interno na memória silenciosamente (evitando poluir a tela)
            tabela_conexoes[chave_fluxo].ultimo_tráfego = agora;
            tabela_conexoes[chave_fluxo].pacotes++;
        }

        // Rotina de Verificação de Inatividade (Executada a cada 5 segundos para limpar a tela)
        if (agora - ultima_limpeza >= 5) {
            for (auto it = tabela_conexoes.begin(); it != tabela_conexoes.end();) {
                // Se o fluxo ficou mais de 7 segundos sem mandar nenhum byte, avisa que parou
                if (agora - it->second.ultimo_tráfego > 7) {
                    cout << VERMELHO << "[🛑 REQUISIÇÃO ENCERRADA] " << RESET 
                         << AMARELO << it->second.ip_origem << CIANO << " parou de transmitir para " 
                         << AMARELO << it->second.host_destino << RESET << endl;
                    it = tabela_conexoes.erase(it); // Remove da tabela de monitoramento
                } else {
                    ++it;
                }
            }
            ultima_limpeza = agora;
        }
    }

    return 0;
}
