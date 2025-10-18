#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <cstring> 

template <typename T>
class Simulador2D {
private:
    T **_grid;
    int _filas;
    int _columnas;

    T *_fuentes;
    int *_fuenteFilas;
    int *_fuenteCols;
    int _numFuentes;
    int _capacidadFuentes;

    float _constantes[3];

    void asignarGridVacio(int f, int c) {
        _filas = f;
        _columnas = c;
        _grid = nullptr;
        try {
            _grid = new T*[_filas];
            for (int i = 0; i < _filas; ++i) {
                _grid[i] = new T[_columnas];
                for (int j = 0; j < _columnas; ++j) _grid[i][j] = static_cast<T>(0);
            }
        } catch (const std::bad_alloc &e) {
            std::cerr << "Error de asignacion de memoria para la grid: " << e.what() << std::endl;
            if (_grid) {
                for (int i = 0; i < _filas; ++i) {
                    if (_grid[i]) delete[] _grid[i];
                }
                delete[] _grid;
                _grid = nullptr;
            }
            throw;
        }
    }

    void liberarGrid() {
        if (!_grid) return;
        for (int i = 0; i < _filas; ++i) delete[] _grid[i];
        delete[] _grid;
        _grid = nullptr;
        _filas = 0;
        _columnas = 0;
    }

    void copiarGridDesde(T** src, int filasSrc, int colsSrc) {
        int minF = (filasSrc < _filas) ? filasSrc : _filas;
        int minC = (colsSrc < _columnas) ? colsSrc : _columnas;
        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                _grid[i][j] = src[i][j];
    }

    void liberarFuentesAux() {
        if (_fuentes) delete[] _fuentes;
        if (_fuenteFilas) delete[] _fuenteFilas;
        if (_fuenteCols) delete[] _fuenteCols;
        _fuentes = nullptr;
        _fuenteFilas = nullptr;
        _fuenteCols = nullptr;
        _numFuentes = 0;
        _capacidadFuentes = 0;
    }

    void asegurarCapacidadFuentes(int minCap) {
        if (_capacidadFuentes >= minCap) return;
        int nuevaCap = (_capacidadFuentes == 0) ? 4 : _capacidadFuentes * 2;
        while (nuevaCap < minCap) nuevaCap *= 2;

        T *nFuentes = nullptr;
        int *nFilas = nullptr;
        int *nCols = nullptr;
        try {
            nFuentes = new T[nuevaCap];
            nFilas = new int[nuevaCap];
            nCols = new int[nuevaCap];
        } catch (const std::bad_alloc &e) {
            std::cerr << "Error al expandir vector de fuentes: " << e.what() << std::endl;
            delete[] nFuentes; delete[] nFilas; delete[] nCols;
            throw;
        }

        for (int i = 0; i < _numFuentes; ++i) {
            nFuentes[i] = _fuentes[i];
            nFilas[i] = _fuenteFilas[i];
            nCols[i] = _fuenteCols[i];
        }

        for (int i = _numFuentes; i < nuevaCap; ++i) {
            nFilas[i] = -1; nCols[i] = -1;
        }

        delete[] _fuentes; delete[] _fuenteFilas; delete[] _fuenteCols;
        _fuentes = nFuentes;
        _fuenteFilas = nFilas;
        _fuenteCols = nCols;
        _capacidadFuentes = nuevaCap;
    }

public:
    Simulador2D(int f = 1, int c = 1) :
        _grid(nullptr), _filas(0), _columnas(0),
        _fuentes(nullptr), _fuenteFilas(nullptr), _fuenteCols(nullptr),
        _numFuentes(0), _capacidadFuentes(0)
    {
        if (f <= 0) f = 1;
        if (c <= 0) c = 1;
        asignarGridVacio(f,c);
        _constantes[0] = 1.0f;
        _constantes[1] = 1.0f;
        _constantes[2] = 1.0f;
    }

    Simulador2D(const Simulador2D<T>& other) :
        _grid(nullptr), _filas(0), _columnas(0),
        _fuentes(nullptr), _fuenteFilas(nullptr), _fuenteCols(nullptr),
        _numFuentes(0), _capacidadFuentes(0)
    {
        _constantes[0] = other._constantes[0];
        _constantes[1] = other._constantes[1];
        _constantes[2] = other._constantes[2];

        asignarGridVacio(other._filas, other._columnas);
        copiarGridDesde(other._grid, other._filas, other._columnas);

        if (other._numFuentes > 0) {
            asegurarCapacidadFuentes(other._numFuentes);
            for (int i = 0; i < other._numFuentes; ++i) {
                _fuentes[i] = other._fuentes[i];
                _fuenteFilas[i] = other._fuenteFilas[i];
                _fuenteCols[i] = other._fuenteCols[i];
            }
            _numFuentes = other._numFuentes;
        }
    }

    Simulador2D<T>& operator=(const Simulador2D<T>& other) {
        if (this == &other) return *this;
        liberarGrid();
        liberarFuentesAux();

        _constantes[0] = other._constantes[0];
        _constantes[1] = other._constantes[1];
        _constantes[2] = other._constantes[2];

        asignarGridVacio(other._filas, other._columnas);
        copiarGridDesde(other._grid, other._filas, other._columnas);

        if (other._numFuentes > 0) {
            asegurarCapacidadFuentes(other._numFuentes);
            for (int i = 0; i < other._numFuentes; ++i) {
                _fuentes[i] = other._fuentes[i];
                _fuenteFilas[i] = other._fuenteFilas[i];
                _fuenteCols[i] = other._fuenteCols[i];
            }
            _numFuentes = other._numFuentes;
        }
        return *this;
    }

    ~Simulador2D() {
        liberarGrid();
        liberarFuentesAux();
    }

    void redimensionarGrid(int nuevaF, int nuevaC) {
        if (nuevaF <= 0) nuevaF = 1;
        if (nuevaC <= 0) nuevaC = 1;

        T **nueva = nullptr;
        try {
            nueva = new T*[nuevaF];
            for (int i = 0; i < nuevaF; ++i) {
                nueva[i] = new T[nuevaC];
                for (int j = 0; j < nuevaC; ++j) nueva[i][j] = static_cast<T>(0);
            }
        } catch (const std::bad_alloc &e) {
            std::cerr << "Error al crear grid temporal en redimensionar: " << e.what() << std::endl;
            if (nueva) {
                for (int i = 0; i < nuevaF; ++i) delete[] nueva[i];
                delete[] nueva;
            }
            throw;
        }

        int minF = (nuevaF < _filas) ? nuevaF : _filas;
        int minC = (nuevaC < _columnas) ? nuevaC : _columnas;
        for (int i = 0; i < minF; ++i)
            for (int j = 0; j < minC; ++j)
                nueva[i][j] = _grid[i][j];

        liberarGrid();
        _grid = nueva;
        _filas = nuevaF;
        _columnas = nuevaC;

        for (int k = 0; k < _numFuentes; ++k) {
            if (_fuenteFilas[k] >= _filas || _fuenteCols[k] >= _columnas ||
                _fuenteFilas[k] < 0 || _fuenteCols[k] < 0) {
                _fuenteFilas[k] = -1;
                _fuenteCols[k] = -1;
            }
        }
    }

    void agregarFuente(T valor) {
        asegurarCapacidadFuentes(_numFuentes + 1);
        _fuentes[_numFuentes] = valor;
        _fuenteFilas[_numFuentes] = -1;
        _fuenteCols[_numFuentes] = -1;
        ++_numFuentes;
    }

    void agregarFuenteEn(T valor, int f, int c) {
        if (f < 0 || c < 0 || f >= _filas || c >= _columnas) {
            std::cerr << "Advertencia: posicion de fuente fuera del rango. Fuente añadida sin posicion.\n";
            agregarFuente(valor);
            return;
        }
        asegurarCapacidadFuentes(_numFuentes + 1);
        _fuentes[_numFuentes] = valor;
        _fuenteFilas[_numFuentes] = f;
        _fuenteCols[_numFuentes] = c;
        _grid[f][c] = valor;
        ++_numFuentes;
    }

    bool posicionarFuente(int indice, int f, int c) {
        if (indice < 0 || indice >= _numFuentes) return false;
        if (f < 0 || c < 0 || f >= _filas || c >= _columnas) return false;
        _fuenteFilas[indice] = f;
        _fuenteCols[indice] = c;
        _grid[f][c] = _fuentes[indice];
        return true;
    }

    void aplicarFuenteEnCelda(int f, int c, T valor) {
        if (f < 0 || c < 0 || f >= _filas || c >= _columnas) {
            std::cerr << "Posición fuera de la grid.\n";
            return;
        }
        for (int i = 0; i < _numFuentes; ++i) {
            if (_fuentes[i] == valor) {
                _fuenteFilas[i] = f;
                _fuenteCols[i] = c;
                _grid[f][c] = valor;
                return;
            }
        }
        agregarFuenteEn(valor, f, c);
    }

    void simularPaso() {
        if (_filas <= 0 || _columnas <= 0) return;

        T **temp = nullptr;
        try {
            temp = new T*[_filas];
            for (int i = 0; i < _filas; ++i)
                temp[i] = new T[_columnas];
        } catch (const std::bad_alloc &e) {
            std::cerr << "Error al asignar matriz temporal en simularPaso: " << e.what() << std::endl;
            if (temp) {
                for (int i = 0; i < _filas; ++i) delete[] temp[i];
                delete[] temp;
            }
            throw;
        }

        for (int i = 0; i < _filas; ++i)
            for (int j = 0; j < _columnas; ++j)
                temp[i][j] = static_cast<T>(0);

        for (int i = 0; i < _filas; ++i) {
            for (int j = 0; j < _columnas; ++j) {
                if (i == 0 || i == _filas-1 || j == 0 || j == _columnas-1) {
                    temp[i][j] = _grid[i][j];
                    continue;
                }
                temp[i][j] = (_grid[i-1][j] + _grid[i+1][j] + _grid[i][j-1] + _grid[i][j+1]) / 4.0;
            }
        }

        for (int i = 0; i < _filas; ++i)
            for (int j = 0; j < _columnas; ++j)
                _grid[i][j] = temp[i][j];

        for (int i = 0; i < _filas; ++i) delete[] temp[i];
        delete[] temp;

        for (int k = 0; k < _numFuentes; ++k) {
            if (_fuenteFilas[k] >= 0 && _fuenteCols[k] >= 0)
                _grid[_fuenteFilas[k]][_fuenteCols[k]] = _fuentes[k];
        }
    }

    void imprimirGrid(std::ostream &o = std::cout, int anchoCol = 8, int precision = 2) const {
        for (int i = 0; i < _filas; ++i) {
            for (int j = 0; j < _columnas; ++j) {
                o << "|" << std::setw(anchoCol) << std::fixed << std::setprecision(precision) << _grid[i][j] << " ";
            }
            o << "|\n";
        }
    }
};

// ===================== Ejemplo =====================

int main() {
    std::cout << "--- Simulador Genérico de Fluidos (Difusión) ---\n\n";

    Simulador2D<float> simF(5,5);
    std::cout << ">> Inicializando Sistema (Tipo FLOAT) <<\n";
    std::cout << "Creando Grid (FLOAT) de 5x5...\n\n";

    std::cout << ">> Agregando Fuentes de Concentración <<\n";
    simF.agregarFuenteEn(100.0f, 2, 2);
    simF.agregarFuenteEn(50.0f, 4, 0);

    std::cout << "\n--- Grid Inicial (Paso 0) ---\n";
    simF.imprimirGrid(std::cout, 8, 2);

    std::cout << "\nOpción: Simular 1 Paso\n";
    simF.simularPaso();

    std::cout << "\n--- Grid Después del Paso 1 ---\n";
    simF.imprimirGrid(std::cout, 8, 2);

    std::cout << "\nOpción: Redimensionar\n";
    simF.redimensionarGrid(6,6);
    std::cout << "Redimensionando Grid a 6x6. Datos copiados.\n";

    std::cout << "\nOpción: Salir\n";
    return 0;
} 