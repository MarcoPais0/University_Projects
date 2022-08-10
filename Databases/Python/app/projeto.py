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

import logging
import time
import jwt
import psycopg2
from flask import Flask, jsonify, request
import datetime
from functools import wraps

app = Flask(__name__)

global secret_key
secret_key = 'projetobd2020-2021'


@app.route('/projeto/')
def inicio():
    return "Leilões Online - Projeto Base de Dados 2020/2021"


def token_required(f):
    @wraps(f)
    def decorated(*args, **kwargs):
        token = None
        if 'access-token' in request.headers:
            token = request.headers['access-token']

        if not token:
            return jsonify('Utilizador não autenticado!')

        try:
            data = jwt.decode(token, secret_key, algorithms='HS256')
            user = {'userid': data['userid'], 'username': data['username'], 'admin': data['admin']}
        except:
            return jsonify('Token expirado!')

        return f(user, *args, **kwargs)

    return decorated


@app.route('/projeto/leiloes/end', methods=['PUT'], strict_slashes=True)
def check_auctions():
    logger.info("###          | PUT | Verifica se os leilões acabaram              ###")
    conn = db_connection()
    cur = conn.cursor()

    # Verifica se existe algum leilão expirado e que ainda não tenha sido dados como finalizado
    statement = """
                  SELECT winnerid, produto_barcode, users_userid, data_final 
                  FROM leiloes 
                  WHERE age(now(), data_final) >= interval '0' day and cancelado = false and finalizado = false 
                  FOR UPDATE"""

    cur.execute(statement)

    rows = cur.fetchall()

    if rows != []:
        for row in rows:
            statement = """UPDATE leiloes SET finalizado = true 
                           WHERE produto_barcode = %s""" % row[1]
            cur.execute(statement)
            
            # Verifica se existe vencedor para informar 
            if row[0] != None:
                statement = """LOCK TABLE notificacoes in exclusive mode;
                                INSERT INTO notificacoes (users_userid, mensagem, leiloes_produto_barcode, data)
                                VALUES (%s, %s, %s, %s)"""
                values = (row[0], "Venceu o leilao %s" %
                          row[1], row[1], row[3])
                cur.execute(statement, values)

                statement = """
                                SELECT leiloes_vencidos 
                                FROM users 
                                WHERE userid = %s 
                                FOR UPDATE;
                                UPDATE users 
                                SET leiloes_vencidos = leiloes_vencidos + 1 
                                WHERE userid = %s""" % (row[0], row[0])

                cur.execute(statement)

            statement = """
                            INSERT INTO notificacoes (users_userid, mensagem, leiloes_produto_barcode, data)
                            VALUES (%s, %s, %s, %s)"""
            values = (row[2], "O seu leilão %s acabou e o vencedor foi %s" % (
                row[1], row[0]), row[1], row[3])
            cur.execute(statement, values)
            cur.execute("commit")
            content = "Todos os leiloes expirados foram terminados com sucesso!"
    else:
        content = "Não existem leilões por terminar!"

    conn.close()
    return jsonify(content)


##
#   Regista Utilizadores
#   http://localhost:8080/projeto/user/register/
##


@app.route("/projeto/users/register/", methods=['POST'], strict_slashes=True)
def user_register():
    logger.info("###            | POST | Registo de Utilizadores              ###")
    payload = request.get_json()
    conn = db_connection()
    cur = conn.cursor()

    if "admin" in payload:
        statement = """LOCK TABLE users in exclusive mode; 
                    INSERT INTO users (username, email, password, admin) 
                    VALUES (%s, %s, crypt(%s, 'bf'), %s) """

        values = (payload["username"],
                payload["email"], payload["password"], payload["admin"])
    else:
        statement = """LOCK TABLE users in exclusive mode; 
                   INSERT INTO users (username, email, password) 
                   VALUES (%s, %s, crypt(%s, 'bf')) """

        values = (payload["username"],
                payload["email"], payload["password"])
    
    try:
        cur.execute(statement, values)
        cur.execute("commit")
        if "admin" in payload and payload["admin"] == True:
            result = 'Novo administrador criado!' + " " + payload["password"]
        else:
            result = 'Novo utilizador criado!' + " " + payload["password"]
    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(error)
        result = 'Falha na criacao do utilizador/administrador!'
    finally:
        if conn is not None:
            conn.close()

    return jsonify(result)


##
#   Autentica o Utilizador
#   http://localhost:8080/projeto/users/login/
##
@app.route("/projeto/users/login", methods=['PUT'])
def user_authentication():
    logger.info("###             | PUT | Autenticação de Utilizadores            ###")
    auth = request.authorization
    conn = db_connection()
    cur = conn.cursor()

    statement = """
                SELECT userid,admin,banido from users
                WHERE username = %s
                AND password = crypt(%s, password)"""

    values = (auth.username, auth.password)

    try:
        cur.execute(statement, values)
        rows = cur.fetchall()
        row = rows[0]
        if(not row[2]):
            token = jwt.encode({'userid': row[0], 'username': auth.username, 'admin': row[1], 'exp': datetime.datetime.utcnow(
            ) + datetime.timedelta(minutes=60)}, secret_key, algorithm='HS256')

            if cur.rowcount != 0:
                result = 'Utilizador  autenticado! Token: ' + token
            else:
                result = 'Insira um username e password validos!'
        else:
            result = 'Auteticação Falhou. Utilizador Banido!'
        cur.execute("commit")
    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(error)
        result = 'Autenticação falhou!'
    finally:
        if conn is not None:
            conn.close()

    return jsonify(result)


##
#   Adiciona Leilões
#   http://localhost:8080/projeto/leiloes/
##
@app.route("/projeto/leiloes/", methods=['POST'])
@token_required
def add_auction(user):
    logger.info("###             | POST | /projeto/leiloes/              ###")
    payload = request.get_json()

    conn = db_connection()
    cur = conn.cursor()

    cur.execute("""SELECT admin FROM users WHERE userid = %s""" % user["userid"])
    rows = cur.fetchall()
    
    # Verifica se é admin ou não. Caso seja não pode criar leilões
    if rows[0][0] != True:

        # O leilão tem de ter no mínimo uma duração de 1 hora
        cur.execute("""SELECT age(now(), timestamp '%s') < - interval '1' hour""" %
                    payload["data_final"])
        rows = cur.fetchall()

        if rows[0][0] == True:

            statement = """LOCK TABLE leiloes in exclusive mode;
                        INSERT INTO leiloes (users_userid, produto_barcode, precomin, titulo, descricao, data_inicial, data_final) 
                        VALUES (%s, %s, %s, %s, %s, %s, %s)"""

            values = (user["userid"], payload["produto_barcode"], payload["precomin"],
                    payload["titulo"], payload["descricao"], "now()", payload["data_final"])

            try:
                cur.execute(statement, values)
                
                statement = """
                    SELECT leiloes_criados 
                    FROM users WHERE userid = %s 
                    FOR UPDATE;
                    UPDATE users SET leiloes_criados = leiloes_criados + 1 
                    WHERE userid = %s""" % (user["userid"], user["userid"])
                cur.execute(statement)

                cur.execute("commit")
                result = 'Leilão Criado!'
            except (Exception, psycopg2.DatabaseError) as error:
                logger.error(error)
                result = 'Falha na criação do leilão!'
            finally:
                if conn is not None:
                    conn.close()
        else:
            result = 'Data Final do leilao invalida!'
    else:
        result = 'Os administradores não podem criar leilões.'
    return jsonify(result)


##
#   Obtém uma lista de leilões
#   http://localhost:8080/projeto/leiloes/
##
@app.route("/projeto/leiloes/", methods=['GET'])
@token_required
def get_all_auctions(user):
    logger.info("###             | GET | projeto/leiloes/              ###")

    conn = db_connection()
    cur = conn.cursor()

    cur.execute("SELECT produto_barcode, descricao FROM leiloes")
    rows = cur.fetchall()

    if rows != []:
        payload = []
        for row in rows:
            content = {'ID Leilao': int(row[0]), 'Descricao': row[1]}
            payload.append(content)  # appending to the payload to be returned
    else:
        payload = "Nao existem leiloes!"

    conn.close()
    return jsonify(payload)


##
#   Pesquisa Leilões por palavra-chave (produto_barcode <produto_barcode> or descricao <descricao>
#   http://localhost:8080/projeto/leiloes/<keyword>
##
@app.route("/projeto/leiloes/<keyword>", methods=['GET'])
@token_required
def get_auction(user, keyword):
    logger.info("###             | GET | projeto/leiloes/<keyword>              ###")

    conn = db_connection()
    cur = conn.cursor()

    if keyword.isdigit():
        statement = """SELECT produto_barcode, descricao 
                       FROM leiloes
                       WHERE produto_barcode = %s""" % keyword
        cur.execute(statement)

    else:
        statement = """SELECT produto_barcode, descricao 
                       FROM leiloes
                       WHERE descricao = \'%s\'""" % keyword
        cur.execute(statement)

    rows = cur.fetchall()
    if rows != []:
        row = rows[0]
        content = {'ID Leilao': int(row[0]), 'Descricao': row[1]}
    else:
        content = "Produto/descricao inexistente!"
    conn.close()
    return jsonify(content)


##
#   Consultar detalhes de um leilão
#   http://localhost:8080/projeto/leiloes/produto_barcode=<produto_barcode>
##
@app.route("/projeto/leiloes/produto_barcode=<produto_barcode>", methods=['GET'])
@token_required
def get_auction_details(user, produto_barcode):
    logger.info("###              | GET | /leilao/produto_barcode=<produto_barcode>              ###")

    conn = db_connection()
    cur = conn.cursor()

    statement = """SELECT * 
                   FROM leiloes 
                   WHERE produto_barcode = %s""" % produto_barcode

    cur.execute(statement)

    rows = cur.fetchall()
    if rows != []:
        row = rows[0]
        payload = ["---- Detalhes do leilão selecionado  ----"]
        content = {'Dono': int(row[9]), 'ID Leilao': int(row[0]), 'Titulo': row[1], 'Descricao': row[2],
                       'Preço Atual': row[3], 'Data Inicial': row[4], 'Data Final': row[5], 'Vencedor': row[6]}
        payload.append(content)
        
        payload.append('---- Mural do Leilão ----')
        statement = """
                        SELECT mensagem, data, users_userid
                        FROM mensagens_mural 
                        WHERE leiloes_produto_barcode = %s
                        ORDER BY data DESC""" % produto_barcode
        
        cur.execute(statement)
        rows = cur.fetchall()
        for row in rows:
            content = {'Data': row[1], 'Mensagens': row[0], 'Criador da mensagem' : row[2]}
            payload.append(content)  
    else:
        payload = "Leilao inexistente!"

    conn.close()
    return jsonify(payload)


##
#   Listar todos os leilões em que o utilizador tenha atividade
#   http://localhost:8080/projeto/users/leiloes/
##
@app.route("/projeto/users/leiloes/", methods=['GET'])
@token_required
def get_user_auctions(user):
    logger.info("###             | GET | /projeto/users/leiloes/              ###")

    conn = db_connection()
    cur = conn.cursor()

    statement = """SELECT * 
                   FROM leiloes, licitacoes 
                   WHERE leiloes.produto_barcode = licitacoes.leiloes_produto_barcode AND licitacoes.users_userid = %s""" % user["userid"]
    cur.execute(statement)
    rows = cur.fetchall()
    statement = """SELECT * 
                   FROM leiloes 
                   WHERE users_userid = %s""" % (user['userid'])
    cur.execute(statement)

    rows.append(cur.fetchall())
    if rows != [] and rows[0] != []:
        payload = []
        for row in rows[0]:
            content = {'Dono': int(row[9]), 'ID Leilao': int(row[0]), 'Titulo': row[1], 'Descricao': row[2],
                       'Preço Atual': row[3], 'Data Inicial': row[4], 'Data Final': row[5], 'Vencedor': row[6]}
            payload.append(content)  # appending to the payload to be returned
    else:
        payload = "Sem participacoes!"

    conn.close()
    return jsonify(payload)


##
#   Listar todas as notificações do utilizador
#   http://localhost:8080/projeto/users/notificacoes
##
@app.route("/projeto/users/notificacoes", methods=['GET'])
@token_required
def list_notifications(user):
    logger.info("###             | GET | /projeto/users/notificacoes             ###")

    conn = db_connection()
    cur = conn.cursor()

    statement = """SELECT * 
                   FROM notificacoes 
                   WHERE users_userid = %s
                   ORDER BY data DESC""" % user["userid"]
    cur.execute(statement)

    rows = cur.fetchall()
    if rows != []:
        payload = []
        for row in rows:
            content = {'Data': row[1], 'ID Leilao': row[3], 'Mensagem': row[2]}
            payload.append(content)  # appending to the payload to be returned
    else:
        payload = "Sem Notificacoes!"

    conn.close()
    return jsonify(payload)


##
#   Efetuar uma Licitação
#   http://localhost:8080/projeto/leilao/<leilao>
##
@app.route("/projeto/licitacoes/", methods=['POST'])
@token_required
def add_bids(user):
    logger.info("###             | POST | /projeto/licitacoes/           ###")

    conn = db_connection()
    cur = conn.cursor()
    payload = request.get_json()

    statement = "SELECT licitacoes(%s, %s, %s)"
    values = (payload["valor"], user["userid"],
              payload["leiloes_produto_barcode"])

    try:
        cur.execute(statement, values)
        cur.execute("commit")
        result = 'Licitacao Criada!'
    except (Exception, psycopg2.DatabaseError) as error:
        result = 'Licitacao falhou!'

    conn.close()
    return jsonify(result)


##
#   Editar propriedades de um leilão
#   http://localhost:8080/projeto/leiloes/leilao=<leilao>
##
@app.route("/projeto/leiloes/leilao=<leilao>", methods=['PUT'])
@token_required
def edit_auction(user, leilao):
    logger.info("###             | PUT | projeto/leiloes/leilao=<leilao>            ###")
    content = request.get_json()

    conn = db_connection()
    cur = conn.cursor()

    if content == None or "descricao" not in content or "titulo" not in content:
        return 'Descrição e título são necessários para atualizar!'

    statement = """SELECT descricao, titulo 
                   FROM leiloes 
                   WHERE produto_barcode = %s AND users_userid = %s
                   FOR UPDATE""" % (leilao, user["userid"])
    cur.execute(statement)
    rows = cur.fetchall()

    if rows != []:
        row = rows[0]

        statement = """
                    UPDATE leiloes 
                    SET descricao = %s, titulo = %s
                    WHERE produto_barcode = %s"""

        values = (content["descricao"], content["titulo"], leilao)
        cur.execute(statement, values)

        statement = """ LOCK TABLE alteracoes_leiloes in exclusive mode;
                        INSERT INTO alteracoes_leiloes (descricao, titulo, leiloes_produto_barcode)
                        VALUES (%s, %s, %s)"""
        values = (row[0], row[1], leilao)
        cur.execute(statement, values)
        result = 'Leilao atualizado!'
        cur.execute("commit")
    else: 
        result = 'Impossivel atualizar!'

    conn.close()
    return jsonify(result)


##
#   Escrever mensagens no mural de um leilão
#   http://localhost:8080/leiloes/mural/
##
@app.route("/projeto/leiloes/mural/", methods=['POST'], strict_slashes=True)
@token_required
def add_mural_message(user):
    logger.info("###             | POST | /projeto/leiloes/mural/              ###")
    payload = request.get_json()
    conn = db_connection()
    cur = conn.cursor()

    statement = """ LOCK TABLE mensagens_mural in exclusive mode; 
                    INSERT INTO mensagens_mural (data, mensagem, users_userid, leiloes_produto_barcode) 
                    VALUES (%s, %s, %s, %s)"""

    values = ("now()", payload["mensagem"], user['userid'],
              payload["leiloes_produto_barcode"])

    try:
        cur.execute(statement, values)
        cur.execute("commit")
        result = 'Nova mensagem adicionada ao mural!'
    except (Exception, psycopg2.DatabaseError) as error:
        logger.error(error)
        result = 'Falha na criação da mensagem no mural!'
    finally:
        if conn is not None:
            conn.close()

    return jsonify(result)


##
#   Cancelar um leilão
#   http://localhost:8080/projeto/leiloes/cancelar_leilao=<produto_barcode>
##
@app.route("/projeto/leiloes/cancelar_leilao=<produto_barcode>", methods=['PUT'], strict_slashes=True)
@token_required
def cancel_auction(user, produto_barcode):
    if(user['admin']):
        logger.info("###             | PUT | /projeto/leiloes/cancelar_leilao=<produto_barcode>              ###")
        conn = db_connection()
        cur = conn.cursor()

        statement = """ SELECT *
                        FROM leiloes
                        WHERE produto_barcode = %s
                        FOR UPDATE""" % produto_barcode
        try:
            cur.execute(statement)
            if cur.fetchall() != []:
                statement = """ UPDATE leiloes 
                            SET cancelado = true
                            WHERE produto_barcode = %s""" % produto_barcode
                cur.execute(statement)
                cur.execute("commit")
                result = 'Leilao cancelado!'
            else:
                raise Exception
        except (Exception, psycopg2.DatabaseError) as error:
            logger.error(error)
            result = 'Nao foi possivel cancelar o leilao!'
        finally:
            if conn is not None:
                conn.close()
        return jsonify(result)
    else:
        return jsonify('Sao necessarios privilegios de administrador!')


##
#   Banir um utilizador
#   http://localhost:8080/projeto/admin/ban_user=<userid>
##
@app.route("/projeto/admin/ban_user=<userid>", methods=['PUT'], strict_slashes=True)
@token_required
def ban_user(user, userid):
    if(user['admin']):
        logger.info("###             | PUT | /projeto/admin/ban_user=<userid>              ###")
        conn = db_connection()
        cur = conn.cursor()

        statement = """ SELECT *
                        FROM users
                        WHERE userid = %s
                        FOR UPDATE""" % userid
        cur.execute(statement)
        statement = """ UPDATE users 
                        SET banido = true
                        WHERE userid = %s""" % userid
        try:
            cur.execute(statement)
            cur.execute("commit")
            result = 'Utilizador Banido!'
        except (Exception, psycopg2.DatabaseError) as error:
            logger.error(error)
            result = 'Nao foi possivel banir o utilizador!'
        finally:
            if conn is not None:
                conn.close()
        return jsonify(result)
    else:
        return jsonify('Sao necessarios privilegios de administrador!')


##
#   Obter Estatísticas
#   http://localhost:8080/projeto/estatisticas/
##
@app.route("/projeto/estatisticas/", methods=['GET'], strict_slashes=True)
@token_required
def statistics(user):
    if(user['admin']):
        logger.info("###             | GET | /projeto/estatisticas/            ###")
        payload = request.get_json()
        conn = db_connection()
        cur = conn.cursor()

        statement = """ SELECT *
                        FROM users 
                        ORDER BY leiloes_criados DESC
                        LIMIT 10;
                                 """
        payload1 = ['Top 10 Utilizadores com mais leiloes criados:'] 
        cur.execute(statement)
        rows = cur.fetchall()
        for row in rows:
            content = {'Utilizador': row[1], 'Leilões Criados': int(row[6])}
            payload1.append(content)

        statement = """ SELECT *
                        FROM users
                        ORDER BY leiloes_vencidos DESC
                        LIMIT 10;
                                   """ 
        payload2 = ['Top 10 Utilizadores com mais leiloes vencidos:'] 
        cur.execute(statement)
        rows = cur.fetchall()
        for row in rows:
            content = {'Utilizador': row[1], 'Leilões Vencidos': int(row[7])}
            payload2.append(content)

        statement = """ SELECT count(*)
                        FROM leiloes
                        WHERE age(now(), data_inicial) < interval '1' day * -10;
                                                                                """ 
        cur.execute(statement)
        content = cur.fetchall()
        payload3 = ['Número total de leilões nos últimos 10 dias:', 'Valor: %d '  % content[0][0]] 
        conn.close()
        return jsonify(payload1 + payload2 + payload3)
    else:
        payload = 'Sao necessarios privilegios de administrador!'
        return jsonify(payload)
    


##########################################################
#                    DATABASE ACCESS                     #
##########################################################
def db_connection():
    db = psycopg2.connect(user="aulaspl",
                          password="aulaspl",
                          host="db",
                          port="5432",
                          database="dbfichas")
    return db


##########################################################
#                         MAIN                           #
##########################################################
if __name__ == "__main__":

    # Set up the logging
    logging.basicConfig(filename="logs/log_file.log")
    logger = logging.getLogger('logger')
    logger.setLevel(logging.DEBUG)
    ch = logging.StreamHandler()
    ch.setLevel(logging.DEBUG)

    # create formatter
    formatter = logging.Formatter('%(asctime)s [%(levelname)s]:  %(message)s',
                                  '%H:%M:%S')

    ch.setFormatter(formatter)
    logger.addHandler(ch)

    time.sleep(1)

    logger.info("\n---------------------------------------------------------------\n" +
                "Projeto BD - API online: http://localhost:8080/projeto/\n\n")
    
    app.run(host="0.0.0.0", debug=True, threaded=True)

