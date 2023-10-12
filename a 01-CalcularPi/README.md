Materia:Algoritmos y Estructura de Datos
Curso:k1102
Profesor: Eduardo Zúñiga
Alumno: Karol Liam 

Descripcion del tp: El codigo es bastante simple, en el main defino un variable tipo double "pi" y una variable de tipo entero "contador".Para asignarle el valor a pi
llamo a la función "Leibniz" de tipo double y le paso como parametro por referencia el contador. Esta funcion va a aplicar la serie de Leibniz (1-(1/3)+(1/5)-(1/7)...),
hasta llegar al primer valor de pi que tenga los primeros 6 decimales correctos. Para hacer esto defini cuatro variables: "sumar" de tipo bool(nos indicara si debemos sumar
o restar cada vez que ejecutemos la serie de Leibniz),"denominador" de tipo double(se incrementara en dos en cada ejecucion la serie de Leibniz),"cuarto" de tipo double(sera
1/4 de pi) y "anterior" de tipo double (servirá para armar la condicion del do while). el do while ejecuta la serue de Leibniz donde el contador que pasamos por referencia
se incrementará en uno en cada ejecución del do while. El while se ejecutara mientras el valor absoluto de la resta de las variable "cuarto" y "anterior" sea mayor a
0.000000326795 siendo el primer valor de pi con 6 decimales correctos el primer numero de esta serie que se encuentra de este intervalo. Esta funcion devuelve la variable
"cuarto" a "pi"(variable del main) quien la recibe multiplicandola por cuatro para llegar al valor que buscamos. Para finalizar imprimimos en pantalla el valor de pi y las
veces que tuvo que ejecutar la serie de Leibniz a traves de la variable "contador". El codigo tarde unos 10 minutos en llegar al resultado. Le deje comentado un cout de la
evolución del while en tiempo de ejecución.


Para llegar a este numero 0.000000326795 tuve que comparar los valores de la posicisiones ("1530010"-"1530009")/4 con los valores de las posiciones ("1530009"-"1530008")/4
("1530010"-"1530009")/4= 0.00000032679 4929339513373633963055908679962158203125
("1530009"-"1530008")/4= 0.00000032679 5142946423311514081433415412902832031250
notando asi que los valores se empezaban a diferenciar en el numero 12 y 13 despues de la coma. En ("1530010"-"1530009")/4 decia 49 mientras que en el otro decia 51 por lo que
tome este intervalo 0.000000326795 en el que se encontraba ("1530010"-"1530009")/4 y no ("1530010"-"1530009")/4 asegurandome asi que iba a finalizar el while en el primer valor
de pi usando la serie de Leibniz con los 6 primero numeros despues de la coma correctos.
