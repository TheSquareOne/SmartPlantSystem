FROM python:3.8-slim-buster

WORKDIR /app

COPY . .

RUN pip install --no-cache-dir --upgrade -r requirements.txt

RUN mkdir -p logs && touch logs/access.log logs/error.log

CMD [ "gunicorn", "--workers", "2", "--bind", "0.0.0.0:5000", "--access-logfile", "/app/logs/access.log", "--error-logfile", "/app/logs/error.log", "app:app" ]