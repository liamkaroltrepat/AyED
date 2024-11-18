Descripcion del tp:
Use cuatro estructuras Registros para cargar los datos, Stock para manejar el stock, Lote que va a ver una lista de lotes por cada producto
y Pedido para manejar los pedidos que manejamos por teclado.
El la estructura stock voy a insertar por nombre de producto sin repetir incrementando cada vez que se repita el stock del producto en cuestion.
Ahi mismo tmb cargo en una variable de Tipo lote el Nombre del lote y la cantidad y lo inserto en la lista de lotes para ese producto.
Cierro el archivo.

Tengo una funcion listarstock que lo uso tanto para listar el stock inicia como el final.
Para manejar los pedidos que podrian ser infinitos cree una variable de tipo string.
Haré una pregunta al usuario de si quiere realizar un pedido y si responde "si" se hara sino se mostrará el stock final y terminará el programa.
Tengo una funcion realizarPedido que arma una lista del pedido cada elemento de la lista tiene como dato una variable del tipo Pedido
Lo que hago es insertar cada elemento de la lista del pedido con el mismo criterio que como inserté los elementos de la lista de stock para asi tener un poco mas de eficiencia en las otras funciones.
Bueno realizarpedido retorna un puntero a la lista del pedido. Verificar pedido se fija que tengamos todo lo del pedido en las cantidades pedidas.
Si tenemos devuelve True y sigue el programa sino ya emite el listado con las cosas que faltaron y vuelve a preguntar al usuario si quere realizar otro pedido.
Si tenemos se ejecute la funcio de actualizarstocklistado que actualiza el stock y emite un listado. 
Despues vaciamos la lista del pedido y volvemos a preguntar al cliente si quiere realizar otro pedido
Si no quiere mostramos stock final y vaciamos la lista de stock;

Integrantes: Liam Karol Trepat;
