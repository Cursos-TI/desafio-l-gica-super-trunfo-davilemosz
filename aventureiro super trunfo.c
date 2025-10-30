/*
 * Super Trunfo - Comparação de Cartas
 * 
 * Este programa implementa um jogo simplificado de Super Trunfo com cidades brasileiras.
 * Compara duas cartas pré-definidas com base em um atributo numérico escolhido no código.
 * 
 * Atributos das cartas:
 * - Estado (string)
 * - Código (string)
 * - Nome da cidade (string)
 * - População (int)
 * - Área (float)
 * - PIB (float)
 * - Pontos turísticos (int)
 * 
 * Atributos calculados:
 * - Densidade populacional (população/área) - menor valor vence
 * - PIB per capita (PIB/população) - maior valor vence
 */                                         

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <locale.h>  // Para suporte a acentos

// Constantes para o menu
#define COMPARAR_POPULACAO 1
#define COMPARAR_AREA 2
#define COMPARAR_PIB 3
#define COMPARAR_DENSIDADE 4
#define COMPARAR_PIB_CAPITA 5
#define COMPARAR_PONTOS_TURISTICOS 6
#define COMPARAR_TUDO 7
#define COMPARAR_AVANCADO 8
#define SAIR 0

// Estrutura para armazenar os dados de uma carta
typedef struct {
    char estado[3];      // Sigla do estado (2 caracteres + '\0')
    char codigo[11];     // Código da carta (10 caracteres + '\0')
    char nome[50];       // Nome da cidade
    int populacao;       // Número de habitantes
    float area;          // Área em km²
    float pib;           // PIB em milhões de reais
    int pontos_turisticos;
    float densidade;     // Calculado: população/área
    float pib_per_capita; // Calculado: PIB/população
} Carta;

/*
 * Atributos disponíveis para comparação:
 * 1 = População (maior vence)
 * 2 = Área (maior vence)
 * 3 = PIB (maior vence)
 * 4 = Densidade populacional (menor vence)
 * 5 = PIB per capita (maior vence)
 */

// Função para limpar o buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para ler os dados de uma carta do usuário
void ler_carta(Carta *c) {
    int ret;
    printf("\n=== CADASTRO DE CARTA ===\n");
    
    do {
        printf("\nEstado (sigla com 2 letras, ex: SP): ");
        ret = scanf("%2s", c->estado);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }
        // Converte para maiúsculas
        c->estado[0] = toupper(c->estado[0]);
        c->estado[1] = toupper(c->estado[1]);
        
        if (strlen(c->estado) != 2) {
            printf("A sigla do estado deve ter exatamente 2 letras!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nCódigo da carta (max 10 caracteres): ");
        ret = scanf("%10s", c->codigo);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }
        if (strlen(c->codigo) == 0) {
            printf("O código não pode estar vazio!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nNome da cidade (max 49 caracteres): ");
        ret = scanf(" %49[^\n]", c->nome);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro na leitura. Tente novamente.\n");
            continue;
        }
        if (strlen(c->nome) == 0) {
            printf("O nome da cidade não pode estar vazio!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nPopulação (número inteiro >= 0): ");
        ret = scanf("%d", &c->populacao);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro: digite um número inteiro válido!\n");
            continue;
        }
        if (c->populacao < 0) {
            printf("A população não pode ser negativa!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nÁrea em km² (número > 0): ");
        ret = scanf("%f", &c->area);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro: digite um número válido!\n");
            continue;
        }
        if (c->area <= 0) {
            printf("A área deve ser maior que zero!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nPIB em milhões de R$ (>= 0): ");
        ret = scanf("%f", &c->pib);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro: digite um número válido!\n");
            continue;
        }
        if (c->pib < 0) {
            printf("O PIB não pode ser negativo!\n");
            ret = 0;
        }
    } while (ret != 1);
    
    do {
        printf("\nNúmero de pontos turísticos (>= 0): ");
        ret = scanf("%d", &c->pontos_turisticos);
        limpar_buffer();
        if (ret != 1) {
            printf("Erro: digite um número inteiro válido!\n");
            continue;
        }
        if (c->pontos_turisticos < 0) {
            printf("O número de pontos turísticos não pode ser negativo!\n");
            ret = 0;
        }
    } while (ret != 1);
}

// Função para escolher o atributo de comparação
int escolher_atributo() {
    int escolha;
    do {
        printf("\nEscolha o atributo para comparação:\n");
        printf("1 - População (maior vence)\n");
        printf("2 - Área (maior vence)\n");
        printf("3 - PIB (maior vence)\n");
        printf("4 - Densidade populacional (menor vence)\n");
        printf("5 - PIB per capita (maior vence)\n");
        printf("Sua escolha: ");
        scanf("%d", &escolha);
        limpar_buffer();
        
        if (escolha < 1 || escolha > 5) {
            printf("Opção inválida! Escolha um número de 1 a 5.\n");
        }
    } while (escolha < 1 || escolha > 5);
    
    return escolha;
}

// Calcula densidade populacional e PIB per capita
void calcular_atributos(Carta *carta) {
    // Calcula densidade se área > 0
    if (carta->area > 0) {
        // População dividida pela área para obter hab/km²
        carta->densidade = (float)carta->populacao / carta->area;
    } else {
        carta->densidade = -1;  // Indica erro: área inválida
    }
    
    // Calcula PIB per capita se população > 0
    if (carta->populacao > 0) {
        // Converte PIB de milhões para reais antes de dividir pela população
        // PIB * 1.000.000 (para converter milhões em reais)
        carta->pib_per_capita = (carta->pib * 1000000.0f) / carta->populacao;
    } else {
        carta->pib_per_capita = -1;  // Indica erro: população inválida
    }
}

// Compara duas cartas e retorna 1 se carta1 vence, 2 se carta2 vence, 0 se empate
int comparar_cartas(Carta *carta1, Carta *carta2, int atributo) {
    float valor1, valor2;
    int menor_vence = 0;
    
    // Define qual atributo será comparado
    switch (atributo) {
        case 1:  // População
            valor1 = carta1->populacao;
            valor2 = carta2->populacao;
            break;
        case 2:  // Área
            valor1 = carta1->area;
            valor2 = carta2->area;
            break;
        case 3:  // PIB
            valor1 = carta1->pib;
            valor2 = carta2->pib;
            break;
        case 4:  // Densidade populacional (menor vence)
            valor1 = carta1->densidade;
            valor2 = carta2->densidade;
            menor_vence = 1;
            break;
        case 5:  // PIB per capita
            valor1 = carta1->pib_per_capita;
            valor2 = carta2->pib_per_capita;
            break;
        case 6:  // Pontos turísticos
            valor1 = carta1->pontos_turisticos;
            valor2 = carta2->pontos_turisticos;
            break;
        default:
            return 0;  // Atributo inválido = empate
    }
    
    // Verifica valores inválidos (negativos indicam erro)
    if (valor1 < 0 || valor2 < 0) {
        return 0;  // Empate se algum valor é inválido
    }
    
    // Compara os valores
    if (valor1 == valor2) {
        return 0;  // Empate
    }
    
    if (menor_vence) {
        return (valor1 < valor2) ? 1 : 2;  // Menor vence
    } else {
        return (valor1 > valor2) ? 1 : 2;  // Maior vence
    }
}

// Retorna o nome do atributo sendo comparado
const char* nome_atributo(int atributo) {
    switch (atributo) {
        case 1: return "População";
        case 2: return "Área";
        case 3: return "PIB";
        case 4: return "Densidade Populacional";
        case 5: return "PIB per capita";
        case 6: return "Pontos Turísticos";
        default: return "Atributo desconhecido";
    }
}

// Exibe os dados de uma carta
void exibir_carta(Carta *carta) {
    printf("\n===================================\n");
    printf("Carta: %s\n", carta->nome);
    printf("-----------------------------------\n");
    printf("Estado: %s\n", carta->estado);
    printf("Codigo: %s\n", carta->codigo);
    // Dados básicos
    printf("Populacao: %d habitantes\n", carta->populacao);
    printf("Area: %.2f km²\n", carta->area);
    printf("PIB: R$ %.2f milhoes\n", carta->pib);
    printf("Pontos Turisticos: %d\n", carta->pontos_turisticos);
    
    // Dados calculados
    printf("\nDados Calculados:\n");
    printf("-----------------------------------\n");
    
    if (carta->densidade >= 0) {
        printf("Densidade Populacional: %.2f hab/km²\n", carta->densidade);
        printf("(%.2f habitantes por quilômetro quadrado)\n", carta->densidade);
    } else {
        printf("Densidade Populacional: Não calculável (área = 0)\n");
    }
    
    if (carta->pib_per_capita >= 0) {
        printf("PIB per capita: R$ %.2f\n", carta->pib_per_capita);
        printf("(R$ %.2f reais por habitante)\n", carta->pib_per_capita);
    } else {
        printf("PIB per capita: Não calculável (população = 0)\n");
    }
    printf("===================================\n");
}

// Função para exibir o menu principal
int exibir_menu() {
    int opcao;
    printf("\n====================================\n");
    printf("            MENU PRINCIPAL          \n");
    printf("====================================\n");
    printf("1. Comparar População\n");
    printf("2. Comparar Área\n");
    printf("3. Comparar PIB\n");
    printf("4. Comparar Densidade Populacional\n");
    printf("5. Comparar PIB per capita\n");
    printf("6. Comparar Pontos Turísticos\n");
    printf("7. Comparar Todos os Atributos\n");
    printf("8. Comparação Avançada\n");
    printf("0. Sair\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    limpar_buffer();
    return opcao;
}

// Função para realizar comparação avançada
int comparacao_avancada(const Carta *c1, const Carta *c2) {
    // Se uma cidade tem mais que o dobro da população da outra e maior PIB
    if (c1->populacao > 2 * c2->populacao && c1->pib > c2->pib) {
        return 1;  // Carta 1 vence
    }
    if (c2->populacao > 2 * c1->populacao && c2->pib > c1->pib) {
        return 2;  // Carta 2 vence
    }
    
    // Se uma cidade tem maior PIB per capita e mais pontos turísticos
    if (c1->pib_per_capita > c2->pib_per_capita && c1->pontos_turisticos > c2->pontos_turisticos) {
        return 1;  // Carta 1 vence
    }
    if (c2->pib_per_capita > c1->pib_per_capita && c2->pontos_turisticos > c1->pontos_turisticos) {
        return 2;  // Carta 2 vence
    }
    
    // Se uma cidade tem menor densidade populacional mas mais pontos turísticos
    if (c1->densidade < c2->densidade && c1->pontos_turisticos > c2->pontos_turisticos) {
        return 1;  // Carta 1 vence (melhor qualidade de vida)
    }
    if (c2->densidade < c1->densidade && c2->pontos_turisticos > c1->pontos_turisticos) {
        return 2;  // Carta 2 vence (melhor qualidade de vida)
    }
    
    return 0;  // Empate se nenhuma condição for atendida
}

// Exibe o resultado da comparação avançada
void exibir_resultado_avancado(const Carta *c1, const Carta *c2) {
    printf("\n=== ANÁLISE AVANÇADA ===\n");
    
    printf("\nPopulação vs PIB:\n");
    printf("%s: %d habitantes, PIB: R$ %.2f milhões\n", c1->nome, c1->populacao, c1->pib);
    printf("%s: %d habitantes, PIB: R$ %.2f milhões\n", c2->nome, c2->populacao, c2->pib);
    
    printf("\nQualidade de Vida:\n");
    printf("%s: %.2f hab/km², %d pontos turísticos\n", c1->nome, c1->densidade, c1->pontos_turisticos);
    printf("%s: %.2f hab/km², %d pontos turísticos\n", c2->nome, c2->densidade, c2->pontos_turisticos);
    
    printf("\nDesenvolvimento:\n");
    printf("%s: R$ %.2f PIB per capita, %d pontos turísticos\n", c1->nome, c1->pib_per_capita, c1->pontos_turisticos);
    printf("%s: R$ %.2f PIB per capita, %d pontos turísticos\n", c2->nome, c2->pib_per_capita, c2->pontos_turisticos);
    
    int resultado = comparacao_avancada(c1, c2);
    printf("\nRESULTADO DA ANÁLISE AVANÇADA:\n");
    if (resultado == 1) {
        printf("🌟 %s demonstra melhor desenvolvimento geral!\n", c1->nome);
    } else if (resultado == 2) {
        printf("🌟 %s demonstra melhor desenvolvimento geral!\n", c2->nome);
    } else {
        printf("🤝 As cidades estão em níveis similares de desenvolvimento.\n");
    }
}

// Exibe o resultado da comparação entre duas cartas
void exibir_comparacao(const Carta *c1, const Carta *c2, int atributo) {
    printf("\n=== COMPARAÇÃO DE CARTAS ===\n");
    printf("Atributo: %s\n\n", nome_atributo(atributo));
    
    switch (atributo) {
        case 1:  // População
            printf("Carta 1 - %s (%s): %d habitantes\n", c1->nome, c1->estado, c1->populacao);
            printf("Carta 2 - %s (%s): %d habitantes\n", c2->nome, c2->estado, c2->populacao);
            break;
        case 2:  // Área
            printf("Carta 1 - %s (%s): %.2f km²\n", c1->nome, c1->estado, c1->area);
            printf("Carta 2 - %s (%s): %.2f km²\n", c2->nome, c2->estado, c2->area);
            break;
        case 3:  // PIB
            printf("Carta 1 - %s (%s): R$ %.2f milhões\n", c1->nome, c1->estado, c1->pib);
            printf("Carta 2 - %s (%s): R$ %.2f milhões\n", c2->nome, c2->estado, c2->pib);
            break;
        case 4:  // Densidade
            if (c1->densidade >= 0) {
                printf("Carta 1 - %s (%s): %.2f hab/km²\n", c1->nome, c1->estado, c1->densidade);
            } else {
                printf("Carta 1 - %s (%s): Densidade não calculável\n", c1->nome, c1->estado);
            }
            if (c2->densidade >= 0) {
                printf("Carta 2 - %s (%s): %.2f hab/km²\n", c2->nome, c2->estado, c2->densidade);
            } else {
                printf("Carta 2 - %s (%s): Densidade não calculável\n", c2->nome, c2->estado);
            }
            break;
        case 5:  // PIB per capita
            if (c1->pib_per_capita >= 0) {
                printf("Carta 1 - %s (%s): R$ %.2f\n", c1->nome, c1->estado, c1->pib_per_capita);
            } else {
                printf("Carta 1 - %s (%s): PIB per capita não calculável\n", c1->nome, c1->estado);
            }
            if (c2->pib_per_capita >= 0) {
                printf("Carta 2 - %s (%s): R$ %.2f\n", c2->nome, c2->estado, c2->pib_per_capita);
            } else {
                printf("Carta 2 - %s (%s): PIB per capita não calculável\n", c2->nome, c2->estado);
            }
            break;
        case 6:  // Pontos turísticos
            printf("Carta 1 - %s (%s): %d pontos turísticos\n", c1->nome, c1->estado, c1->pontos_turisticos);
            printf("Carta 2 - %s (%s): %d pontos turísticos\n", c2->nome, c2->estado, c2->pontos_turisticos);
            break;
    }
    printf("\n");
}

int main() {
    setlocale(LC_ALL, "Portuguese");  // Configura o programa para português
    setbuf(stdout, NULL);  // Evita problemas de buffer no Windows
    
    Carta carta1, carta2;
    int opcao;
    
    system("cls");
    printf("\n====================================\n");
    printf("    SUPER TRUNFO - CIDADES BRASILEIRAS    \n");
    printf("====================================\n\n");
    
    printf("=== CADASTRO DA CARTA 1 ===\n");
    ler_carta(&carta1);
    calcular_atributos(&carta1);
    
    printf("\nPressione ENTER para cadastrar a próxima carta...");
    getchar();
    
    system("cls");
    printf("=== CADASTRO DA CARTA 2 ===\n");
    ler_carta(&carta2);
    calcular_atributos(&carta2);
    
    system("cls");
    printf("=== CARTAS CADASTRADAS ===\n");
    printf("\nCARTA 1:");
    exibir_carta(&carta1);
    printf("\nCARTA 2:");
    exibir_carta(&carta2);
    
    do {
        opcao = exibir_menu();
        int resultado;
        
        switch (opcao) {
            case COMPARAR_POPULACAO:
            case COMPARAR_AREA:
            case COMPARAR_PIB:
            case COMPARAR_DENSIDADE:
            case COMPARAR_PIB_CAPITA:
            case COMPARAR_PONTOS_TURISTICOS:
                system("cls");
                exibir_comparacao(&carta1, &carta2, opcao);
                resultado = comparar_cartas(&carta1, &carta2, opcao);
                printf("\nRESULTADO: ");
                if (resultado == 1) printf("%s venceu!\n", carta1.nome);
                else if (resultado == 2) printf("%s venceu!\n", carta2.nome);
                else printf("Empate!\n");
                break;
                
            case COMPARAR_TUDO:
                system("cls");
                printf("\n=== RESULTADOS DE TODAS AS COMPARAÇÕES ===\n\n");
                
                int vitorias_carta1 = 0, vitorias_carta2 = 0, empates = 0;
                
                // Compara todos os atributos
                for (int i = 1; i <= 6; i++) {
                    exibir_comparacao(&carta1, &carta2, i);
                    resultado = comparar_cartas(&carta1, &carta2, i);
                    if (resultado == 1) vitorias_carta1++;
                    else if (resultado == 2) vitorias_carta2++;
                    else empates++;
                    printf("\n");
                }
                
                // Exibe placar final
                printf("\n=== PLACAR FINAL ===\n");
                printf("%s: %d vitórias\n", carta1.nome, vitorias_carta1);
                printf("%s: %d vitórias\n", carta2.nome, vitorias_carta2);
                printf("Empates: %d\n", empates);
                
                if (vitorias_carta1 > vitorias_carta2)
                    printf("\n🏆 Vencedor geral: %s!\n", carta1.nome);
                else if (vitorias_carta2 > vitorias_carta1)
                    printf("\n🏆 Vencedor geral: %s!\n", carta2.nome);
                else
                    printf("\n🤝 Empate geral!\n");
                break;
                
            case COMPARAR_AVANCADO:
                system("cls");
                exibir_resultado_avancado(&carta1, &carta2);
                break;
                
            case SAIR:
                system("cls");
                printf("\n==================================\n");
                printf("        OBRIGADO POR JOGAR!        \n");
                printf("==================================\n\n");
                break;
                
            default:
                printf("\nOpção inválida! Tente novamente.\n");
        }
        
        if (opcao != SAIR) {
            printf("\nPressione ENTER para continuar...");
            getchar();
            system("cls");
            
            // Mostra as cartas novamente
            printf("=== CARTAS CADASTRADAS ===\n");
            printf("\nCARTA 1:");
            exibir_carta(&carta1);
            printf("\nCARTA 2:");
            exibir_carta(&carta2);
        }
    } while (opcao != SAIR);
    
    return 0;
}