/* 
##
# =============================================
# ============== Data Base ====================
# ========== Project 2020/2021 ================
# =============================================
# Authors:
#   Marco da Cruz Pais - <uc2019218981@student.uc.pt>
#   Simão Carvalho Monteiro - <uc2019215412@student.uc.pt>
#   Tiago Miguel Gomes Marques Oliveira - <uc2019219068@student.uc.pt>
#   University of Coimbra - Degree in Computer Engineering
##
*/


/* 
DROP TABLE users CASCADE;
DROP TABLE leiloes CASCADE;
DROP TABLE licitacoes CASCADE;
DROP TABLE mensagens_mural CASCADE;
DROP TABLE notificacoes CASCADE;
DROP TABLE alteracoes_leiloes CASCADE;
DROP TABLE users_leiloes CASCADE;
DROP TABLE users_leiloes_1 CASCADE;
*/

/* USADO PARA A ENCRIPTAÇÃO DAS PASSWORDS*/
CREATE EXTENSION pgcrypto;

CREATE TABLE users (
	userid	 SERIAL UNIQUE NOT NULL,
	username VARCHAR(512) NOT NULL,
	email	 VARCHAR(512) NOT NULL,
	password VARCHAR(512) NOT NULL,
	admin	 BOOL DEFAULT false,
	banido	 BOOL DEFAULT false,
	leiloes_criados INTEGER DEFAULT 0,
	leiloes_vencidos INTEGER DEFAULT 0,
	PRIMARY KEY(userid)
);

CREATE TABLE leiloes (
	produto_barcode BIGINT UNIQUE NOT NULL,
	titulo		 VARCHAR(512) NOT NULL,
	descricao	 VARCHAR(512),
	precomin	 FLOAT(8) NOT NULL,
	data_inicial	 TIMESTAMP NOT NULL,
	data_final	 TIMESTAMP NOT NULL,
	winnerid	 BIGINT UNIQUE,
	cancelado	 BOOL DEFAULT false,
	finalizado	 BOOL DEFAULT false,
	users_userid	 INTEGER NOT NULL,
	PRIMARY KEY(produto_barcode)
);

CREATE TABLE licitacoes (
	licitacaoid		 SERIAL UNIQUE NOT NULL,
	valor			 BIGINT NOT NULL,
	data			 TIMESTAMP NOT NULL,
	users_userid		 INTEGER NOT NULL,
	leiloes_produto_barcode BIGINT NOT NULL,
	invalido 			BOOL DEFAULT false,
	PRIMARY KEY(licitacaoid)
);

CREATE TABLE mensagens_mural (
	mensagemid		 SERIAL UNIQUE NOT NULL,
	data			 TIMESTAMP NOT NULL,
	mensagem		 VARCHAR(512),
	leiloes_produto_barcode BIGINT NOT NULL,
	users_userid		 INTEGER NOT NULL,
	PRIMARY KEY(mensagemid)
);

CREATE TABLE notificacoes (
	mensagemid		 SERIAL UNIQUE NOT NULL,
	data			 TIMESTAMP,
	mensagem		 VARCHAR(512),
	users_userid		 INTEGER NOT NULL,
	leiloes_produto_barcode BIGINT NOT NULL,
	PRIMARY KEY(mensagemid)
);

CREATE TABLE alteracoes_leiloes (
	descricao		 VARCHAR(512),
	titulo			 VARCHAR(512),
	leiloes_produto_barcode BIGINT NOT NULL
);

CREATE TABLE users_leiloes (
	users_userid		 INTEGER,
	leiloes_produto_barcode BIGINT NOT NULL,
	PRIMARY KEY(users_userid)
);

CREATE TABLE users_leiloes_1 (
	users_userid		 INTEGER NOT NULL,
	leiloes_produto_barcode BIGINT,
	PRIMARY KEY(leiloes_produto_barcode)
);

ALTER TABLE leiloes ADD CONSTRAINT leiloes_fk1 FOREIGN KEY (users_userid) REFERENCES users(userid);
ALTER TABLE licitacoes ADD CONSTRAINT licitacoes_fk1 FOREIGN KEY (users_userid) REFERENCES users(userid);
ALTER TABLE licitacoes ADD CONSTRAINT licitacoes_fk2 FOREIGN KEY (leiloes_produto_barcode) REFERENCES leiloes(produto_barcode);
ALTER TABLE mensagens_mural ADD CONSTRAINT mensagens_mural_fk1 FOREIGN KEY (leiloes_produto_barcode) REFERENCES leiloes(produto_barcode);
ALTER TABLE mensagens_mural ADD CONSTRAINT mensagens_mural_fk2 FOREIGN KEY (users_userid) REFERENCES users(userid);
ALTER TABLE notificacoes ADD CONSTRAINT notificacoes_fk1 FOREIGN KEY (users_userid) REFERENCES users(userid);
ALTER TABLE notificacoes ADD CONSTRAINT notificacoes_fk2 FOREIGN KEY (leiloes_produto_barcode) REFERENCES leiloes(produto_barcode);
ALTER TABLE alteracoes_leiloes ADD CONSTRAINT alteracoes_leiloes_fk1 FOREIGN KEY (leiloes_produto_barcode) REFERENCES leiloes(produto_barcode);
ALTER TABLE users_leiloes ADD CONSTRAINT users_leiloes_fk1 FOREIGN KEY (users_userid) REFERENCES users(userid);
ALTER TABLE users_leiloes ADD CONSTRAINT users_leiloes_fk2 FOREIGN KEY (leiloes_produto_barcode) REFERENCES leiloes(produto_barcode);

/* EXEMPLOS DE UTILIZADORES */
INSERT INTO users (username, email, password) VALUES ('olaa', 'olaa', crypt('olaa123', 'bf'));
INSERT INTO users (username, email, password) VALUES ('adeus', 'adeus', crypt('adeus123', 'bf'));
INSERT INTO users (username, email, password) VALUES ('marco', 'marco@gmail.com', crypt('marco123', 'bf'));
INSERT INTO users (username, email, password) VALUES ('simao', 'simao@gmail.com', crypt('simao123', 'bf'));
INSERT INTO users (username, email, password) VALUES ('tiago', 'tiago@gmail.com', crypt('tiago123', 'bf'));

/* EXEMPLO DE ADMINISTRADOR*/
INSERT INTO users (username, email, password, admin) VALUES ('badbunny', 'badbunny@com.gmail.com', crypt('badbunny123', 'bf'), true);


/* TRIGGER PARA AS LICITAÇÕES */
CREATE FUNCTION bid_exceeded() RETURNS TRIGGER AS $bid_exceeded$
    BEGIN
        IF NEW.winnerid <> OLD.winnerid THEN
            LOCK TABLE notificacoes in exclusive mode;
            INSERT INTO notificacoes(data, mensagem, users_userid, leiloes_produto_barcode)
            VALUES(now(), 'A sua licitação foi ultrapassada!', OLD.winnerid, OLD.produto_barcode);
            INSERT INTO notificacoes(data, mensagem, users_userid, leiloes_produto_barcode)
            VALUES(now(), 'Nova licitação máxima!', OLD.users_userid, OLD.produto_barcode);
        END IF;
		IF NEW.users_userid IS NULL THEN 
				RAISE EXCEPTION 'Could not cancel auction';
		END IF;
		IF NEW.cancelado <> OLD.cancelado THEN
			LOCK TABLE mensagens_mural in exclusive mode;
            INSERT INTO mensagens_mural(data, mensagem, users_userid, leiloes_produto_barcode)
            VALUES(now(), 'O leilão foi cancelado!', NEW.users_userid, OLD.produto_barcode);
		END IF;
        RETURN NEW;
    END;
$bid_exceeded$ LANGUAGE plpgsql;
CREATE TRIGGER bid_exceeded 
    AFTER UPDATE ON leiloes
    FOR EACH ROW
    EXECUTE PROCEDURE bid_exceeded();


/* TRIGGER PARA AS NOTIFICAÇÕES */
CREATE FUNCTION notification_deliver() RETURNS TRIGGER AS $notification_deliver$
    DECLARE 
	row_notificacoes record;
	cur_notificacoes cursor FOR 
		WITH mensagem AS(
			SELECT leiloes_produto_barcode, users_userid
			FROM mensagens_mural
			ORDER BY mensagens_mural.mensagemid DESC
			LIMIT 1
		)
		SELECT licitacoes.leiloes_produto_barcode, licitacoes.users_userid
		FROM licitacoes, mensagem
		WHERE licitacoes.leiloes_produto_barcode = mensagem.leiloes_produto_barcode AND licitacoes.users_userid <> mensagem.users_userid;

	cur_notificacoes_owner cursor FOR 
		WITH mensagem AS(
			SELECT leiloes_produto_barcode, users_userid
			FROM mensagens_mural
			ORDER BY mensagens_mural.mensagemid DESC
			LIMIT 1
		)
		SELECT leiloes.produto_barcode, leiloes.users_userid
		FROM  leiloes, mensagem
		WHERE leiloes.produto_barcode = mensagem.leiloes_produto_barcode AND leiloes.users_userid <> mensagem.users_userid;
    BEGIN
        LOCK TABLE notificacoes in exclusive mode;
        OPEN cur_notificacoes;
        LOOP
            FETCH cur_notificacoes INTO row_notificacoes;
            EXIT WHEN NOT FOUND;
            INSERT INTO notificacoes(data, mensagem, users_userid, leiloes_produto_barcode)
            VALUES (now(), 'Foi escrita uma mensagem no mural!', row_notificacoes.users_userid,  row_notificacoes.leiloes_produto_barcode);
        END LOOP;
		CLOSE cur_notificacoes;
		OPEN cur_notificacoes_owner;
		FETCH cur_notificacoes_owner INTO row_notificacoes;
		IF row_notificacoes.users_userid IS NOT NULL THEN 
			INSERT INTO notificacoes(data, mensagem, users_userid, leiloes_produto_barcode)
			VALUES (now(), 'Foi escrita uma mensagem no seu mural!', row_notificacoes.users_userid,  row_notificacoes.produto_barcode);
		END IF;
        RETURN NEW;  
	CLOSE cur_notificacoes_owner;
    END;

$notification_deliver$ LANGUAGE plpgsql;
CREATE TRIGGER notification_deliver
    AFTER INSERT ON mensagens_mural
    FOR EACH ROW
    EXECUTE PROCEDURE notification_deliver();


/*TRIGGER PARA O CANCELAMENTO DE UM UTILIZADOR*/
CREATE FUNCTION user_banned() RETURNS TRIGGER AS $user_banned$
	DECLARE 
	row record;
	cur_licitacoes_user cursor FOR
		SELECT *
		FROM licitacoes
		WHERE users_userid = NEW.userid
		FOR UPDATE;
	cur_leiloes cursor FOR
		SELECT *
		FROM leiloes
		WHERE winnerid = NEW.userid
		FOR UPDATE;
    BEGIN
        IF NEW.banido <> OLD.banido THEN
			OPEN cur_licitacoes_user;
			LOOP
				FETCH cur_licitacoes_user INTO row;
				EXIT WHEN NOT FOUND;
				UPDATE licitacoes SET invalido = true WHERE CURRENT of cur_licitacoes_user;
			END LOOP;
			OPEN cur_leiloes;
			LOOP
				FETCH cur_leiloes INTO row;
				EXIT WHEN NOT FOUND;
				WITH licitacao as(
					SELECT *
					FROM licitacoes
					WHERE row.produto_barcode = leiloes_produto_barcode and invalido = false and users_userid <> row.users_userid
					ORDER BY valor DESC
					LIMIT 1
				)
				UPDATE leiloes SET winnerid = licitacao.users_userid, precomin = licitacao.valor 
				FROM licitacao
				WHERE produto_barcode = licitacao.leiloes_produto_barcode;
				LOCK TABLE mensagens_mural IN exclusive mode;
				INSERT INTO mensagens_mural(data, mensagem, users_userid, leiloes_produto_barcode)
				VALUES(now(), 'O leilao sofreu alteracoes. Pedimos desculpa!', row.users_userid, row.leiloes_produto_barcode);
			END LOOP;
			CLOSE cur_leiloes;
        END IF;
        RETURN NEW;
    END;
$user_banned$ LANGUAGE plpgsql; 
CREATE TRIGGER user_banned
    AFTER UPDATE ON users
    FOR EACH ROW
  	EXECUTE PROCEDURE
 	user_banned();


/* FUNÇÂO PARA AS LICITAÇÔES */
CREATE OR REPLACE FUNCTION licitacoes(u_valor BIGINT, u_userid INTEGER, u_barcode BIGINT)
   RETURNS VOID LANGUAGE PLPGSQL AS $$
DECLARE 
	row_licitacoes  record;
	cur_licitacoes cursor(u_valor BIGINT, u_userid INTEGER, u_barcode BIGINT) FOR
		WITH ban as(
			SELECT banido, admin
			from users
			where userid = u_userid
		)
		SELECT *
		FROM leiloes, ban
		WHERE produto_barcode = u_barcode AND users_userid <> u_userid AND precomin < u_valor AND age(now(), data_final) < interval '0' day AND cancelado = false AND finalizado = false and ban.banido = false AND ban.admin = false
        FOR UPDATE;
BEGIN
    OPEN  cur_licitacoes(u_valor, u_userid, u_barcode);
    FETCH cur_licitacoes INTO row_licitacoes;
	IF row_licitacoes.users_userid IS NULL THEN 
		RAISE EXCEPTION 'Could not perform bidding';
	END IF;
    UPDATE leiloes SET precomin = u_valor, winnerid = u_userid
    WHERE CURRENT of cur_licitacoes;
    CLOSE cur_licitacoes;
    LOCK TABLE licitacoes in exclusive mode;
    INSERT INTO licitacoes (valor, data, users_userid, leiloes_produto_barcode) 
    VALUES (u_valor, now(), u_userid, u_barcode);
END; $$