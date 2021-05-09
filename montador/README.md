# Montador

## Comando para compilar:
make

## Comando pra rodar:
./montador meuarquivo.asm

## Comando para apagar pastas antigas
make clean


Aluna: Nayara Silva   170153304
SB 2020/02  


--------------------------------------------------------------------------
tipos de erro:

declarações e rótulos ausentes(Quando voce tem SPACE Ou CONST sem rotulo antes
)
declarações e rótulos repetidos;
diretivas e instruções inválidas(se não é rotulo, é diretiva ou instrução, se não é nenhum, é uma instrução invalida)
instruções e diretivas com a quantidade de operando errada(se é instrução ou se é diretiva e nao tem o numero certo de operando)
instruções com o tipo de operando inválido;
tokens inválidos (Tokens maior do que 50 ou com caractere que nao seja letra, digito ou _)