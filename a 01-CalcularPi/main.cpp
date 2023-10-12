

#include <iostream>
#include <iomanip>
using namespace std;

double Leibniz(int &contador)
{
   
    bool sumar = false;
    double denominador = 3;
     double cuarto = 1;/*cuarto *4 = pi */
    double anterior = 0;
    do
    {
        anterior = cuarto;
        if(sumar==true)
        {
            cuarto = cuarto+(1/denominador);
            sumar=false;
        }
        else
        {
             cuarto = cuarto-(1/denominador);
             sumar = true;
        }
        denominador = denominador +  2;
        contador+=1;
        /*cout<<setprecision(64)<<fixed<<contador<<": "<<cuarto*4<<endl;*/
    }while(cuarto - anterior > 0.000000326795  || cuarto - anterior < -0.000000326795);
    return cuarto;
   
    
}

int main(){
    double pi;
    int contador;
    pi = Leibniz(contador)*4;
    cout<<setprecision(8)<<fixed<<"Primer valor pi con los primero 6 decimales correctos: "<<pi<<endl;
    cout<<"encontrado dsp de "<<contador<<" ejecuciones del algoritmo de Leibniz";
    return 0;
}