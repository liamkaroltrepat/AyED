#include <iostream>
#include <fstream>
#include "rwstring.hpp"

using namespace std;

const int dprod = 10; // Longitud del producto
const int dprov = 19; // Longitud de la provincia
const int cprod = 100; // Cantidad de productos. No deberian ser mas de 100 productos (arreglos estaticos)
const int cdocks = 8; // Cantidad de docks

struct Despacho {
    int nroDock;        // se podria llamar Registro pero me parecio que en este caso Despacho era un buen nombre
    string producto;    
    string provincia;  
    int cantidad;      
};


struct DockInfo {
    int totalDespachos;          // total de despachos realizados en el dock
    string productos[cprod];       // no se cuantos productos van a ser
    int cantidades[cprod];         // cantidades totales de cada producto
    int productosCount;          // Número de productos únicos
};

fstream& operator>>(fstream &fs, Despacho &ds) {
    fs.read(reinterpret_cast<char*>(&ds.nroDock), sizeof(ds.nroDock));
    ds.producto = readstring(fs, dprod);
    ds.provincia = readstring(fs, dprov);
    fs.read(reinterpret_cast<char*>(&ds.cantidad), sizeof(ds.cantidad));
    return fs;
}

void procesarDespachos(Despacho despachos[], int cantidadDespachos, DockInfo docks[]) {
    for (int i = 0; i < cdocks; i++) {
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

void listarDocks(DockInfo docks[]) {
    for (int i = 0; i < 8; i++) {
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

template <typename T, typename Comparator>
int buscarExtremo(T elementos[], int cantidad, Comparator criterio) {
    int extremoIndex = 0;
    for (int i = 1; i < cantidad; i++) {
        if (criterio(elementos[i], elementos[extremoIndex])) {
            extremoIndex = i;
        }
    }
    return extremoIndex;
}

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
    int dockMinIndex = buscarExtremo(docks, 8, [](DockInfo& a, DockInfo& b) {
        return a.totalDespachos < b.totalDespachos;
    });
    DockInfo& dockMin = docks[dockMinIndex];

    cout << "\nDock con menos despachos: " << dockMinIndex
         << " (" << dockMin.totalDespachos << " despachos)\n";

    // Producto con mayor cantidad en ese dock
    int maxProductoIndex = buscarExtremo(dockMin.cantidades, dockMin.productosCount, [](int a, int b) {
        return a > b;
    });
    cout << "Producto más despachado: " << dockMin.productos[maxProductoIndex]
         << " (" << dockMin.cantidades[maxProductoIndex] << " unidades)\n";

    // Listar provincias y cantidades despachadas para el producto más despachado en ese dock
    listarDespachosPorProducto(dockMin, dockMin.productos[maxProductoIndex], despachos, cantidadDespachos, docks);

    return 0;
}
