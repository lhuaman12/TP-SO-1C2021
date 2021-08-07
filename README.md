# tp-2021-1c-bastardosSinGloria
Pasos para deployar el TP :

## clonar la shared
**-git clone https://github.com/FeroMateo/shared-library-so.git**

## clonar el repo
**-git clone https://github.com/sisoputnfrba/tp-2021-1c-bastardosSinGloria.git**

## entrar en la carpeta del repo 
**-cd tp-2021-1c-bastardosSinGloria**
## ejecutar el deploy
**-sh deploy.sh**

## agregar variable de entorno (en cada instancia de la terminal)
**-export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/shared-library-so/SharedUtils/Debug/**
