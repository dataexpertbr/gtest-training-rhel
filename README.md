# gtest-training-rhel

Curso prático de GoogleTest e GoogleMock em C++, do zero, em um ambiente Red Hat Enterprise Linux 8.

O objetivo não é decorar macros. É aprender a raciocinar sobre testes: o que testar, como escrever, como avaliar se o teste é bom e, no final, como automatizar a geração deles.

> *Work hard. Have fun. Make history.*

## Para quem é

Para quem trabalha com C++ e precisa escrever testes unitários, mas ainda não teve a chance de aprender os fundamentos. Não é preciso experiência prévia com GoogleTest.

## Princípios do curso

- **Nenhum capítulo sem prática.** Todo conceito vem com laboratório e desafio.
- **Shift Left.** Buscamos a qualidade o mais cedo possível: requisitos, ambiguidades e riscos antes do código.
- **TDD.** Sempre que houver comportamento novo, seguimos o ciclo RED, GREEN, REFACTOR.
- **Experimentação.** Provocamos falhas de propósito para entender por que um teste detecta, ou não, um defeito.

Shift Left e TDD não são capítulos isolados: orientam todos os módulos.

## Ambiente

| Item | Escolha |
|---|---|
| Sistema | RHEL 8.10 em máquina virtual (Hyper-V) |
| Compilador | GCC |
| Build | CMake |
| Testes | GoogleTest e GoogleMock |

## Módulos

| # | Módulo | Nível | Situação |
|---|---|---|---|
| 0 | [Ambiente de estudo com RHEL 8 no Hyper-V](00-ambiente-rhel8-hyperv.md) | Preparação | Disponível |
| 1 | Ambiente e fundamentos: GCC, CMake, GoogleTest, `TEST`, `EXPECT_*` e `ASSERT_*` | Iniciante | Em preparação |
| 2 | Fundamentos de qualidade de testes: limites, entradas inválidas e oráculos | Iniciante | Planejado |
| 3 | Organização e reutilização: fixtures, `TEST_F`, `TEST_P` e testes tipados | Intermediário | Planejado |
| 4 | Recursos avançados do GoogleTest: exceções, death tests, filtros e diagnóstico | Intermediário/avançado | Planejado |
| 5 | GoogleMock: `MOCK_METHOD`, `EXPECT_CALL`, `ON_CALL`, matchers e ações | Intermediário/avançado | Planejado |
| 6 | Cobertura e avaliação: linhas, branches, condições e gcov | Avançado | Planejado |
| 7 | Testes de mutação: operadores, mutantes e escore de mutação | Avançado | Planejado |
| 8 | Projeto integrador: geração automática de testes | Aplicação prática | Planejado |

## Formato de cada aula

1. **Problema:** um requisito pequeno e realista.
2. **Raciocínio:** o que precisa ser verificado e quais dúvidas resolver antes de implementar.
3. **Teste primeiro:** escrita dos testes e dos resultados esperados.
4. **Execução:** compilar, executar e observar as falhas.
5. **Implementação:** o código necessário para os testes passarem.
6. **Refatoração:** melhorar o código sem perder o comportamento verificado.
7. **Desafio:** uma variação para resolver antes de consultar a solução.
8. **Revisão crítica:** limitações, casos ausentes e defeitos que os testes ainda não detectam.

## Pré-requisitos

- Windows 10/11 Pro ou Enterprise, com Hyper-V
- 8 GB de RAM (16 GB recomendado) e 60 GB livres em disco
- Conta gratuita no [Red Hat Developer Program](https://developers.redhat.com)

Os detalhes estão no [Módulo 0](00-ambiente-rhel8-hyperv.md).

## Contribuições

Encontrou um erro ou um passo confuso? Abra uma *issue* ou envie um *pull request*.

## Licença

- Código: [MIT](LICENSE)
- Material didático (textos e imagens): [CC BY 4.0](LICENSE-docs)

Red Hat e Red Hat Enterprise Linux são marcas registradas da Red Hat, Inc.
Windows e Hyper-V são marcas registradas da Microsoft Corporation.
GoogleTest é um projeto de código aberto do Google LLC.
Este material é independente e não é afiliado nem endossado por essas empresas.
