# tp-2021-1c-bastardosSinGloria
## Pasos para deployar el TP :

### Clonar la shared
**git clone https://github.com/FeroMateo/shared-library-so.git**

### Clonar el repo
**git clone https://github.com/sisoputnfrba/tp-2021-1c-bastardosSinGloria.git**

### Entrar en la carpeta del repo 
**cd tp-2021-1c-bastardosSinGloria**
### Ejecutar el deploy
**sh deploy.sh**

### Agregar variable de entorno (en cada instancia de la terminal)
**export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/shared-library-so/SharedUtils/Debug/**
