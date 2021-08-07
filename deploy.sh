actual_path=$(pwd)
 
cd
 
#INICIO DE COMMONS
 
if test -d /home/utnso/so-commons-library; then
    cd /home/utnso/so-commons-library/
    sudo make uninstall
    cd ..
    sudo rm -r ./so-commons-library
fi
 
cd

mkdir polus2

cd
 
git clone https://github.com/sisoputnfrba/so-commons-library.git
cd so-commons-library/
sudo make install
 
#FIN COMMONS
 
#INICIO NIVEL GUI
 
sudo apt-get install libncurses5-dev
 
if test -d /home/utnso/so-nivel-gui-library; then
    cd /home/utnso/so-nivel-gui-library/
    sudo make uninstall
    cd ..
    sudo rm -r ./so-nivel-gui-library
fi
 
cd
 
git clone https://github.com/sisoputnfrba/so-nivel-gui-library.git
cd so-nivel-gui-library/
sudo make install

cd

git clone https://github.com/sisoputnfrba/a-mongos-pruebas

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/shared-library-so/SharedUtils/Debug/
 
#FIN NIVEL GUI
 
#COMPILACION
 
cd $actual_path
 
make
 
#SETEO DE IPs
 
echo "Ingresar IP Discordiador: "
read ip_d
echo "Ingresar IP Mi-RAM HQ: "
read ip_r
echo "Ingresar IP i-MongoStore: "
read ip_m
 
cd $actual_path/pruebas
 
echo -e "IP_D=$ip_d
IP_R=$ip_r
IP_M=$ip_m" > env.sh
 
chmod +x env.sh
 
#FIN DE SETTEO
