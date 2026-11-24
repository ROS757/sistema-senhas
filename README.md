# Sistema de Senhas de Atendimento

Projeto da disciplina **Algoritmos e Pensamento Computacional** (Ciência da Computação – UDF), desenvolvido em linguagem **C**.

**Autor:** Rafael Oliveira da Silva

## Objetivo

Simular o controle de filas de um atendimento de pacientes. O atendente informa os dados do paciente, o sistema **classifica automaticamente** a categoria do atendimento, **emite uma senha sequencial** e controla a chamada dos pacientes respeitando a prioridade de cada categoria.

## Funcionalidades

O sistema é dividido em quatro módulos, conforme o enunciado.

**Módulo 1 – Registro de senhas**
- Pergunta ao atendente: gênero, idade, se está gestante (apenas quando o gênero é feminino), se é lactante (apenas quando o gênero é feminino), se possui necessidade especial e se possui obesidade.
- Com base nas respostas, identifica a categoria e emite a senha.

**Módulo 2 – Classificação das senhas**

| Código | Categoria | Condição |
|---|---|---|
| N001 | Normal | Pacientes sem condições de prioridade cadastradas |
| P001 | Preferencial | Gestantes, lactantes, pessoas com necessidades especiais, pessoas obesas, mulheres com 60 anos ou mais e homens com 65 anos ou mais |
| U001 | Urgente | Pacientes com idade superior a 80 anos |

As senhas são sequenciais dentro de cada categoria (N001, N002, N003...).

**Módulo 3 – Registro de atendimento**
- Consultar a próxima senha a ser chamada.
- Registrar a chamada do paciente.
- Identificar a senha em atendimento.
- Finalizar o atendimento.
- Atualizar a fila após a conclusão de cada atendimento.

**Módulo 4 – Painel de chamada**
- Senha atualmente chamada.
- Categoria do atendimento.
- Próximas senhas aguardando chamada.
- Quantidade de pacientes em espera por categoria.

**Regras de funcionamento**
- As senhas urgentes são chamadas antes das preferenciais e das normais.
- As senhas preferenciais são chamadas antes das normais.
- Dentro de cada categoria, respeita-se a ordem de emissão.
- Só é possível chamar um novo paciente depois de finalizar o atendimento atual.

## Como compilar e executar

É necessário ter o compilador **GCC** instalado.

```bash
gcc sistema_senhas.c -o sistema_senhas
```

Para executar:

```bash
./sistema_senhas        # Linux e macOS
sistema_senhas.exe      # Windows
```

Também é possível compilar e executar em um compilador online, como o OnlineGDB, colando o conteúdo de `sistema_senhas.c`.

## Estrutura do código

O programa foi **modularizado**: cada função tem uma única responsabilidade e a função `main()` apenas repete o menu e chama as demais funções.

**Bibliotecas usadas:** `stdio.h` (entrada e saída) e `stdlib.h` (função `exit`, usada se a entrada de dados for encerrada).

**Estruturas de armazenamento:** uma matriz `fila[3][MAX]` guarda as senhas emitidas (linha 0 = Urgente, linha 1 = Preferencial, linha 2 = Normal). Os vetores `inicio[]` e `fim[]` controlam, em cada categoria, a próxima senha a chamar e a quantidade de senhas emitidas. As categorias estão em ordem de prioridade, por isso basta percorrê-las da primeira à última para achar a próxima senha.

## Funções implementadas

| Função | Responsabilidade |
|---|---|
| `limparBuffer()` | Descarta o que sobrou na entrada após uma leitura |
| `lerInteiro()` | Lê um número e só aceita valores dentro do intervalo permitido |
| `lerSimNao()` | Faz uma pergunta de Sim/Não e retorna 1 ou 0 |
| `exibirMenu()` | Mostra o menu principal |
| `classificarPaciente()` | Define a categoria (Urgente, Preferencial ou Normal) a partir dos dados do paciente |
| `emitirSenha()` | Gera e guarda a próxima senha da categoria |
| `registrarPaciente()` | Coleta os dados do paciente, classifica e emite a senha |
| `proximaCategoria()` | Descobre qual categoria tem a próxima senha a ser chamada |
| `consultarProxima()` | Mostra a próxima senha, sem chamá-la |
| `chamarPaciente()` | Chama a próxima senha e inicia o atendimento |
| `identificarAtendimento()` | Mostra a senha que está em atendimento |
| `finalizarAtendimento()` | Encerra o atendimento e mostra a fila atualizada |
| `exibirPainel()` | Mostra o painel de chamada |
| `executarOpcao()` | Chama a função correspondente à opção do menu |
| `main()` | Repete o menu até o usuário escolher sair |

## Validação de dados

- Todo número digitado é lido por `lerInteiro()`, que repete a pergunta enquanto o valor for inválido (letras, números fora do intervalo etc.).
- A idade aceita valores de 0 a 120.
- Operações inválidas são tratadas com mensagem, por exemplo: chamar paciente sem ninguém na fila, chamar outro paciente antes de finalizar o atendimento atual ou finalizar quando não há atendimento em andamento.
- Cada categoria comporta até 100 senhas por execução; ao atingir o limite, o sistema avisa que a fila está cheia.

## Exemplo de utilização

```
===== SISTEMA DE SENHAS - ATENDIMENTO =====
1 - Registrar paciente e emitir senha
2 - Consultar proxima senha a ser chamada
3 - Chamar paciente
4 - Identificar senha em atendimento
5 - Finalizar atendimento
6 - Exibir painel de chamada
0 - Sair
Escolha: 1

--- REGISTRO DO PACIENTE ---
Genero: 1-Feminino  2-Masculino
Opcao: 2
Idade: 85
Possui necessidade especial?
1-Sim  2-Nao: 2
Possui condicao de obesidade?
1-Sim  2-Nao: 2

Senha emitida: U001 (Urgente)
```

Painel de chamada com três pacientes aguardando (um de cada categoria), antes de qualquer chamada:

```
============ PAINEL DE CHAMADA ============
Senha chamada: nenhuma

Proximas senhas aguardando chamada:
Urgente (1 em espera): U001
Preferencial (1 em espera): P001
Normal (1 em espera): N001
===========================================
```

Ao escolher a opção 3, o sistema chama primeiro a `U001`, mesmo tendo sido emitida depois de outras senhas:

```
Chamando a senha U001 (Urgente)
```

## Testes realizados

| Entrada | Resultado esperado | Resultado |
|---|---|---|
| Masculino, 85 anos, sem condições | U001 (Urgente) | Passou |
| Masculino, 81 anos | Urgente (acima de 80) | Passou |
| Masculino, 80 anos | Preferencial (não é superior a 80, mas tem 65 ou mais) | Passou |
| Feminino, 60 anos | Preferencial | Passou |
| Feminino, 59 anos, sem condições | Normal | Passou |
| Masculino, 65 anos | Preferencial | Passou |
| Masculino, 64 anos, sem condições | Normal | Passou |
| Feminino, gestante | Preferencial | Passou |
| Feminino, lactante | Preferencial | Passou |
| Paciente com necessidade especial ou obesidade | Preferencial | Passou |
| Digitar letra ou número fora do intervalo | Mensagem de erro e nova pergunta | Passou |
| Chamar paciente com as filas vazias | Aviso "Nao ha senhas aguardando chamada" | Passou |
| Chamar outro paciente sem finalizar o atual | Aviso para finalizar o atendimento | Passou |
| Finalizar sem atendimento em andamento | Aviso "Nao ha atendimento em andamento" | Passou |
| Emitir N, P e U e chamar | Ordem de chamada: U001, P001, N001 | Passou |

### Imagens dos testes

**Teste 1 – Paciente com 85 anos (senha Urgente):**

![Teste urgente](imagens/01-urgente.png)

**Teste 2 – Paciente gestante (senha Preferencial):**

![Teste preferencial](imagens/02-preferencial.png)

**Teste 3 – Paciente sem condições de prioridade (senha Normal):**

![Teste normal](imagens/03-normal.png)

**Teste 4 – Validação de dados (letras e valores fora do intervalo):**

![Teste de validação](imagens/04-validacao.png)

**Teste 5 – Painel de chamada com uma senha de cada categoria:**

![Painel com três senhas](imagens/05-painel-tres-senhas.png)

**Teste 6 – Ordem de chamada (Urgente, Preferencial e depois Normal):**

![Ordem de chamada](imagens/06-ordem-de-chamada.png)

**Teste 7 – Operações com a fila vazia:**

![Fila vazia](imagens/07-fila-vazia.png)

**Teste 8 – Tentar chamar outro paciente sem finalizar o atendimento:**

![Chamar sem finalizar](imagens/08-chamar-sem-finalizar.png)

## Arquivos do repositório

- `sistema_senhas.c`: código-fonte do sistema.
- `README.md`: documentação do projeto.
- `imagens/`: imagens dos testes mostradas neste README.
