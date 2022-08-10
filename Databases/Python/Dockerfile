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


from python:3.9-alpine

RUN apk update && apk add postgresql-dev gcc python3-dev musl-dev

run pip install flask

run pip install PyJWT

run pip install APScheduler

run pip install psycopg2-binary

copy . /app

volume ["/app"]

workdir /app

run mkdir -p logs

EXPOSE 5000

CMD ["python", "projeto.py"]
