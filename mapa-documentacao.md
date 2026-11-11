# Mapa da documentação oficial

A documentação do GoogleTest, em <https://google.github.io/googletest/>, tem 18 páginas em três grupos: primeiros passos (*Get Started*), guias (*Guides*) e referências (*References*). Os guias *Advanced Topics* e *Mocking Cookbook* sozinhos têm mais de cem seções.

Este curso não cobre tudo, de propósito. Ele cobre o essencial com profundidade e mostra onde está o restante, para que você consiga usar a documentação por conta própria. Este mapa mostra o que entra em cada módulo, o que fica só mencionado e o que fica de fora.

## O que o curso cobre

| Parte da documentação | Módulo | Profundidade |
|---|---|---|
| [Primer](https://google.github.io/googletest/primer.html): conceitos, asserções, testes simples, execução | 1 | Completa |
| [Assertions](https://google.github.io/googletest/reference/assertions.html): booleanas e comparações | 1 | Completa |
| [Assertions](https://google.github.io/googletest/reference/assertions.html): textos e ponto flutuante | 2 | Completa |
| [Primer](https://google.github.io/googletest/primer.html): *fixtures* (`TEST_F`) e escrever o próprio `main()` | 3 | Completa |
| [Advanced Topics](https://google.github.io/googletest/advanced.html): testes parametrizados por valor e por tipo, recursos compartilhados entre testes | 3 | Completa |
| [Advanced Topics](https://google.github.io/googletest/advanced.html): exceções, *death tests*, pular testes, `SCOPED_TRACE`, asserções com predicado, preparação global | 4 | Completa |
| [Advanced Topics](https://google.github.io/googletest/advanced.html): opções de execução (filtro, repetição, ordem aleatória, relatórios XML e JSON) | 4 | Completa |
| [Advanced Topics](https://google.github.io/googletest/advanced.html): ensinar o GoogleTest a imprimir os seus tipos | 4 | Breve |
| [Mocking for Dummies](https://google.github.io/googletest/gmock_for_dummies.html) | 5 | Completa |
| [Mocking Cookbook](https://google.github.io/googletest/gmock_cook_book.html): criar mocks, *Nice* e *Strict*, matchers, ordem das chamadas, ações | 5 | Seleção do essencial |
| [Matchers](https://google.github.io/googletest/reference/matchers.html) e [Actions](https://google.github.io/googletest/reference/actions.html) | 5 | Como consulta, praticando os mais usados |

Os Módulos 6 a 8 (cobertura, testes de mutação e geração automática de testes) usam outras ferramentas e não dependem de partes adicionais desta documentação.

## Só mencionado, com o caminho na documentação

| Tópico | Motivo |
|---|---|
| *Event listeners* e registro de testes por programa | Úteis para quem constrói ferramentas sobre o GoogleTest |
| Distribuir testes entre várias máquinas, integração com *sanitizers* | Assunto de infraestrutura |
| Testar código privado (`FRIEND_TEST`) | Discutido como prática a evitar, mais do que como técnica |
| Escrever matchers e ações próprios; mock de funções livres e de métodos não virtuais | Avançados para quem está começando |
| Macros antigas (`MOCK_METHODn`) e recursos marcados como obsoletos | Só para reconhecer em código legado |

## Fora do escopo

| Tópico | Motivo |
|---|---|
| [Quickstart: Bazel](https://google.github.io/googletest/quickstart-bazel.html) e [pkg-config](https://google.github.io/googletest/pkgconfig.html) | O curso usa CMake |
| Asserções `HRESULT` | Exclusivas do Windows |
