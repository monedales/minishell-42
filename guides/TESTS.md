# 🧪 Minishell — Testes da Avaliação


## 1. Compilação

```bash
make -n          # deve mostrar -Wall -Wextra -Werror
make re          # deve compilar sem erros
make             # não deve re-linkar (rodar duas vezes seguidas)
```

✅ Esperado: compila limpo, sem warnings, sem re-link

---

## 2. Comando Simples & Variável Global

```bash
/bin/ls
/usr/bin/wc /etc/hosts
/bin/echo hello
```

```bash
# Linha vazia — não deve travar nem crashar
[Enter]
[Enter]
[Enter]
```

```bash
# Só espaços e tabs — não deve travar
   [Enter]
	[Enter]
```

**Pergunta esperada do avaliador:**
> "Quantas variáveis globais você usa? Por quê?"

Resposta: uma — `g_signal` (volatile sig_atomic_t). Armazena apenas o número
do sinal recebido. Necessária porque handlers de sinal não podem acessar
estruturas complexas com segurança.

---

## 3. Argumentos

```bash
/bin/ls -la
/bin/ls -la /tmp
/usr/bin/wc -l /etc/hosts
/bin/echo hello world foo bar
/usr/bin/cat /etc/hosts
```

Repete com comandos e argumentos diferentes. Não deve travar nem crashar.

---

## 4. echo

```bash
echo hello
echo hello world
echo -n hello
echo -n hello world **verificar**
echo -n
echo
echo -n -n hello
echo hello   world      spaces
```

✅ `-n` suprime o newline
✅ Múltiplos `-n` aceitos
✅ `echo` sem args imprime linha vazia

---

## 5. exit

```bash
exit
```
Relança o minishell e testa:
```bash
exit 0
```
```bash
exit 42
```
```bash
exit -1
```
```bash
exit 256
```
```bash
exit abc
```
```bash
exit 1 2
```

✅ `exit` sem arg → sai com último exit status
✅ `exit 42` → retorna 42
✅ `exit -1` → 255 (módulo 256)
✅ `exit 256` → 0 (módulo 256)
✅ `exit abc` → "numeric argument required", sai com 2
✅ `exit 1 2` → "too many arguments", **não sai**

---

## 6. Return value ($?)

```bash
/bin/ls
echo $?
```
```bash
/bin/ls /tmp/arquivo_que_nao_existe
echo $?
```
```bash
/bin/notexist
echo $?
```
```bash
/bin/ls /tmp/arquivo_que_nao_existe
expr $? + $?
```
```bash
echo $?
echo $?
```

✅ Comando ok → 0
✅ Arquivo não encontrado → 1 ou 2
✅ Comando não encontrado → 127
✅ `expr $? + $?` → deve usar o valor de $? duas vezes na expansão
✅ Segundo `echo $?` → 0 (o echo teve sucesso)

---

---

## 7. Sinais

**No prompt vazio:**
```
ctrl-C   → nova linha + novo prompt
ctrl-\   → não faz nada
ctrl-D   → sai (relancar depois!)
```

**No prompt depois de escrever algo (sem executar):**
```
echo hel[ctrl-C]   → nova linha + novo prompt limpo
                     apertar Enter depois não executa nada
echo hel[ctrl-D]   → não faz nada
echo hel[ctrl-\]   → não faz nada
```

**Com processo bloqueado rodando (`cat` ou `grep`):**
```bash
cat
```
```
[ctrl-C]   → mata o cat, volta pro prompt com ^C
[ctrl-\]   → mata o cat com "Quit (core dumped)"
[ctrl-D]   → fecha o stdin do cat, cat termina normalmente
```

```bash
grep "algo"
```
Repete os três sinais acima.

---

## 8. Aspas Duplas

```bash
echo "hello world"
echo "hello    world    spaces"
echo "cat lol.c | cat > lol.c"
echo "pipe | inside | quotes"
echo "redir > inside > quotes"
echo "it's a test"
echo "$USER inside double quotes"
```

✅ Tudo dentro de `"..."` é uma palavra só
✅ `$` ainda expande dentro de aspas duplas
✅ `|` e `>` dentro de aspas são literais

---

## 9. Aspas Simples

```bash
echo 'hello world'
echo ''
echo '$USER'
echo '$HOME'
echo '$?'
echo 'pipe | inside | quotes'
echo 'redir > inside'
echo 'no $EXPAND here'
```

✅ Nada expande dentro de `'...'`
✅ `echo '$USER'` deve imprimir `$USER` literal
✅ `echo ''` imprime linha vazia

---

## 10. env

```bash
env
```

✅ Mostra todas as variáveis de ambiente atuais no formato KEY=VALUE

---

## 11. export

```bash
export TEST=hello
env | grep TEST
export TEST=world
env | grep TEST
export NEWVAR=42
env | grep NEWVAR
export
```

✅ Cria e atualiza variáveis
✅ `export` sem arg lista tudo no formato `declare -x KEY="VALUE"`

---

## 12. unset

```bash
export TOREMOVE=yes
env | grep TOREMOVE
unset TOREMOVE
env | grep TOREMOVE
unset VARQUENAOEXISTE
echo $?
```

✅ Remove variável do ambiente
✅ `unset` em variável inexistente → silencioso, sem erro

---

## 13. cd

```bash
cd /tmp
/bin/ls
cd ..
/bin/ls
cd .
pwd
cd /tmp/pasta_que_nao_existe
echo $?
cd
pwd
```

✅ Muda de diretório corretamente
✅ `.` e `..` funcionam
✅ Diretório inválido → mensagem de erro
✅ `cd` sem arg → vai para $HOME

---

---

## 14. pwd

```bash
pwd
cd /tmp
pwd
cd /usr/local
pwd
cd /
pwd
```

✅ Imprime o diretório atual correto em cada contexto

---

## 15. Relative Path

```bash
cd /usr/local/bin
ls ../../lib
ls ../../../etc
ls ../../../../tmp
pwd
cd ../../../../tmp
pwd
```

✅ Caminhos com múltiplos `..` funcionam
✅ Executa binários com caminho relativo

---

## 16. Environment PATH

```bash
ls
wc --version
echo hello
```

```bash
unset PATH
ls
echo $?
wc --version
```

```bash
export PATH=/bin:/usr/bin
ls
wc --version
```

```bash
export PATH=/usr/bin:/bin
ls
```

✅ Sem PATH → comandos não encontrados (127)
✅ PATH com múltiplos diretórios → verifica da esquerda pra direita
✅ Restaurar PATH e verificar que volta a funcionar

---

## 17. Redirecionamentos

```bash
echo hello > /tmp/ms_out.txt
cat /tmp/ms_out.txt
```

```bash
echo world >> /tmp/ms_out.txt
cat /tmp/ms_out.txt
```

```bash
cat < /tmp/ms_out.txt
```

```bash
wc -l < /tmp/ms_out.txt
```

```bash
echo line1 > /tmp/ms_a.txt
echo line2 > /tmp/ms_b.txt
cat < /tmp/ms_a.txt > /tmp/ms_c.txt
cat /tmp/ms_c.txt
```

```bash
cat << EOF
linha 1
linha 2
linha 3
EOF
```

```bash
cat << STOP > /tmp/ms_heredoc.txt
conteudo do heredoc
STOP
cat /tmp/ms_heredoc.txt
```

✅ `>` cria/sobrescreve
✅ `>>` anexa
✅ `<` redireciona stdin
✅ Heredoc `<<` funciona e não precisa aparecer no histórico
✅ Combinação de pipe + redir funciona

---

## 18. Pipes

```bash
ls | cat
echo hello | cat
ls | wc -l
cat /etc/hosts | grep localhost
```

```bash
ls | cat | wc -l
cat /etc/hosts | grep local | wc -l
```

```bash
/bin/ls /tmp/arquivo_que_nao_existe | grep bla
echo $?
```

```bash
cat | cat | ls
```

✅ Pipes simples e múltiplos funcionam
✅ Comando com erro no início do pipeline → resto do pipe continua
✅ `cat | cat | ls` → ls imprime imediatamente, os cats ficam aguardando

---

## 19. Go Crazy & History

```bash
echo hello
[ctrl-C]
[Enter]
```
> Buffer deve estar limpo — Enter não executa nada

```bash
echo primeiro
echo segundo
[↑]
[↑]
[↓]
[Enter]
```
> Navega pelo histórico com setas, roda o comando selecionado

```bash
dsbksdgbksdghsd
```
> Não crasha, imprime "command not found"

```bash
cat | cat | ls
```
> ls executa imediatamente, cats aguardam stdin

```bash
echo $(seq 1 100)
/bin/ls -la -R /usr/lib
```
> Comandos longos com muitos argumentos não crasham

---

## 20. Variáveis de Ambiente

```bash
echo $HOME
echo $USER
echo $PATH
echo $?
```

```bash
echo "$USER"
```

```bash
export USER=claudinho
echo "$USER"
echo $USER
```

```bash
echo $VARQUENAOEXISTE
echo "$VARQUENAOEXISTE"
```

✅ `$` interpreta como variável de ambiente
✅ `"$USER"` expande dentro de aspas duplas
✅ Variável não definida → string vazia, sem erro
✅ `echo "$USER"` imprime o valor de USER

---

## ⚠️ Lembrar na avaliação

- Após cada bloco de testes com `exit`, **relançar o minishell**
- Testar leaks com `leaks --atExit -- ./minishell` ao final
- Ter bash aberto ao lado para comparar comportamento quando necessário
- A variável global `g_signal` deve ser a **única** variável global
