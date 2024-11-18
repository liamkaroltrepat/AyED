#include <iostream>
#include <fstream>
#include "rwstring.hpp"
#include "listas.hpp"

using namespace std;


struct Registro {
    string producto;
    string lote;
    int cant;
};

struct Lote {
    string nom_lote;
    int cant;
};

struct Stock {
    string nom_prod;
    int stk_prod = 0; //podria inicializar abajo
    Nodo<Lote>* l_lotes = nullptr; // aca tmb
};

struct Pedido{
    string nom_prod;
    int cant = 0;
};


const int dprod = 10;
const int dlote = 6;
ostream& operator << (ostream& os, const Stock& s)
{
    os << s.nom_prod << "\t" << s.stk_prod;
    return os;
}

ostream& operator << (ostream& os, const Lote& l)
{
    os << l.nom_lote << "\t" << l.cant;
    return os;
}
istream& operator >> (istream& is, Pedido& ped)  
{
    is >> ped.nom_prod >> ped.cant;
    return is;
}
ostream& operator << (ostream& os, Pedido& ped)
{
    os << ped.nom_prod << " - " << ped.cant;
    return os;
}


fstream& operator >> (fstream& fs, Registro &reg)
{
    reg.producto = readstring(fs, dprod) ;
    reg.lote = readstring(fs, dlote);
    fs.read(reinterpret_cast< char * >(&reg.cant),sizeof(reg.cant));
    return fs;

}        
template <typename T> int criterioJugador(T a, T b) //aca puse un criterio como template unicamente porque cree la casualidad de que hay dos estructuras que tienen un elemento nom_prod y las dos van a ordenar por ese elemento
{
   return a.nom_prod.compare(b.nom_prod);
}
int criterioLote(Lote a, Lote b)
{
    return b.nom_lote.compare(a.nom_lote);
}

void listarStock (Nodo<Stock>* l_stock){ // no lo paso por referencia por eso no creo punteros auxiliares
    cout << "Producto \t Cantidad" << endl; 
    mostrar(l_stock);
}



Nodo<Pedido>* realizarPedido (){
    cout << "Usted ha solicitado realizar un pedido" << endl;
    cout << "Ingrese el producto que quiera pedir junto a la cantidad de unidades del mismo" << endl;
    cout << "Repita esto para todos los productos que desee"<< endl;
    cout << "Nota: si ingresa mas de una vez un producto las cantidades se sumarán" <<endl;
    Pedido ped;
    Pedido aux;
    Nodo<Pedido>* p_prd_ped;
    Nodo<Pedido>* p_ped = nullptr;
    while(cin >> ped){
        aux.nom_prod = ped.nom_prod;
        p_prd_ped = insertar_unico(aux, p_ped, criterioJugador);
        p_prd_ped->dato.cant += ped.cant;
    }
    cout << "\nPedido Realizado con exito" << endl;
    return p_ped;
}



bool verificarPedido(Nodo<Pedido>* p_ped, Nodo<Stock>* l_stk) {
    bool verificado = true; // Suponemos que el pedido será válido inicialmente

    while (p_ped != nullptr) {
        // Si el stock está vacío o ya no quedan productos para comparar
        if (l_stk == nullptr || p_ped->dato.nom_prod < l_stk->dato.nom_prod) {
            // Producto no encontrado en stock
            cout << "Stock insuficiente de " << p_ped->dato.nom_prod
                 << ", cantidad faltante " << p_ped->dato.cant << endl;
            verificado = false;
            p_ped = p_ped->sig;
        } 
        else if (p_ped->dato.nom_prod == l_stk->dato.nom_prod) {
            // Producto encontrado en stock
            if (l_stk->dato.stk_prod < p_ped->dato.cant) {
                cout << "Stock insuficiente de " << p_ped->dato.nom_prod
                     << ", cantidad faltante " << p_ped->dato.cant - l_stk->dato.stk_prod << endl;
                verificado = false;
            }
            // Avanzamos ambas listas porque este producto ya fue procesado
            p_ped = p_ped->sig;
            l_stk = l_stk->sig;
        } 
        else {
            // Avanzamos el stock porque no encontramos coincidencia
            l_stk = l_stk->sig;
        }
    }
    

    return verificado;
}


void actualizarStockListado(Nodo<Stock>* &l_stk, Nodo<Pedido>* p_ped)
{
    Nodo<Stock>* p_aux_stock = l_stk; // Puntero auxiliar para recorrer el stock

    while (p_ped != nullptr) {
        if (p_aux_stock != nullptr && p_ped->dato.nom_prod == p_aux_stock->dato.nom_prod) {
            // Producto encontrado en el stock, procesamos el pedido
            int cant_a_despachar = p_ped->dato.cant;

            cout << "Producto: " << p_ped->dato.nom_prod << ", Cantidad despachada: " << cant_a_despachar << endl;
            cout << "Lotes utilizados: ";

            p_aux_stock->dato.stk_prod -= cant_a_despachar;  // Reducimos el stock general

            Nodo<Lote>* l_lotes = p_aux_stock->dato.l_lotes; // Lista de lotes del producto
            while (l_lotes != nullptr && cant_a_despachar > 0) {
                if (l_lotes->dato.cant >= cant_a_despachar) {
                    // Si el lote tiene suficiente cantidad para cubrir el pedido
                    l_lotes->dato.cant -= cant_a_despachar;
                    cout << " - " << l_lotes->dato.nom_lote;
                    cant_a_despachar = 0;  // Pedido completamente despachado
                } else {
                    // Si el lote no tiene suficiente cantidad, consumimos todo el lote
                    cant_a_despachar -= l_lotes->dato.cant;
                    cout << " - " << l_lotes->dato.nom_lote;
                    Nodo<Lote>* aux = l_lotes;
                    l_lotes = l_lotes->sig;
                    delete aux;  // Eliminamos el lote consumido
                }
                cout << endl;
            }
            // Avanzamos en ambas listas (pedido y stock)
            p_ped = p_ped->sig;
            p_aux_stock = p_aux_stock->sig;
        }
        else {
            // Si el producto no coincide con el stock, simplemente avanzamos en el stock
            p_aux_stock = p_aux_stock->sig;
        }
    }
}

int main() {
    Nodo<Stock>* l_stock = nullptr;

    fstream arch;
    arch.open("Datos.bin", ios::in | ios::binary);
    if (!arch) {
        cout << "Error al abrir el archivo" << endl;
        return EXIT_FAILURE;
    }  
    Registro reg;
    Stock stk;
    Lote lot;
    Nodo<Stock>* p_stk; 
    while (arch >> reg) {
        stk.nom_prod = reg.producto;
        p_stk = insertar_unico(stk, l_stock, criterioJugador);
        p_stk->dato.stk_prod += reg.cant;
        lot.cant = reg.cant;
        lot.nom_lote = reg.lote;
        insertar(lot, p_stk->dato.l_lotes, criterioLote);
    }
    arch.close();

    // Mostrar stock inicial
    cout << "Stock inicial" << endl;
    listarStock(l_stock);
    Nodo<Pedido>* p_ped;
    string realizar_pedido; // para ver si el usuario quiere realizar uno o más pedidos

    cout << "¿Desea realizar un pedido?. INGRESE \"si\" EN CASO AFIRMATIVO." << endl;
    cin >> realizar_pedido;  // Primero pedimos si desea realizar un pedido
    while (realizar_pedido == "si") {
        p_ped = realizarPedido();  // Realizar el pedido
        cin.clear();
        clearerr(stdin);
        mostrar(p_ped);
        bool verificado = verificarPedido(p_ped, l_stock);  // Verificar el pedido

        if (verificado) {
            cout << "\nPedido verificado con exito (podemos despachar y entregar)" << endl;
            actualizarStockListado(l_stock, p_ped);  // Actualizar stock
            cout << "\nPedido entregado con exito (stock actualizado)" << endl;
        }

        // Limpiar la lista de pedidos
        while (p_ped != nullptr) {
            pop(p_ped);
        }

        // Preguntar si desea realizar otro pedido
        cout << "¿Desea realizar otro pedido?. INGRESE \"si\" EN CASO AFIRMATIVO." << endl;
        cin >> realizar_pedido;
    }

    // Mostrar stock final
    cout << "Stock final" << endl;
    listarStock(l_stock);

    return 0;
}
