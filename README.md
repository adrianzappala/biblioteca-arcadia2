# Biblioteca Arcadia

Sistema de gestión de biblioteca desarrollado en lenguaje C como Trabajo Práctico Final de la materia **Programación I**, de la Tecnicatura Universitaria en Programación de la UTN Facultad Regional Mar del Plata.

## Descripción

Biblioteca Arcadia permite administrar el catálogo de libros, los autores, los socios y los préstamos de una biblioteca. También incorpora un sistema de reportes estadísticos y recomendaciones de lectura.

## Funcionalidades

### 1. Gestión del catálogo
- Alta y modificación de libros.
- Listado de libros y consulta de disponibilidad.
- Búsqueda por ISBN, título, autor, género, año e inicial.
- Listado alfabético.
- Historial de búsquedas.
- Resumen de stock por autor.
- Listados mediante memoria dinámica y recursividad.

### 2. Gestión de autores
- Listado de autores.
- Consulta de obras asociadas a cada autor.

### 3. Gestión de socios
- Registro de nuevos socios.
- Listado y búsqueda por ID o DNI.
- Modificación de datos.
- Cambio de estado del socio.

### 4. Gestión de préstamos
- Registro de préstamos.
- Registro de devoluciones.
- Consulta de préstamos activos y devueltos.
- Historial completo.
- Identificación de préstamos vencidos.
- Control de disponibilidad de ejemplares.

### 5. Sistema de recomendaciones
Permite recomendar libros según:
- Autor seleccionado.
- Género seleccionado.

El sistema muestra los libros que tienen ejemplares disponibles y no están descatalogados.

### 6. Reportes
Incluye reportes sobre:
- Libros más prestados.
- Bestseller anual.
- Ranking de autores y géneros.
- Socios más activos.
- Préstamos vencidos.
- Estado del inventario.
- Libros menos solicitados.
- Préstamos por género y mes.

## Tecnologías y conceptos utilizados

- Lenguaje C.
- Code::Blocks y compilador MinGW/GCC.
- Estructuras.
- Funciones y modularización.
- Archivos binarios.
- Manejo de memoria dinámica.
- Búsqueda y ordenamiento.
- Recursividad.
- Pilas para el historial de búsquedas.

## Ejecución del programa

1. Abrir el proyecto `BibliotecaArcadia.cbp` desde Code::Blocks.
2. Compilar y ejecutar con la tecla `F9`.
3. Ingresar al menú principal.
4. Si el catálogo está vacío, utilizar la opción **Cargar obras iniciales al catálogo**.
5. Registrar socios y realizar préstamos para probar las funcionalidades.

## Almacenamiento de datos

El sistema utiliza archivos binarios `.dat` para almacenar la información de libros, socios y préstamos. Estos archivos se generan durante la ejecución del programa.

Los archivos de datos de prueba locales no forman parte del código fuente del repositorio.

## Proyecto académico

**Materia:** Programación I  
**Carrera:** Tecnicatura Universitaria en Programación  
**Institución:** Universidad Tecnológica Nacional – Facultad Regional Mar del Plata

