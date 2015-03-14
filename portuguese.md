# Instruções de instalação #

## Cliente no celular ##
### Dependências ###
O cliente depende do Python para S60 (testado em 1.3.22, 1.4.0 e 1.4.1) e podem ser encontrados no [sourceforge](http://sourceforge.net/project/showfiles.php?group_id=154155). Apenas atente que há versões diferentes do python dependendo da versão do seu celular (se for 2 ou 3 edição).

Para instalar o python, copie os 2 arquivos .sis (python e o script shell) para seu telefone e selecione-os usando o gerenciador de arquivos do próprio telefone. Existem outras alternativas para realizar a instalação do python, maiores detalhes [aqui](http://wiki.opensource.nokia.com/projects/Installing_PyS60).

### Instalando o Amora ###
Crie uma pasta no cartão de memória do telefone (E:\Python) e copie o conteúdo do arquivo comprimido do cliente do [amora](http://amora.googlecode.com/files/amora_client-0.8.tar.bz2) na pasta.

Você deve usar o python script shell (haverá um ícone na pasta 'minhas aplicações') para iniciar o cliente do amora (selecione o arquivo 'presenter.py'). No python script shell, selecione 'Options->Run script'.

O cliente do amora deve ser executado se você realizou os passos corretamente até aqui.

## Servidor de PC ##
### Dependências de Software ###
Para compilar no lado do servidor, você deve ter instalados:

  * gcc compilador
  * Xlibs
  * XTest libs
  * BlueZ libs
  * Imlib2 libs

E também a extensão XTest habilitada no seu servidor X (padrão no Ubuntu). Então, no Ubuntu, execute em um terminal:

$sudo apt-get install gcc libxext-dev libbluetooth2-dev libimlib2-dev libxtst-dev


### Compilando ###
Depois de instalar as dependências, extraia os arquivos do servidor com:

$ tar -zxvf amora\_server-0.x.tar.gz

onde _x_ é a versão **atual** do servidor.

Mude para o diretório extraído e execute os comandos:

$mkdir build

$cd build

$../configure

$make

Ele deve gerar um executável chamado 'amora', você deve começar a aplicação (e lembre-se de se certificar se o seu dongle bluetooth está conectado):

$./amora


## Versão do repositório ##

Se você baixou o código do repositório, você deve primeiro gerar os scripts de compilação. Isto exige como dependências os pacotes do autotools (automake, etc).

Havendo estas dependências pré-instaladas, rode em um terminal:

$ autoreconf -i --force

e então basta seguir as instruções anteriores para compilar a versão tarball do servidor.

# Sobre o projeto #

O foco do projeto é em construir um programa para celular com interface intuitiva e cujo servidor no lado do PC não requira nenhuma configuração complexa.

O que funciona: movimento de mouse, clique, arrastar com o mouse, scroll wheel, atalhos de teclado (ENTER, ESC, ESPAÇO, F5, 'F', setas direita/esquerda). Abaixo você pode ver o mapeamento dos comandos nas teclas do celular:

![http://amora.googlecode.com/svn/wiki/keymap_wide.jpg](http://amora.googlecode.com/svn/wiki/keymap_wide.jpg)

Você pode solicitar imagens das janelas ativas e também usar um modo onde cada clique do mouse automaticamente gera a imagem da janela ativa (útil quando controlando slides de uma apresentação).

O projeto já exibe as características:

  * cliente do celular pode ser iniciado indendentemente do servidor no PC

  * é possível fechar a conecção e depois abri-la novamente

  * o servidor pode detectar se o cliente por algum motivo morreu

  * o cliente pode detectar se o servidor caiu


Já é bastante estável e eu já o utilizo para controlar meus slides e quando assisto filmes: é possível começar uma apresentação, pará-la, mover o cursor para um diretório, fazer duplo clique em um arquivo de vídeo e controlar a maior parte das ações no mplayer).


## Grupo de usuários ##

Se você tem questões, sugestões, relatos de problemas ou apenas deseja compartilhar suas experiências com o Amora, então visite: http://groups.google.com/group/amora-user