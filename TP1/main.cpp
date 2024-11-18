#include <iostream>
#include <fstream>
#include "rwstring.hpp"

using namespace std;

const int dprod = 10; // Longitud del producto
const int dprov = 19; // Longitud de la provincia
const int cprod = 100; // Cantidad de productos (maxima no de berian ser mas)
const int clocks = 8; // Cantidad de Locks

struct Despacho {
    int nroDock;        // Número del dock (0 a 7)
    string producto;    // Producto (10 caracteres)
    string provincia;   // Provincia (19 caracteres)
    int cantidad;       // Cantidad despachada
};

struct DockInfo {
    int totalDespachos;          // Total de despachos realizados en el dock
    string productos[cprod];       // Lista de productos únicos
    int cantidades[cprod];         // Cantidades totales de cada producto
    int productosCount;          // Número de productos únicos
};

fstream& operator>>(fstream &fs, Despacho &ds) {
    fs.read(reinterpret_cast<char*>(&ds.nroDock), sizeof(ds.nroDock));
    ds.producto = readstring(fs, dprod);
    ds.provincia = readstring(fs, dprov);
    fs.read(reinterpret_cast<char*>(&ds.cantidad), sizeof(ds.cantidad));
    return fs;
}

// Función para procesar los despachos
void procesarDespachos(Despacho despachos[], int cantidadDespachos, DockInfo docks[]) {
    for (int i = 0; i < clocks; i++) {
        docks[i].totalDespachos = 0;
        docks[i].productosCount = 0;
    }

    for (int i = 0; i < cantidadDespachos; i++) {
        Despacho& despacho = despachos[i];
        DockInfo& dock = docks[despacho.nroDock];
        dock.totalDespachos++;

        // Buscar el producto en el dock
        int productoIndex = -1;
        for (int j = 0; j < dock.productosCount; j++) {
            if (dock.productos[j] == despacho.producto) {
                productoIndex = j;
                break;
            }
        }

        // Agregar producto nuevo si no existe
        if (productoIndex == -1) {
            productoIndex = dock.productosCount++;
            dock.productos[productoIndex] = despacho.producto;
            dock.cantidades[productoIndex] = 0;
        }

        dock.cantidades[productoIndex] += despacho.cantidad;
    }
}

// Función para listar despachos por dock
void listarDocks(DockInfo docks[]) {
    for (int i = 0; i < clocks; i++) {
        DockInfo& dock = docks[i];
        cout << "Dock " << i << ": " << dock.totalDespachos << " despachos" << endl;

        // Ordenar productos
        for (int j = 0; j < dock.productosCount - 1; j++) {
            for (int k = j + 1; k < dock.productosCount; k++) {
                if (dock.productos[j] > dock.productos[k]) {
                    // Intercambiar productos y cantidades
                    swap(dock.productos[j], dock.productos[k]);
                    swap(dock.cantidades[j], dock.cantidades[k]);
                }
            }
        }

        for (int j = 0; j < dock.productosCount; j++) {
            cout << "  - " << dock.productos[j] << ": " << dock.cantidades[j] << endl;
        }
    }
}

// Criterio de comparación para encontrar el dock con menos despachos
int criterioMenor(const DockInfo& a, const DockInfo& b) {
    if (a.totalDespachos < b.totalDespachos) {
        return 1; // a es menor, buscamos el mínimo
    } else if (a.totalDespachos > b.totalDespachos) {
        return -1; // a es mayor
    } else {
        return 0; // son iguales
    }
}

// Criterio de comparación para encontrar el producto con mayor cantidad
int criterioMayor(const int& a, const int& b) {
    return (a > b) ? 1 : (a < b) ? -1 : 0;
}
// Función genérica para buscar el extremo (min o max) según el criterio
template <typename T>
int buscarExtremo(T elementos[], int cantidad, int (*criterio)(const T&, const T&)) {
    int extremoIndex = 0;
    for (int i = 1; i < cantidad; i++) {
        if (criterio(elementos[i], elementos[extremoIndex]) > 0) {
            extremoIndex = i;
        }
    }
    return extremoIndex;
}

// Función para listar los despachos de un producto específico en un dock
void listarDespachosPorProducto(DockInfo& dock, const string& producto, Despacho despachos[], int cantidadDespachos, DockInfo docks[]) {
    cout << "\nDespachos de producto: " << producto << " en Dock: " << &dock - docks << endl;
    for (int i = 0; i < cantidadDespachos; i++) {
        if (despachos[i].nroDock == &dock - docks && despachos[i].producto == producto) {
            cout << "  Provincia: " << despachos[i].provincia << ", Cantidad: " << despachos[i].cantidad << endl;
        }
    }
}

int main() {
    const int MAX_DESPACHOS = 1000;
    Despacho despachos[MAX_DESPACHOS];
    int cantidadDespachos = 0;

    // Leer archivo binario en el main
    fstream archivo("Datos.bin", ios::in | ios::binary);
    if (!archivo) {
        cout << "Error al abrir el archivo Datos.bin" << endl;
        return 1;
    }

    while (archivo >> despachos[cantidadDespachos]) {
        cantidadDespachos++;
    }
    archivo.close();

    // Procesar y listar los despachos
    DockInfo docks[8];
    procesarDespachos(despachos, cantidadDespachos, docks);

    cout << "Listado de despachos por dock:\n";
    listarDocks(docks);

    // Encontrar dock con menos despachos
    int dockMinIndex = buscarExtremo(docks, 8, criterioMenor);
    DockInfo& dockMin = docks[dockMinIndex];

    cout << "\nDock con menos despachos: " << dockMinIndex
         << " (" << dockMin.totalDespachos << " despachos)\n";

    // Producto con mayor cantidad en ese dock
    int maxProductoIndex = buscarExtremo(dockMin.cantidades, dockMin.productosCount, criterioMayor);
    cout << "Producto más despachado: " << dockMin.productos[maxProductoIndex]
         << " (" << dockMin.cantidades[maxProductoIndex] << " unidades)\n";

    // Listar provincias y cantidades despachadas para el producto más despachado en ese dock
    listarDespachosPorProducto(dockMin, dockMin.productos[maxProductoIndex], despachos, cantidadDespachos, docks);

    return 0;
}
