# scoreDEI

O scoreDEI é uma plataforma para consulta de resultados desportivos em tempo real.

Existem três tipos de utilizadores que são:

- Admin
- User
- Not User

Os **Not User** podem, então efetuar operações como:

- Acompanhar jogo
- Consultar estatísticas

Os **User** podem fazer tudo que o Not User faz mais efetuar operações como:

- Registar evento num jogo

Por fim os **Admin** podem efetuar todas as operações anterios mais as seguintes:

- Registar utilizadores
- Criar/Gerir Equipas
- Criar/Gerir Jogadores
- Criar/Gerir Jogos de Futebols
- Obter dados da API (api-sports.io)

## Executar o scoreDei:

- O scoreDei vem com todos os ficheiros necessários, na pasta code, para ser executado.
- Criamos, também, um container dentro da pasta code para correr a aplicação num container docker.

**Container:**

- Deverá abrir o docker e inicializar o container, através de um IDE que suporte Remote Containers ou mesmo na própria aplicação docker.
- Dentro do container irá se encontrar na pasta workspace em que poderá executar o comando:

```
  ./mvnw spring-boot:run
```

- Se tiver problemas na primeira execução, corra o comando, se pedir **password** irá ser "main":

```
  sudo chown -R main:main ../workspace
```

- E só depois:

```
  ./mvnw spring-boot:run
```

**Ficheiro WAR (Executar dentro do container):**

- Existe, também, um ficheiro WAR no seguinte diretório:

```
  scoreDEI\Code\target\scoreDEI-0.0.1-SNAPSHOT.war
```

- Este ficheiro WAR poderá ser executado, dentro do diretório referido acima, através do seguinte comando:

```
  java -jar scoreDEI-0.0.1-SNAPSHOT.war
```

## Usar o scoreDei:

O scoreDei é muito simples de usar, tal como outro website.

- Inicialmente estarão dois utilizadores na base de dados, um admin e um user:

```
  #Admin
  Email: admin@admin
  Password: admin

  # User
  Email: user@user
  Password: user
```

- Através das credenciais dadas acima será possível começar a utilizar a aplicação da maneira que entender, sendo que cada template está conectado a outro de maneira uniforme o que torna desnecessário usar os URL's.
