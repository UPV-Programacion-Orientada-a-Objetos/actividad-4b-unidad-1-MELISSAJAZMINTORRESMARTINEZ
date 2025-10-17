[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WYgN10Ds)
[![Open in Codespaces](https://classroom.github.com/assets/launch-codespace-2972f46106e565e64193e422d61a12cf1da4916b45550586e14ef0a7c637dd04.svg)](https://classroom.github.com/open-in-codespaces?assignment_repo_id=21147196)

# Actividad Práctica: Simulador Genérico de Fluidos 2D (Modelado Físico)

## Definición del Problema a Resolver

Una firma de ingeniería requiere un simulador de bajo nivel para modelar la propagación de una magnitud física (como calor, presión o concentración de un químico) a través de una superficie **2D**. El sistema debe ser flexible para manejar diferentes tipos de magnitudes (enteros para concentración discreta, flotantes para presión, etc.).

El objetivo es construir una clase genérica `Simulador2D<T>` que gestione:

  1. Una Matriz Dinámica **2D** (`T**`) que representa la cuadrícula del espacio físico. Sus dimensiones deben ser redimensionables.
  2. Un Vector Dinámico de tipo `T` para almacenar los parámetros de contorno o fuente de la simulación.
  3.  Un Vector Estático de tipo `float` para almacenar las constantes físicas invariables de la simulación.

El caso de estudio se centrará en la propagación de la concentración de un químico utilizando una versión simplificada de las ecuaciones de difusión.

---

## Temas Relacionados y Necesarios

Para resolver este problema, se deberá integrar los siguientes conocimientos:

| Tema Principal                                  | Concepto a Aplicar                                                                                                                                                               |
| :---------------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Programación Orientada a Objetos (POO) en C++    | Definición de una Clase Genérica (`Simulador2D<T>`), encapsulamiento, constructor, destructor y métodos.                                                                       |
| Datos Genéricos (Templates)                      | Uso de `typename T` en la clase para que la Matriz, el Vector Dinámico y las operaciones funcionen con `int`, `float` o `double`.                                             |
| Matrices Multidimensionales Dinámicas           | Gestión manual de la matriz **2D** (`T**`) mediante punteros dobles, incluyendo la asignación, liberación y el proceso de redimensión (`new[]` y `delete[]`).                 |
| Vectores Dinámicos (Punteros)                  | Uso de un puntero simple (`T*`) para gestionar el Vector de Fuentes y su expansión manual si es necesario.                                                                    |
| Vectores Estáticos                             | Uso de un arreglo fijo (`float[]`) dentro de la clase para almacenar constantes de simulación.                                                                                   |
| Caso de Estudio (Física/Matemática)             | Comprensión básica de la Ecuación de Difusión (o Laplaciana **2D**) para la propagación de valores en una cuadrícula. |

---

## Definición y Detalles del Proceso a Desarrollar

### A. Estructura de la Clase y Atributos

```cpp
template <typename T>
class Simulador2D {
private:
    // Matriz de la cuadrícula (Dinámica y Genérica)
    T **_grid;
    int _filas;
    int _columnas;

    // Vector de Fuentes (Dinámico y Genérico)
    T *_fuentes;
    int _numFuentes;
    int _capacidadFuentes;

    // Vector de Constantes (Estático)
    float _constantes[3]; // [0] = Coeficiente de Difusión, [1] = DeltaX, [2] = DeltaT

public:
    // MÉTODOS PÚBLICOS OBLIGATORIOS:
    Simulador2D(int f, int c); // Constructor
    ~Simulador2D(); // Destructor
    void redimensionarGrid(int nuevaF, int nuevaC);
    void agregarFuente(T valor); // Expansión manual del vector dinámico si es necesario
    void simularPaso(); // Implementación del algoritmo
    // ... otros métodos de acceso y visualización
};
```

### B. Proceso de Solución Detallado

1.  **Gestión de Memoria y Redimensión (Matriz):** El constructor debe asignar el `_grid` (matriz **2D**) y el destructor debe liberar la memoria correctamente. El método `redimensionarGrid` debe manejar la creación de una nueva matriz, copiar datos y liberar la antigua, conservando la simulación anterior.

2.  **Gestión de Vector Dinámico (Fuentes):** El método `agregarFuente` debe verificar si `_numFuentes` alcanza `_capacidadFuentes`. Si es así, debe duplicar la capacidad asignando un nuevo puntero, copiando los datos antiguos y liberando el puntero antiguo (`_fuentes`).

3.  **Algoritmo de Simulación (Difusión Simplificada):** El método `simularPaso()` debe aplicar la siguiente lógica a cada celda interna de la matriz (excluyendo bordes):

    *   El valor de la celda en el nuevo paso de tiempo es el promedio de sus cuatro vecinos (arriba, abajo, izquierda, derecha) en el paso de tiempo anterior.
    *   Se requiere una matriz auxiliar (temporal) del mismo tamaño para almacenar los nuevos valores antes de copiarlos a la matriz principal.

    *   **Bordes:** Los valores de los bordes deben mantenerse constantes (Condición de contorno cero) o ser iguales a las fuentes definidas por el vector `_fuentes`.

4.  **Integridad Genérica:** Todos los métodos de la clase, especialmente `simularPaso` y `agregarFuente`, deben funcionar sin errores, independientemente de si **T** es `int` o `float`.

---

## Requerimientos Funcionales y No Funcionales

### Requisitos Funcionales

*   **Clase Genérica Completa:** Implementar la clase `Simulador2D<T>` y probarla con al menos dos tipos (`int` y `float`) en el main.
*   **Redimensión de Matriz:** El método `redimensionarGrid` debe funcionar correctamente.
*   **Expansión de Vector Dinámico:** El método `agregarFuente` debe implementar la lógica de duplicación de capacidad del vector `_fuentes`.
*   **Simulación:** Implementar `simularPaso()` con la lógica de promedio de vecinos, utilizando una matriz temporal para evitar errores de propagación inmediata.
*   **Visualización:** Implementar un método para imprimir el `_grid` a la consola.

### Requisitos No Funcionales

*   **Control de Memoria:** Prohibido el uso de `std::vector`, `std::array`, o cualquier contenedor de la **STL**. Se debe usar exclusivamente punteros (`new` y `delete`).
*   **Implementación POO:** Todo el trabajo de gestión de datos debe estar encapsulado en la clase `Simulador2D`.
*   **Manejo de Templates:** Las funciones deben ser verdaderamente genéricas para el tipo **T**.

---

## Ejemplo de Entradas y Salidas del Problema en Consola

```Bash
--- Simulador Genérico de Fluidos (Difusión) ---

>> Inicializando Sistema (Tipo FLOAT) <<
Creando Grid (FLOAT) de 5x5...

>> Agregando Fuentes de Concentración <<
Agregar Fuente 1 (Valor 100.0): Éxito.
Agregar Fuente 2 (Valor 50.0): Éxito.

>> Aplicando Valores Iniciales <<
Grid[2,2] = 100.0 (Fuente 1)
Grid[4,0] = 50.0 (Fuente 2)

--- Grid Inicial (Paso 0) ---
| 0.0  | 0.0 | 0.0   | 0.0 | 0.0 |
| 0.0  | 0.0 | 0.0   | 0.0 | 0.0 |
| 0.0  | 0.0 | 100.0 | 0.0 | 0.0 |
| 0.0  | 0.0 | 0.0  | 0.0  | 0.0 |
| 50.0 | 0.0 | 0.0  | 0.0  | 0.0 |

Opción: Simular 1 Paso

Simulando Propagación...

--- Grid Después del Paso 1 ---
| 0.0  | 0.0  | 0.0  | 0.0  | 0.0 |
| 0.0  | 0.0  | 25.0 | 0.0  | 0.0 |
| 12.5 | 25.0 | 0.0  | 25.0 | 0.0 |
| 12.5 | 0.0  | 0.0  | 0.0  | 0.0 |
| 50.0 | 12.5 | 0.0  | 0.0  | 0.0 |

Opción: Redimensionar
Redimensionando Grid a 6x6. Datos copiados.

Opción: Salir
Liberando memoria del Grid y Fuentes... Sistema cerrado.
```

---

## Temas Adicionales de Investigación Necesarios

Para llevar a cabo esta actividad con un nivel de ingeniería apropiado, los alumnos deberán investigar a fondo:

1. **Regla de los Tres/Cinco (C++)**: La necesidad crítica de implementar el Constructor de Copia, el Operador de Asignación y el Destructor para la clase `Simulador2D<T>` y cómo estos deben funcionar con estructuras dinámicas y templates.
2. **Asignación de Memoria Segura**: Técnicas para garantizar que new no falle, o cómo manejar la excepción `std::bad_alloc` cuando se expande el vector dinámico o la matriz.
3. **Implementación de Matriz 2D con Templates**: El uso de `T**` y la sintaxis correcta para acceder a los elementos: `datos[i][j]`.
4. **Algoritmos de Copia en C++**: La implementación eficiente de la función de copiado de datos al realizar la redimensión (`memcpy` o bucles manuales de asignación) para evitar la pérdida de rendimiento.