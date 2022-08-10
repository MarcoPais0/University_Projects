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


FROM library/postgres


ENV POSTGRES_USER aulaspl
ENV POSTGRES_PASSWORD aulaspl
ENV POSTGRES_DB dbfichas
ENV CREATE EXTENSION pgcrypto;

COPY projeto_data.sql /docker-entrypoint-initdb.d/

EXPOSE 5432
