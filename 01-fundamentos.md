# Módulo 1: Fundamentos

Neste módulo você escreve o seu primeiro projeto com testes, do zero, seguindo o ciclo do TDD. No caminho, aprende a estrutura de um projeto CMake com GoogleTest, a ler o resultado de uma falha e a diferença entre as duas famílias de verificação do framework, `EXPECT_*` e `ASSERT_*`.

Ao final você terá:

- um projeto com uma função de produção e quatro testes, construído um teste por vez;
- a experiência de ver cada teste falhar antes de passar;
- clareza sobre quando usar `ASSERT_*` e quando usar `EXPECT_*`;
- o hábito de compilar e executar os testes a cada mudança.

**Tempo estimado:** 1h30.

**Pré-requisito:** o [Módulo 0B](00b-ferramentas-vscode.md) concluído, com o VS Code conectado à VM.

## Sumário

- [1. O problema](#1-o-problema)
- [2. Raciocínio antes do código](#2-raciocínio-antes-do-código)
- [3. Preparar o projeto](#3-preparar-o-projeto)
- [4. Primeiro ciclo: um campo](#4-primeiro-ciclo-um-campo)
- [5. Segundo ciclo: vários campos](#5-segundo-ciclo-vários-campos)
- [6. Terceiro ciclo: campo vazio no meio](#6-terceiro-ciclo-campo-vazio-no-meio)
- [7. Quarto ciclo: linha vazia](#7-quarto-ciclo-linha-vazia)
- [8. Desafio](#8-desafio)
- [9. Revisão crítica](#9-revisão-crítica)
- [Na documentação oficial](#na-documentação-oficial)
- [Referência rápida](#referência-rápida)
- [Critérios de domínio](#critérios-de-domínio)

> [!NOTE]
> **Onde trabalhar.** Você vai construir o projeto do zero na pasta `~/pratica/01-fundamentos` da VM, digitando cada arquivo conforme a aula pede. A pasta [`gabarito/01-fundamentos`](gabarito/01-fundamentos) deste repositório tem a versão final, só para consulta: compare com a sua quando terminar. Resista à tentação de copiá-la antes: o aprendizado está em passar pelas etapas.

---

## 1. O problema

> Separar uma linha de texto em campos, usando um caractere delimitador.
>
> Exemplo: `"ana;32;Recife"`, com o delimitador `;`, resulta em `"ana"`, `"32"` e `"Recife"`.

É uma tarefa comum: ler arquivos de configuração, registros de log, dados exportados de planilhas.

## 2. Raciocínio antes do código

Antes de escrever qualquer linha, liste os comportamentos que a função precisa ter e as dúvidas que o requisito deixa em aberto. Isso é **Shift Left** na prática: descobrir as perguntas difíceis agora, quando mudar de ideia não custa nada.

| Situação | Entrada | Resultado esperado |
|---|---|---|
| Um campo, sem delimitador | `"ana"` | `"ana"` |
| Vários campos | `"ana;32;Recife"` | `"ana"`, `"32"`, `"Recife"` |
| Campo vazio no meio | `"ana;;Recife"` | `"ana"`, `""`, `"Recife"` |
| Linha vazia | `""` | **?** |
| Delimitador no fim | `"ana;32;"` | **?** |

Duas linhas estão com interrogação. O requisito não diz o que fazer nesses casos, e qualquer resposta é defensável. Em vez de deixar a implementação decidir por acaso, vamos tomar essas decisões de forma explícita, quando chegarmos a elas.

A tabela é a nossa **lista de tarefas** do TDD. Cada linha vira um ciclo:

1. **Vermelho:** escrever um teste para o próximo comportamento e vê-lo falhar.
2. **Verde:** escrever o mínimo de código para o teste passar.
3. **Refatorar:** melhorar o código, se for preciso, sem mudar o comportamento.

## 3. Preparar o projeto

### 3.1 Criar a pasta

No terminal do VS Code conectado à VM:

```bash
mkdir -p ~/pratica/01-fundamentos
```

O `-p` cria também as pastas intermediárias que faltarem (aqui, a `pratica`) e não reclama se a pasta já existir.

Abra a pasta no VS Code em **File → Open Folder**, digitando `/home/seu_usuario/pratica/01-fundamentos`.

A janela recarrega. No terminal (se não houver um aberto, use **Terminal → New Terminal**), entre na pasta e confirme onde está:

```bash
cd ~/pratica/01-fundamentos && pwd
```

O `cd` entra na pasta, e o `pwd` mostra em qual pasta o terminal está. O resultado deve ser:

```
/home/seu_usuario/pratica/01-fundamentos
```

> [!IMPORTANT]
> **Todos os comandos desta aula partem dessa pasta.** O `.` do `cmake -S . -B build` significa "a pasta atual", e os demais caminhos usados na aula, como `build`, também são relativos a ela. Sempre que abrir um terminal novo, confira com `pwd` antes de continuar.

### 3.2 O `CMakeLists.txt`

Crie o arquivo `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(fundamentos CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.18.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

# Código de produção
add_library(separar separar.cpp)
target_include_directories(separar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(separar PRIVATE -Wall -Wextra -Wpedantic)

# Testes
enable_testing()

add_executable(separar_test separar_test.cpp)
target_link_libraries(separar_test PRIVATE separar GTest::gtest_main)
target_compile_options(separar_test PRIVATE -Wall -Wextra -Wpedantic)

include(GoogleTest)
gtest_discover_tests(separar_test)
```

O que cada parte faz:

| Trecho | Função |
|---|---|
| `cmake_minimum_required(VERSION 3.16)` | A versão **mínima** de CMake que o projeto exige: a instalada precisa ser esta ou uma mais nova (na VM, a 3.26) |
| `project(fundamentos CXX)` | Dá nome ao projeto e declara a linguagem |
| `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` | Grava as opções de compilação em `build/compile_commands.json`, que o VS Code usa para analisar o código ([Módulo 0B, passo 5.9](00b-ferramentas-vscode.md#59-configurar-a-análise-de-código-do-editor)) |
| `CMAKE_CXX_STANDARD 17` e as duas linhas seguintes | Compila em C++17 puro, o padrão exigido pelo GoogleTest |
| `FetchContent_Declare` e `FetchContent_MakeAvailable` | Baixam o GoogleTest na versão `v1.18.0` e o incluem na compilação |
| `add_library(separar ...)` | O **código de produção** vira uma biblioteca **estática** (`libseparar.a`), separada dos testes. Estática é o padrão do CMake quando não se pede outra coisa |
| `target_include_directories` | Permite que quem usa a biblioteca encontre o `separar.h` |
| `target_compile_options(... -Wall -Wextra -Wpedantic)` | Liga os avisos do compilador, só para o nosso código: `-Wall` e `-Wextra` apontam código suspeito (variável não usada, comparação duvidosa), e `-Wpedantic` aponta o que foge do C++ padrão. Você verá um exemplo na seção 4.3 |
| `add_executable(separar_test ...)` | Cria o programa que executa os testes |
| `target_link_libraries(... separar GTest::gtest_main)` | Liga o programa de testes à nossa biblioteca e ao GoogleTest |
| `gtest_main` | Fornece a função `main()`, que roda todos os testes |
| `enable_testing()` | Liga o suporte a testes do CMake, o que permite usar o `ctest` |
| `include(GoogleTest)`, `gtest_discover_tests(separar_test)` | Depois de cada compilação, executa o `separar_test` pedindo só a **lista** dos testes que ele contém, e registra cada um no `ctest` como um teste separado. É por isso que o `ctest` mostra `Separar.UmCampo` e os demais pelo nome, e não um único teste chamado `separar_test`. O `ctest` é apresentado na seção 4.5 |

> [!NOTE]
> **Por que separar produção e teste?** O código de produção não sabe que os testes existem. Ele é uma biblioteca que poderia ser usada por qualquer programa; o executável de testes é apenas um exemplo de consumidor. É assim em projetos reais e é o que permite ter o mesmo código testado e entregue.

### 3.3 Os arquivos de produção, ainda vazios

Crie o `separar.h` com uma única linha:

```cpp
#pragma once
```

O `#pragma once` impede que o conteúdo do cabeçalho seja incluído mais de uma vez na mesma compilação, o que causaria erros de declaração repetida. É a primeira linha de todo cabeçalho do curso.

E o `separar.cpp`, também com uma linha:

```cpp
#include "separar.h"
```

A estrutura do projeto existe, mas a função ainda não. É de propósito: no TDD, o teste vem antes.

A configuração do projeto fica para a seção 4.2, quando o arquivo de teste já existir.

---

## 4. Primeiro ciclo: um campo

### 4.1 Teste primeiro

Crie o arquivo `separar_test.cpp`:

```cpp
#include "separar.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(Separar, UmCampo)
{
    const auto campos = separar("ana", ';');

    ASSERT_EQ(campos.size(), 1u);
    EXPECT_EQ(campos[0], "ana");
}
```

A anatomia de um teste:

- **`TEST(Separar, UmCampo)`** declara um teste. O primeiro nome agrupa testes relacionados (a *suíte*); o segundo identifica este teste em particular. Juntos, formam o nome completo: `Separar.UmCampo`.
- **A primeira linha do corpo** executa o código que está sendo testado.
- **As linhas seguintes** verificam o resultado:
  - `ASSERT_EQ(a, b)` exige que `a` seja igual a `b`; se não for, registra a falha e **interrompe** o teste.
  - `EXPECT_EQ(a, b)` exige o mesmo; se não for, registra a falha e o teste **continua**.

  A seção 4.4 mostra, na prática, por que essa diferença importa.
- **`1u`** é o número 1 sem sinal (*unsigned*), a mesma família de tipo que `size()` devolve. Aqui, escrever só `1` também funcionaria; usamos `1u` para que os dois lados da comparação sejam do mesmo tipo.

> [!NOTE]
> **Sublinhados vermelhos no editor.** Ao salvar o arquivo, o VS Code provavelmente vai sublinhar trechos em vermelho. Há dois motivos, e os dois são esperados neste ponto:
>
> - **O projeto ainda não foi configurado.** Sem o arquivo `build/compile_commands.json`, que só aparece na próxima seção, o editor não sabe que o projeto usa C++17 e analisa os cabeçalhos do GoogleTest de forma errada. É isso que produz mensagens estranhas como *expression must have bool type* em `ASSERT_EQ` e `EXPECT_EQ`.
> - **A função `separar` ainda não foi declarada.** O editor antecipa o mesmo erro que o compilador vai apontar na próxima seção.
>
> Depois da configuração, na seção 4.2, deve restar só o segundo motivo; e na seção 4.3, quando a função for declarada, nenhum. Se a mensagem *expression must have bool type* continuar depois de configurar, confira o [passo 5.9 do Módulo 0B](00b-ferramentas-vscode.md#59-configurar-a-análise-de-código-do-editor).

### 4.2 Configurar e executar: não compila

Com todos os arquivos no lugar, configure o projeto:

```bash
cmake -S . -B build
```

Na primeira vez, o CMake baixa o GoogleTest. As últimas linhas devem ser parecidas com estas (os tempos variam):

```
-- Configuring done (2.6s)
-- Generating done (0.0s)
-- Build files have been written to: /home/seu_usuario/pratica/01-fundamentos/build
```

A configuração só precisa ser repetida se o `CMakeLists.txt` mudar ou se um arquivo citado nele for criado depois. Daqui em diante, basta compilar:

```bash
cmake --build build -j2
```

A compilação para com este erro:

```
separar_test.cpp:10:25: error: ‘separar’ was not declared in this scope
```

**Isso não é um teste falhando.** O teste nem chegou a existir como programa: o compilador recusou o código porque a função `separar` não foi declarada. É a primeira distinção importante do curso:

| Situação | Quem reclama | O que significa |
|---|---|---|
| Erro de compilação | O compilador | O código está incompleto ou incorreto como C++. Nenhum teste foi executado |
| Teste falhando | O GoogleTest | O programa compilou e rodou, mas o comportamento não é o esperado |

Lembre-se do ciclo da seção 2: a etapa **vermelha** é ver o teste **falhar**. Um erro de compilação ainda não é essa etapa, porque nenhum teste chegou a ser executado. Para chegar a ela, falta fazer o código compilar.

### 4.3 O esboço: a etapa vermelha

Substitua o conteúdo do `separar.h`, declarando a função:

```cpp
#pragma once

#include <string>
#include <vector>

// Separa uma linha de texto em campos, usando o delimitador informado.
// Exemplo: separar("ana;32;Recife", ';') devolve {"ana", "32", "Recife"}.
std::vector<std::string> separar(const std::string& linha, char delimitador);
```

E o do `separar.cpp`, com um esboço que devolve uma lista vazia:

```cpp
#include "separar.h"

std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    return {};
}
```

Compile e execute os testes com um único comando:

```bash
cmake --build build -j2 && ./build/separar_test
```

> [!TIP]
> **Use sempre o `&&`.** Ele só executa os testes se a compilação der certo. O motivo: quando a compilação falha, o executável da compilação **anterior** continua na pasta `build`. Sem o `&&` (por exemplo, separando os comandos com `;` ou digitando um depois do outro), o `./build/separar_test` roda esse executável antigo, e você analisa o resultado de um código que não é o que está na tela, sem perceber.

Durante a compilação aparecem dois avisos. Cada um traz o arquivo, a linha e a coluna, a mensagem e, abaixo, o trecho do código com uma marca sob o parâmetro em questão:

```
separar.cpp: In function ‘std::vector<std::__cxx11::basic_string<char> > separar(const string&, char)’:
separar.cpp:3:53: warning: unused parameter ‘linha’ [-Wunused-parameter]
 std::vector<std::string> separar(const std::string& linha, char delimitador)
                                  ~~~~~~~~~~~~~~~~~~~^~~~~
separar.cpp:3:65: warning: unused parameter ‘delimitador’ [-Wunused-parameter]
 std::vector<std::string> separar(const std::string& linha, char delimitador)
                                                            ~~~~~^~~~~~~~~~~
```

Na tela, os nomes de arquivo aparecem com o caminho completo, como `/home/seu_usuario/pratica/01-fundamentos/separar.cpp`.

São esperados: o esboço recebe os parâmetros e ainda não os usa. Os avisos vão desaparecer quando a implementação estiver completa. Não os ignore no dia a dia, porque costumam apontar erros reais.

> [!TIP]
> **Onde procurar os avisos.** Eles aparecem no meio da saída da compilação, **antes** dos resultados dos testes, e é fácil perdê-los de vista. Além disso, só aparecem na compilação em que o arquivo é recompilado: nas seguintes, se o `separar.cpp` não mudou, ele não é compilado de novo, e os avisos não se repetem. Para vê-los outra vez, atualize a data do arquivo com o `touch` antes do comando de sempre:
>
> ```bash
> touch separar.cpp && cmake --build build -j2 && ./build/separar_test
> ```
>
> Os avisos aparecem algumas linhas acima de `Running main() from ...`, que marca o início dos testes. Entre os dois ficam as linhas `Linking ...` e `Built target ...` da compilação.

O teste é executado e falha:

```
[ RUN      ] Separar.UmCampo
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:12: Failure
Expected equality of these values:
  campos.size()
    Which is: 0
  1u
    Which is: 1

[  FAILED  ] Separar.UmCampo (0 ms)
```

**Como ler uma falha.** Esse formato vai acompanhar você pelo curso inteiro:

| Linha | O que informa |
|---|---|
| `separar_test.cpp:12: Failure` | O arquivo e a linha da verificação que falhou |
| `Expected equality of these values:` | O tipo de verificação: igualdade |
| `campos.size()` e `Which is: 0` | O **primeiro argumento** do `ASSERT_EQ`, escrito como está no código, e o valor que ele teve na execução: `campos.size()` resultou em 0 |
| `1u` e `Which is: 1` | O **segundo argumento** do `ASSERT_EQ`, o valor esperado, e o seu valor: 1 |

Leia assim: "na linha 12, esperava-se que `campos.size()` fosse igual a `1u`, mas valia 0".

Agora sim estamos na etapa vermelha do ciclo: o programa compila, roda, e o teste falha **pelo motivo certo**. A função existe, mas ainda não faz o que deveria.

**As outras linhas da saída.** Na tela aparecem algumas linhas além das mostradas acima. Três delas estão em toda execução:

| Linha | O que significa |
|---|---|
| `Running main() from .../gtest_main.cc` | A função `main()` do programa de testes veio do GoogleTest, pelo `GTest::gtest_main` do `CMakeLists.txt`. Você não precisou escrevê-la |
| `[----------] Global test environment set-up.` | Início da preparação **global**: recursos que podem ser criados uma única vez, antes de todos os testes |
| `[----------] Global test environment tear-down` | Fim: a liberação desses recursos, depois de todos os testes |

As duas últimas aparecem mesmo quando não há nada a preparar, como aqui. O recurso de preparação global é assunto de um módulo mais adiante.

### 4.4 Experimento: `ASSERT` ou `EXPECT`?

O teste usa `ASSERT_EQ` para conferir o tamanho e `EXPECT_EQ` para conferir o conteúdo. A diferença:

| Família | Quando a verificação falha |
|---|---|
| `EXPECT_*` | Registra a falha e **continua** executando o teste |
| `ASSERT_*` | Registra a falha e **interrompe** o teste imediatamente |

Para ver por que isso importa, troque **temporariamente** o `ASSERT_EQ` da linha 12 por `EXPECT_EQ`:

```cpp
    EXPECT_EQ(campos.size(), 1u);
```

Compile e execute:

```bash
cmake --build build -j2 && ./build/separar_test
```

A falha da linha 12 aparece, e logo depois o programa de testes inteiro aborta:

```
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:12: Failure
Expected equality of these values:
  campos.size()
    Which is: 0
  1u
    Which is: 1

Segmentation fault (core dumped)
```

O que aconteceu: com `EXPECT_EQ`, o teste seguiu em frente depois de descobrir que a lista estava vazia, e a linha 13 tentou ler `campos[0]`, uma posição que não existe. Em C++ isso é **comportamento indefinido** (*undefined behavior*, ou **UB**): a linguagem não define o que acontece, e o programa pode fazer qualquer coisa. No nosso ambiente, terminou em falha de segmentação (*segmentation fault*). Como o programa de testes inteiro abortou, se houvesse outros testes depois deste, nenhum deles chegaria a rodar.

**A regra prática:** use `ASSERT_*` quando o resto do teste depende daquela condição, como conferir o tamanho antes de acessar os elementos. Use `EXPECT_*` nos demais casos, porque assim um único teste mostra todas as falhas de uma vez.

Não desfaça o experimento ainda: ele é útil na próxima seção.

### 4.5 O `ctest`: o executor de testes do CMake

Até aqui você executou os testes chamando diretamente o `./build/separar_test`. Existe uma segunda forma, o `ctest`, que você já usou no Módulo 0B.

**O que é.** O `ctest` é o executor de testes que vem junto com o CMake, no mesmo pacote `cmake` instalado no Módulo 0B. Ele executa os testes que o `CMakeLists.txt` registrou: o `enable_testing()` liga esse suporte, e o `gtest_discover_tests` registra cada teste do `separar_test`.

**A diferença para o `./build/separar_test`:**

| | `./build/separar_test` | `ctest --test-dir build` |
|---|---|---|
| O que executa | Os testes de um único programa | Todos os testes registrados no projeto, de todos os programas de teste que ele tiver |
| Como executa | Todos os testes no mesmo processo | **Cada teste num processo separado**: para o `Separar.UmCampo`, ele roda `separar_test --gtest_filter=Separar.UmCampo`. A opção `--gtest_filter` faz o programa executar só os testes cujo nome combina com o filtro; você vai usá-la na seção 5.4 |
| Saída | Detalhada, a do GoogleTest | Um resumo: qual teste passou e qual falhou |
| Bom para | Investigar uma falha | Conferir o projeto inteiro de uma vez |

O processo separado faz diferença justamente no experimento da seção anterior. Com o `EXPECT_EQ` ainda no lugar do `ASSERT_EQ`, execute:

```bash
ctest --test-dir build
```

```
Internal ctest changing into directory: /home/seu_usuario/pratica/01-fundamentos/build
Test project /home/seu_usuario/pratica/01-fundamentos/build
    Start 1: Separar.UmCampo
1/1 Test #1: Separar.UmCampo ..................***Exception: SegFault  0.08 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   0.08 sec

The following tests FAILED:
	  1 - Separar.UmCampo (SEGFAULT)
Errors while running CTest
Output from these tests are in: /home/seu_usuario/pratica/01-fundamentos/build/Testing/Temporary/LastTest.log
Use "--rerun-failed --output-on-failure" to re-run the failed cases verbosely.
```

Como ler:

| Trecho | O que informa |
|---|---|
| `Internal ctest changing into directory: ...` | Por causa do `--test-dir build`, o `ctest` entrou na pasta `build` para trabalhar |
| `Start 1: Separar.UmCampo` | Início do teste número 1 |
| `1/1 Test #1: ... ***Exception: SegFault  0.08 sec` | O teste 1 de 1 terminou com o programa abortado por falha de segmentação, em 0,08 segundo. Um teste que falha normalmente aparece como `***Failed`, e um que passa, como `Passed` |
| `0% tests passed, 1 tests failed out of 1` | O resumo: nenhum teste passou |
| `The following tests FAILED:` | A lista dos testes com problema, com o motivo entre parênteses |
| `Errors while running CTest` | Houve falhas; o `ctest` termina com código de erro, o que interessa a scripts e à integração contínua |
| `Output from these tests are in: .../LastTest.log` | A saída completa de cada teste ficou gravada nesse arquivo |
| `Use "--rerun-failed --output-on-failure" ...` | Uma sugestão: executar de novo só os que falharam, mostrando a saída deles |

Repare que o `ctest` **registrou o aborto como a falha de um teste** e seguiria para o próximo, se houvesse. Como cada teste roda em um processo próprio, um teste que derruba o programa não impede os demais de rodar. Executando o `./build/separar_test` diretamente, o aborto encerraria tudo.

Para ver, junto do resumo, a saída do GoogleTest dos testes que falharam, use a opção `--output-on-failure`:

```bash
ctest --test-dir build --output-on-failure
```

A mensagem de falha aparece logo abaixo da linha do teste:

```
1/1 Test #1: Separar.UmCampo ..................***Exception: SegFault  0.07 sec
Running main() from /home/seu_usuario/pratica/01-fundamentos/build/_deps/googletest-src/googletest/src/gtest_main.cc
Note: Google Test filter = Separar.UmCampo
...
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:12: Failure
Expected equality of these values:
  campos.size()
    Which is: 0
  1u
    Which is: 1
```

A linha `Note: Google Test filter = Separar.UmCampo` mostra o filtro que o `ctest` usou para executar só aquele teste.

Agora sim, **volte a linha 12 para `ASSERT_EQ`** antes de continuar.

### 4.6 Verde: a implementação mínima

O mínimo para este teste passar é devolver a linha inteira como um único campo. Altere o `separar.cpp`:

```cpp
#include "separar.h"

std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    return {linha};
}
```

Parece trapaça, mas é intencional. O teste atual só exige isso, e escrever mais do que o teste exige é escrever código que nenhum teste verifica. Os próximos ciclos vão forçar a implementação a crescer.

```bash
cmake --build build -j2 && ./build/separar_test
```

Na saída da **compilação**, antes dos resultados dos testes, aparece agora só um dos dois avisos da seção 4.3:

```
separar.cpp:3:65: warning: unused parameter ‘delimitador’ [-Wunused-parameter]
```

Na tela ele vem com o trecho do código e a marca sob o parâmetro, como na seção 4.3. O aviso sobre `linha` desapareceu, porque agora a função usa esse parâmetro. O `delimitador` continua sem uso, e o aviso dele permanece.

O teste passa:

```
[ RUN      ] Separar.UmCampo
[       OK ] Separar.UmCampo (0 ms)
...
[  PASSED  ] 1 test.
```

### 4.7 Refatorar

Refatorar é mudar a **estrutura** do código sem mudar o seu **comportamento**: renomear, simplificar, eliminar repetição. Por isso essa etapa só começa com todos os testes passando. São eles que provam, depois de cada mudança, que o comportamento continua o mesmo.

A etapa é obrigatória, mas mudar o código não é. O obrigatório é **avaliar**, e esta lista vai acompanhar você em todos os ciclos:

| Pergunta | Olhe para |
|---|---|
| Os nomes dizem o que as coisas fazem? | A função, as variáveis e os nomes dos testes |
| Há repetição que poderia ser eliminada? | O código de produção e o de teste |
| Há código que nenhum teste exige? | O código de produção |
| O teste está fácil de ler? | A separação entre executar (a chamada) e verificar (as asserções) |
| O compilador está avisando algo? | A saída da compilação |

Aplicada ao código de agora:

- **Nomes:** `separar`, `campos` e `Separar.UmCampo` descrevem bem o que são.
- **Repetição:** não há; o código tem uma linha, e o teste, um só caso.
- **Código sem teste:** não há; a única linha existe porque o teste a exige.
- **Leitura do teste:** a chamada vem primeiro, e as verificações, depois de uma linha em branco.
- **Avisos:** o do `delimitador`. Resolvê-lo agora exigiria usar o delimitador, ou seja, escrever código que nenhum teste pede ainda. O certo é deixá-lo: o próximo ciclo vai exigir esse uso, e o aviso some sozinho.

Conclusão: nada a mudar neste ciclo. Isso não é pular a etapa; é o resultado dela.

---

## 5. Segundo ciclo: vários campos

### 5.1 Vermelho

Acrescente ao final do `separar_test.cpp`:

```cpp

TEST(Separar, VariosCampos)
{
    const auto campos = separar("ana;32;Recife", ';');

    ASSERT_EQ(campos.size(), 3u);
    EXPECT_EQ(campos[0], "ana");
    EXPECT_EQ(campos[1], "32");
    EXPECT_EQ(campos[2], "Recife");
}
```

```bash
cmake --build build -j2 && ./build/separar_test
```

O teste anterior continua passando, e o novo falha:

```
[ RUN      ] Separar.UmCampo
[       OK ] Separar.UmCampo (0 ms)
[ RUN      ] Separar.VariosCampos
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:20: Failure
Expected equality of these values:
  campos.size()
    Which is: 1
  3u
    Which is: 3

[  FAILED  ] Separar.VariosCampos (0 ms)
...
[  PASSED  ] 1 test.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] Separar.VariosCampos
```

Repare que, graças ao `ASSERT_EQ`, o teste parou na linha 20. As três verificações de conteúdo não foram executadas, e o programa não abortou.

Antes de seguir, duas perguntas para você responder sozinho. Anote as respostas; elas serão comparadas no início da seção 5.3.

1. Por que `campos.size()` vale **1**, e não 3? Releia a implementação atual no `separar.cpp`.
2. O que precisa mudar no `separar.cpp` para este teste passar, ou seja, para `campos.size()` valer 3 e cada verificação seguinte encontrar o valor esperado? Descreva a ideia em português, ou já em código.

### 5.2 Mensagens personalizadas

Toda verificação aceita uma mensagem extra, acrescentada com `<<`, que aparece quando ela falha. Experimente na linha 20:

```cpp
    ASSERT_EQ(campos.size(), 3u) << "linha: ana;32;Recife";
```

A falha passa a terminar com:

```
  3u
    Which is: 3
linha: ana;32;Recife
```

A mensagem é útil quando a falha sozinha não deixa claro o contexto, por exemplo em testes que verificam muitos valores parecidos. Pode manter ou remover; o comportamento do teste não muda.

### 5.3 Verde

**A primeira pergunta da seção 5.1.** A implementação atual é `return {linha};`: ela devolve a linha inteira, `"ana;32;Recife"`, como um único campo, sem sequer olhar para o `;`. Por isso `campos.size()` vale 1. O teste `UmCampo` passava só porque `"ana"` não tem delimitador; o `VariosCampos` é o primeiro teste a exigir que a função **procure** o delimitador.

**A segunda pergunta.** Uma forma de fazer: começar do início da linha e procurar o próximo `;`. Se encontrar, o trecho até ele é um campo, e a busca recomeça logo depois do `;`. Quando não houver mais `;`, o que sobrou é o último campo. Para `"ana;32;Recife"`, em que os `;` estão nas posições 3 e 6:

| Busca a partir da posição | Encontra `;` em | Campo obtido | Próxima busca a partir de |
|---|---|---|---|
| 0 | 3 | `ana` (posições 0 a 2) | 4 |
| 4 | 6 | `32` (posições 4 e 5) | 7 |
| 7 | não encontra | `Recife` (da posição 7 até o fim) | fim |

Se a sua resposta foi diferente, mas chega aos mesmos três campos, ótimo: há várias implementações possíveis, e os testes vão dizer se a sua também funciona.

> [!NOTE]
> **Isso não é refatorar.** A mudança a seguir altera o **comportamento** da função, que passa a separar campos. No ciclo do TDD, isso é a etapa **verde**: fazer o teste novo passar. A refatoração vem depois, com todos os testes passando, e muda só a **estrutura** do código.

Em C++, altere o `separar.cpp`:

```cpp
#include "separar.h"

std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    std::vector<std::string> campos;
    std::string::size_type inicio = 0;

    while (true)
    {
        const auto fim = linha.find(delimitador, inicio);
        if (fim == std::string::npos)
        {
            campos.push_back(linha.substr(inicio));
            return campos;
        }

        campos.push_back(linha.substr(inicio, fim - inicio));
        inicio = fim + 1;
    }
}
```

Como o código corresponde à tabela:

| No código | Na tabela |
|---|---|
| `inicio` | A posição a partir da qual se busca |
| `linha.find(delimitador, inicio)` | Procura o próximo `;` a partir de `inicio` e devolve a posição dele |
| `std::string::npos` | O valor que o `find` devolve quando **não encontra** |
| `linha.substr(inicio, fim - inicio)` | O trecho que começa em `inicio` e tem `fim - inicio` caracteres: o campo |
| `linha.substr(inicio)` | O trecho de `inicio` até o fim da linha: o último campo |
| `inicio = fim + 1` | A próxima busca começa logo depois do `;` |

```bash
cmake --build build -j2 && ./build/separar_test
```

Agora, os dois testes passam, e os avisos do compilador desapareceram, porque agora os dois parâmetros são usados:

```
[  PASSED  ] 2 tests.
```

### 5.4 Refatorar e executar só um teste

Aplique a lista da seção 4.7. Os nomes continuam claros, não há repetição no código de produção, toda linha dele é exigida por um dos dois testes, e os avisos do compilador desapareceram. Nada a mudar.

Quando há muitos testes, é útil executar só alguns. O filtro aceita o nome completo do teste e o curinga `*`:

```bash
./build/separar_test --gtest_filter='Separar.Vari*'
```

```
Note: Google Test filter = Separar.Vari*
[ RUN      ] Separar.VariosCampos
[       OK ] Separar.VariosCampos (0 ms)
...
[  PASSED  ] 1 test.
```

---

## 6. Terceiro ciclo: campo vazio no meio

### 6.1 Vermelho?

Acrescente ao final do `separar_test.cpp`:

```cpp

TEST(Separar, CampoVazioNoMeio)
{
    const std::vector<std::string> esperado{"ana", "", "Recife"};

    EXPECT_EQ(separar("ana;;Recife", ';'), esperado);
}
```

Este teste é escrito de outra forma: em vez de conferir o tamanho e depois cada elemento, como nos testes anteriores, ele compara a lista inteira de uma vez. Isso funciona por dois motivos:

- **O `EXPECT_EQ(a, b)` verifica se `a == b`.** Ele funciona com qualquer tipo para o qual o operador `==` esteja definido: números, textos (`std::string`) e também listas (`std::vector`). Para duas listas, o `==` do C++ é verdadeiro quando elas têm o mesmo tamanho e os mesmos elementos, na mesma ordem.
- **Quando a verificação falha, o GoogleTest mostra o conteúdo dos dois lados.** Aqui, os dois lados são listas: a que a função `separar` devolveu e a lista `esperado`. Você verá como isso aparece na seção 6.2.

```bash
cmake --build build -j2 && ./build/separar_test
```

```
[ RUN      ] Separar.CampoVazioNoMeio
[       OK ] Separar.CampoVazioNoMeio (0 ms)
...
[  PASSED  ] 3 tests.
```

### 6.2 Um teste que passa de primeira

O novo teste passou sem que mudássemos nada. No TDD, isso merece atenção, porque há duas explicações possíveis:

- **O comportamento já existia.** A implementação do ciclo anterior já trata campos vazios corretamente. É o nosso caso, e o teste continua valioso: ele documenta e protege esse comportamento contra mudanças futuras.
- **O teste não verifica o que imaginamos.** Por exemplo, um teste sem nenhuma verificação passa sempre.

Para ter certeza de que é o primeiro caso, faça a prova: troque temporariamente o `""` do `esperado` por `"x"` e execute. O teste falha, mostrando as duas listas:

```
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:30: Failure
Expected equality of these values:
  separar("ana;;Recife", ';')
    Which is: { "ana", "", "Recife" }
  esperado
    Which is: { "ana", "x", "Recife" }
```

Desfaça a troca antes de continuar. **Um teste que você nunca viu falhar é um teste em que você não pode confiar.**


### 6.3 Duas dúvidas comuns

**"Então não preciso mais do `ASSERT_EQ`?"**

Para este tipo de verificação, não. Comparar a lista inteira não acessa nenhuma posição, então não há o que proteger: o `==` de duas listas confere o tamanho antes de comparar os elementos. E a mensagem de falha costuma ser até mais clara. Se o `VariosCampos` tivesse sido escrito assim, o vermelho da seção 5.1 teria mostrado a causa de imediato:

```
  separar("ana;32;Recife", ';')
    Which is: { "ana;32;Recife" }
  esperado
    Which is: { "ana", "32", "Recife" }
```

Fica claro que a linha inteira virou um único campo.

O `ASSERT_*` continua indispensável quando as linhas seguintes **dependem** de uma condição: verificar o tamanho antes de acessar uma posição, um ponteiro antes de usá-lo, um arquivo antes de lê-lo. Os primeiros testes deste módulo foram escritos elemento por elemento justamente para mostrar esse uso.

Como regra prática: quando você conhece o resultado completo esperado, compare tudo de uma vez; quando interessa só uma parte, ou o resultado é grande, verifique as partes.

Reescrever o `UmCampo` e o `VariosCampos` nesse formato seria uma **refatoração dos testes**: muda a estrutura, mas o que eles verificam continua o mesmo. Se quiser fazer, aplique a lista da seção 4.7, confirme que tudo continua passando, e faça a prova da seção 6.2 em cada teste reescrito: um teste refatorado também precisa ser visto falhando.

**"Então não preciso mais dos testes anteriores?"**

Precisa, e um experimento mostra por quê. Imagine que alguém, querendo "otimizar", acrescente no início da função uma regra que parece razoável: se a linha não tem delimitador, não há o que separar.

```cpp
std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    if (linha.find(delimitador) == std::string::npos)
        return {};

    std::vector<std::string> campos;
```

Faça essa alteração no `separar.cpp` e execute:

```bash
cmake --build build -j2 && ./build/separar_test
```

```
[ RUN      ] Separar.UmCampo
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:12: Failure
Expected equality of these values:
  campos.size()
    Which is: 0
  1u
    Which is: 1

[  FAILED  ] Separar.UmCampo (0 ms)
[ RUN      ] Separar.VariosCampos
[       OK ] Separar.VariosCampos (0 ms)
[ RUN      ] Separar.CampoVazioNoMeio
[       OK ] Separar.CampoVazioNoMeio (0 ms)
...
[  PASSED  ] 2 tests.
[  FAILED  ] 1 test, listed below:
[  FAILED  ] Separar.UmCampo
```

Só o primeiro teste percebeu o defeito. Se ele tivesse sido apagado quando os outros foram escritos, a alteração passaria sem nenhum alerta, e `separar("ana", ';')` passaria a devolver uma lista vazia.

Cada teste protege um comportamento diferente: o `UmCampo`, a linha sem delimitador; o `VariosCampos`, a separação; o `CampoVazioNoMeio`, os campos vazios. Juntos, formam uma rede de proteção que cresce a cada ciclo. É por isso que, no TDD, **todos** os testes rodam a cada mudança, e não só o mais novo: é o que garante que uma mudança não quebrou algo que já funcionava. Essa proteção se chama **teste de regressão**.

**Desfaça a alteração** antes de continuar.

---

## 7. Quarto ciclo: linha vazia

### 7.1 A decisão

Na seção 2, a linha vazia ficou com interrogação. É hora de decidir: separar `""` deve resultar em **nenhum campo** ou em **um campo vazio**?

As duas respostas são razoáveis, e projetos diferentes escolhem de formas diferentes. Vamos decidir que **uma linha vazia não tem campos**, porque, para quem processa um arquivo, uma linha em branco normalmente não é um registro.

O importante é que a decisão seja tomada de propósito, e registrada. Acrescente ao comentário do `separar.h` uma terceira linha:

```cpp
// Separa uma linha de texto em campos, usando o delimitador informado.
// Exemplo: separar("ana;32;Recife", ';') devolve {"ana", "32", "Recife"}.
// Uma linha vazia não tem campos.
std::vector<std::string> separar(const std::string& linha, char delimitador);
```

### 7.2 Vermelho

Acrescente ao final do `separar_test.cpp`:

```cpp

TEST(Separar, LinhaVazia)
{
    EXPECT_TRUE(separar("", ';').empty());
}
```

`EXPECT_TRUE(condição)` exige que a condição seja verdadeira.

```bash
cmake --build build -j2 && ./build/separar_test
```

```
[ RUN      ] Separar.LinhaVazia
/home/seu_usuario/pratica/01-fundamentos/separar_test.cpp:35: Failure
Value of: separar("", ';').empty()
  Actual: false
Expected: true

[  FAILED  ] Separar.LinhaVazia (0 ms)
```

Antes de seguir, duas perguntas para você responder sozinho, como na seção 5.1. Anote as respostas; elas serão comparadas no início da seção 7.3.

1. Por que o teste falhou? Simule a implementação da seção 5.3 passando a linha `""`: o que ela devolve?
2. O que precisa mudar no `separar.cpp` para este teste passar, sem que os outros três deixem de passar?

### 7.3 Verde

**A primeira pergunta da seção 7.2.** Simulando a implementação com a linha vazia, como na tabela da seção 5.3:

| Busca a partir da posição | Encontra `;` em | Campo obtido | Próxima busca a partir de |
|---|---|---|---|
| 0 | não encontra: a linha não tem nenhum caractere | `""` (da posição 0 até o fim, ou seja, nada) | fim |

A função devolve uma lista com **um campo vazio**, e não uma lista vazia; por isso o `empty()` resulta em `false`. É o comportamento da outra decisão possível da seção 7.1, e o teste capturou exatamente essa diferença.

**A segunda pergunta.** Pela decisão da seção 7.1, a linha vazia não tem campos, então ela nem deveria entrar no laço. Assim, a implementação mais simples é verificar isso logo no início da função e devolver uma lista vazia. Os outros três testes nunca passam uma linha vazia, então essa verificação não os afeta.

Acrescente ao início da função, no `separar.cpp`:

```cpp
std::vector<std::string> separar(const std::string& linha, char delimitador)
{
    if (linha.empty())
        return {};

    std::vector<std::string> campos;
```

```bash
cmake --build build -j2 && ./build/separar_test
```

```
[  PASSED  ] 4 tests.
```

### 7.4 Conferir pelo `ctest`

```bash
ctest --test-dir build
```

```
1/4 Test #1: Separar.UmCampo ..................   Passed    0.00 sec
2/4 Test #2: Separar.VariosCampos .............   Passed    0.00 sec
3/4 Test #3: Separar.CampoVazioNoMeio .........   Passed    0.00 sec
4/4 Test #4: Separar.LinhaVazia ...............   Passed    0.00 sec

100% tests passed, 0 tests failed out of 4
```

Os seus arquivos agora devem estar iguais aos da pasta [`gabarito/01-fundamentos`](gabarito/01-fundamentos) do repositório. Compare.

---

## 8. Desafio

Falta uma linha da tabela da seção 2: o **delimitador no fim**, como em `"ana;32;"`.

1. **Antes de escrever o teste**, preveja: o que a implementação atual devolve para essa entrada?
2. Decida qual deveria ser o comportamento e escreva o teste `Separar.DelimitadorNoFim`.
3. Execute. O resultado confirmou a sua previsão?

Tente resolver antes de abrir a resposta.

<details>
<summary>Resposta</summary>

A implementação atual devolve **três campos**: `"ana"`, `"32"` e um campo vazio no final. Depois do último `;` não há nada, e esse "nada" vira um campo.

Esse comportamento é coerente com o do campo vazio no meio, e é o que fazem as planilhas ao exportar uma linha cuja última coluna está vazia. Se você decidir mantê-lo, o teste é:

```cpp
TEST(Separar, DelimitadorNoFim)
{
    const std::vector<std::string> esperado{"ana", "32", ""};

    EXPECT_EQ(separar("ana;32;", ';'), esperado);
}
```

Ele passa de primeira. Faça a prova da seção 6.2 para confirmar que o teste é capaz de falhar.

Se você decidiu que o campo vazio final deve ser descartado, o teste falha, e você tem um novo ciclo vermelho e verde pela frente. As duas decisões são válidas, desde que tomadas de propósito e registradas no comentário da função.

</details>

---

## 9. Revisão crítica

Quatro testes passando não significam que a função está correta para qualquer entrada. Situações que **nenhum teste verifica ainda**:

- **Delimitador no início**, como em `";ana"`.
- **Linha que é só o delimitador**, como `";"`. Quantos campos?
- **Outro delimitador**, como `,` ou tabulação. Todos os testes usam `;`.
- **Espaços em volta dos campos**, como em `"ana ; 32"`. Devem ser preservados ou removidos?
- **Campos entre aspas** que contêm o delimitador, como no formato CSV completo. Fora do escopo desta função, mas vale registrar a limitação.

Cada item é uma pergunta de requisito antes de ser uma pergunta de código. O Módulo 2 trata justamente disso: como encontrar os casos que faltam, de forma sistemática.

---

## Na documentação oficial

A documentação do GoogleTest fica em <https://google.github.io/googletest/>, em inglês. Ela é extensa, e boa parte é material de consulta. Este curso não substitui a documentação: o objetivo é você entender o suficiente para usá-la por conta própria.

**O que este módulo praticou:**

| Assunto | Página | Seção |
|---|---|---|
| Conceitos: teste, suíte, asserção | [GoogleTest Primer](https://google.github.io/googletest/primer.html) | *Basic Concepts* e *Simple Tests* |
| `EXPECT_*` × `ASSERT_*` | [GoogleTest Primer](https://google.github.io/googletest/primer.html) | *Assertions* |
| `EXPECT_TRUE`, `EXPECT_EQ` e as demais comparações | [Assertions](https://google.github.io/googletest/reference/assertions.html) | *Boolean Conditions* e *Binary Comparison* |
| A macro `TEST` | [Testing Reference](https://google.github.io/googletest/reference/testing.html) | *TEST* |
| O projeto com CMake e `FetchContent` | [Quickstart: CMake](https://google.github.io/googletest/quickstart-cmake.html) | *Set up a project* |
| `--gtest_filter` | [Advanced Topics](https://google.github.io/googletest/advanced.html) | *Selecting Tests* |

**Para ir além, quando quiser:**

| Assunto | Página | Seção |
|---|---|---|
| Comparar textos de C e números com casas decimais | [Assertions](https://google.github.io/googletest/reference/assertions.html) | *String Comparison* e *Floating-Point Comparison* |
| Falhar ou passar de forma explícita | [Assertions](https://google.github.io/googletest/reference/assertions.html) | *Explicit Success and Failure* |
| Outras opções de execução: repetir, embaralhar, relatórios | [Advanced Topics](https://google.github.io/googletest/advanced.html) | *Running Test Programs: Advanced Options* |

O [mapa da documentação](mapa-documentacao.md) mostra o que cada módulo do curso cobre.

---

## Referência rápida

### As verificações deste módulo

| Verificação | Exige que |
|---|---|
| `EXPECT_EQ(a, b)` | `a == b` |
| `EXPECT_TRUE(c)` | `c` seja verdadeira |

Outras verificações básicas, que seguem a mesma lógica e aparecem nos próximos módulos:

| Verificação | Exige que |
|---|---|
| `EXPECT_NE(a, b)` | `a != b` |
| `EXPECT_LT(a, b)`, `EXPECT_LE(a, b)` | `a < b`, `a <= b` |
| `EXPECT_GT(a, b)`, `EXPECT_GE(a, b)` | `a > b`, `a >= b` |
| `EXPECT_FALSE(c)` | `c` seja falsa |
| `EXPECT_DOUBLE_EQ(a, b)` | dois `double` sejam iguais, tolerando o erro de arredondamento |
| `EXPECT_NEAR(a, b, erro)` | a diferença entre `a` e `b` seja no máximo `erro` |

Todas têm a versão `ASSERT_*` correspondente.

### `EXPECT` ou `ASSERT`

| Use | Quando |
|---|---|
| `ASSERT_*` | O restante do teste depende da condição (tamanho antes de acessar elementos, ponteiro antes de usar) |
| `EXPECT_*` | Nos demais casos, para ver todas as falhas de um teste de uma só vez |

### Comandos

| Comando | Para quê |
|---|---|
| `cmake -S . -B build` | Configurar. Na primeira vez, quando o `CMakeLists.txt` mudar ou quando um arquivo citado nele for criado |
| `cmake --build build -j2 && ./build/separar_test` | Compilar e, se der certo, executar os testes |
| `./build/separar_test --gtest_filter='Suite.Teste*'` | Executar só os testes cujo nome combina com o filtro |
| `ctest --test-dir build` | Executar todos os testes registrados, cada um em um processo, com um resumo |
| `ctest --test-dir build --output-on-failure` | O mesmo, mostrando a saída dos testes que falharam |

---

## Critérios de domínio

Antes de seguir para o Módulo 2, confira se você consegue:

- [ ] Explicar a diferença entre um erro de compilação e um teste que falha.
- [ ] Ler uma mensagem de falha e dizer qual verificação falhou, em que linha e com que valores.
- [ ] Explicar, com o experimento da seção 4.4, por que o `ASSERT_EQ` protege o restante do teste.
- [ ] Descrever o ciclo vermelho, verde e refatorar, e por que o vermelho vem primeiro.
- [ ] Justificar por que um teste que passa de primeira merece uma prova.
- [ ] Explicar, com o experimento da seção 6.3, por que os testes anteriores continuam necessários.
- [ ] Explicar por que a decisão sobre a linha vazia foi registrada no `separar.h`.

---

**Próximo módulo:** Módulo 2, Fundamentos de qualidade de testes: limites, entradas inválidas e oráculos.

*Work hard. Have fun. Make history.*
