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
 
#FIN NIVEL GUI
 
#COMPILACION
 
cd $actual_path
 
make
 
#FIN COMPILACION
 
#DESCARGANDO PRUEBAS RELEASE 1.0.0
sudo apt install unzip
cd pruebas
rm -r a-mongos-pruebas
unzip a-mongos-pruebas-1.0.0.zip
mv a-mongos-pruebas-1.0.0 a-mongos-pruebas
 
#FIN DESCARGANDO PRUEBAS RELEASE 1.0.0
 
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