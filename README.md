# Servidor Web em C com Sockets

Servidor HTTP simples desenvolvido em C usando Winsock2. O projeto serve arquivos estáticos da pasta `www` e foi criado para estudo de sockets e do protocolo HTTP.

## Requisitos

- Windows
- GCC para Windows (MinGW-w64), disponível no `PATH`

## Compilar

Execute os comandos na pasta raiz do projeto:

```powershell
gcc -Wall -Wextra .\src\servidor.c .\src\funcoes.c -o .\servidor.exe -lws2_32
```

## Executar

```powershell
.\servidor.exe
```

Com o servidor em execução, acesse <http://localhost:8080/>. Encerre-o com `Ctrl+C`.

O caminho `www` é relativo à pasta de onde o servidor é iniciado. Por isso, execute o programa a partir da raiz do projeto.

## Estrutura

```text
.
├── src/
│   ├── funcoes.c
│   ├── funcoes.h
│   └── servidor.c
├── www/
│   ├── css/
│   ├── img/
│   ├── js/
│   ├── erro404.html
│   ├── erro501.html
│   ├── formulario.html
│   └── index.html
├── .gitignore
└── README.md
```

## Funcionalidades

- Aceita conexões TCP IPv4 na porta `8080`.
- Processa requisições HTTP `GET` e retorna páginas, imagens, CSS e JavaScript.
- Retorna páginas de erro para arquivos não encontrados (`404`) e métodos não implementados (`501`).
- Trata cada conexão em uma thread.

O formulário HTML demonstra uma requisição `POST`, mas o servidor ainda não implementa esse método. Este é um projeto didático, não destinado a uso em produção.

## Publicar no GitHub

Crie um repositório vazio no GitHub e, na pasta raiz do projeto, execute:

```powershell
git init
git add .
git commit -m "Adiciona servidor web em C com Winsock"
git branch -M main
git remote add origin https://github.com/SEU_USUARIO/NOME_DO_REPOSITORIO.git
git push -u origin main
```

Substitua `SEU_USUARIO` e `NOME_DO_REPOSITORIO` pelos dados do repositório criado. O `.gitignore` evita enviar executáveis e arquivos intermediários de compilação.
