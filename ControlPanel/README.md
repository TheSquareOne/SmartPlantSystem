# Control panel  

This is simple website used to control plant nodes. Website is build using python framework Flask and run using gunicorn in a docker.  

#### Folder structure:  
ControlPanel\  
├─ logs\
├─ static\  
├── *.js  
├── *.css  
├─ templates\  
├── *.html    
├─ app.py  
├─ Dockerfile  
├─ requirements.txt  
└─ .dockerignore

### docker-compose  
```yaml
version: "3"
services:
  web:
    build:
      context: /share/Containers/smartplant_controlpanel
      dockerfile: ./Dockerfile
      no_cache: true
    container_name: smartplant-controlpanel
    hostname: smartplant-controlpanel
    environment:
      - PUID=1001
      - PGID=1000
      - TZ=Europe/Helsinki
    volumes:
      - /share/Containers/smartplant_controlpanel/logs:/app/logs
    ports:
      - 5000:5000
    restart: unless-stopped
```

### Dockerfile
```Dockerfile
FROM python:3.8-slim-buster

WORKDIR /app

COPY . .

RUN pip install --no-cache-dir --upgrade -r requirements.txt

RUN mkdir -p logs && touch logs/access.log logs/error.log

CMD [ "gunicorn", "--workers", "2", "--bind", "0.0.0.0:5000", "--access-logfile", "/app/logs/access.log", "--error-logfile", "/app/logs/error.log", "app:app" ]
```

 - Pull python image.
 - Set working directory to `/app`.
 - Copy everything from the Dockerfile folder to the image.
 - Upgrade and install all the libraries from the `requirements.txt` using pip.
 - Create directory `logs` and create files `access.log` and `error.log`.
 - Start gunicorn which runs the website. Number of workers and port can be changed if needed.  


### Use virtual environment for development and testing.  

Create Python virtual environment.  
```
py -3 -m venv .venv
```  

Allow scripts on powershell and activate venv.  
```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
.venv\Scripts\activate
```  

Install requirements.  
```
pip install -r requirements.txt
```  

Run flask app,
```
flask run
```  

or run specific app.
```
flask --app app.py run
```  

