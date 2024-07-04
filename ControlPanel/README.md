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

